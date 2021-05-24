#include "lua.hpp"
#include "physic.hpp"
#include "render.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "level.hpp"
#include "sdl.hpp"
#include "utility.hpp"
#include "text.hpp"
#include "effect.hpp"
#include "main.hpp"
#include "interface.hpp"
#include "shader.hpp"
#include <detail/Userdata.h>
#include <algorithm>
using namespace luabridge;
using namespace std;
uint8_t prev_key[SDL_NUM_SCANCODES];
namespace lua {
lua_State *L;
string need_load;
map<unsigned int,unsigned int>timers;
vector<string>loaded;
float game_time=0;
int prev_time=0;
float time_scale=1.0f;
struct Thread {
	LuaRef l;
	bool used=true;
};
vector<Thread>threads;
int run_thread(void *ptr) {
	LuaRef *func=TYPE(LuaRef*,ptr);
	if(func->isFunction()) {
		(*func)();
	}
	return 0;
}

void create_thread(LuaRef val) {
	threads.push_back({val,true});
	SDL_CreateThread(run_thread,"lua_thread",&threads[threads.size()-1].l);
}
bool get_interval(unsigned int ms) {
	if(get_time()-timers[ms]>ms/time_scale) {
		return 1;
	}
	return 0;
}
void update_intervals() {
	for(auto &t : timers) {
		if(get_time()-t.second>t.first/time_scale)
			t.second=get_time();
	}
	while(prev_time<SDL_GetTicks()+1) {
		prev_time++;
		game_time+=time_scale;
	}
}
float get_time() {
	return game_time;
}
//НАЧАЛО КОСТЫЛЕЙ И ОСНОВНЫХ ПРИЧИН БАГОВ
void set_mask(Color &c) {
	scene_mask=c;
}
void print(LuaRef r) {
	interface.console.AddLog(r.tostring());
	cout<<r.tostring()<<endl;
}
Color &get_mask() {
	return scene_mask;
}
void clear_loaded_list() {
	loaded.clear();
}
void dofile(string file) {
	if(!exist_file(file))return;
	if(luaL_dofile(L, file.c_str())) {
		throw string(lua_tostring(L, -1));
	}
}

vector<string>get_table_keys(string name) {
	vector<string>keys;
	LuaRef g=getGlobal(L,"_G");
	while(name.find('.')!=name.npos) {
		string table(name,0,name.find('.'));
		name.erase(0,name.find('.')+1);
		g=g[table];
		if(g.isNil())return {};
	}
	if(!g.isTable())return {};
	g.push(L);
	push(L,Nil());
	while(lua_next(L,-2)!=0) {
		LuaRef key=LuaRef::fromStack(L,-2);
		LuaRef val=LuaRef::fromStack(L,-1);
		if(key.isString()) {
			if(val.isFunction())
				keys.push_back(key.cast<string>()+"(");
			else
				keys.push_back(key.cast<string>());
		}
		lua_pop(L,1);
	}
	sort(keys.begin(),keys.end());
	if(name!="")
		for(int q=0; q<keys.size(); q++)
			if(keys[q].find(name)!=0) {
				keys.erase(keys.begin()+q);
				q--;
			}
	return keys;
}

void dostring(string text) {
	if(luaL_dostring(L, text.c_str())) {
		throw string(lua_tostring(L, -1));
	}
}
void doscript(string file) {
	dofile(prefix+file+".lua");
}
void level(string str) {
	need_load=str;
}
void init_body(b2Body *body,bool ex) {
	if(B_DATA(body,script).size()) {
		bool ok=0;
		for(string l : loaded)
			if(B_DATA(body,script)==l)
				ok=1;
		if(!ok) {
			luaL_dostring(L,(B_DATA(body,script)+"=extend(Body)\n").c_str());
			doscript("bodies/"+B_DATA(body,script));
			loaded.emplace_back(B_DATA(body,script));
		}
		if(ex)
			getGlobal(L,B_DATA(body,script).c_str())["init"](body);
	}
}
void init_bodies() {
	for(auto body : bodies) {
		init_body(body.second);
	}
}

void update_bodies() {
	for(auto body : bodies) {
		if(B_DATA(body.second,script).size()) {
			if(getGlobal(L,B_DATA(body.second,script).c_str())["update"](body.second)) {
				destroy_body(body.second);
				return;
			}
		}
	}
}
void init_entity(Entity *entity,bool ex) {
	bool ok=0;
	for(string l : loaded)
		if(entity->type==l)
			ok=1;
	if(!ok) {
		luaL_dostring(L,(entity->type+"=extend(Entity)\n").c_str());
		doscript("entities/"+entity->type);
		loaded.emplace_back(entity->type);
	}
	if(ex) {
		getGlobal(L,entity->type.c_str())["init"](entity);
		getGlobal(L,"Entity")["init"](entity);
	}
}
void init_entities() {
	for(auto entity : entities)
		init_entity(entity.second);
}
void init_weapon(Entity *entity,bool ex) {
	bool ok=0;
	for(string l : loaded)
		if(entity->weapon.name==l)
			ok=1;
	if(!ok) {
		luaL_dostring(L,(entity->weapon.name+"=extend(Weapon)").c_str());
		doscript("weapon/"+entity->weapon.name);
	}
	if(ex) {
		getGlobal(L,"Weapon")["init"](&entity->weapon,entity);
		getGlobal(L,entity->weapon.name.c_str())["init"](&entity->weapon,entity);
		loaded.emplace_back(entity->weapon.name);
	}
}
void update_entities() {
	for(auto entity : entities) {
		if(entity.second) {
			if(getGlobal(L,"Entity")["update"](entity.second)||
					getGlobal(L,entity.second->type.c_str())["update"](entity.second)) {
				destroy_entity(entity.second);
				return;
			}
			if(entity.second->weapon.name.size()) {
				getGlobal(L,"Weapon")["update"](&entity.second->weapon,entity.second);
				getGlobal(L,entity.second->weapon.name.c_str())["update"](&entity.second->weapon,entity.second);
			}
		}
	}
}

int fire1(Entity *ent) {
	return getGlobal(L,ent->weapon.name.c_str())["fire1"](&ent->weapon,ent);
}
int fire2(Entity *ent) {
	return getGlobal(L,ent->weapon.name.c_str())["fire2"](&ent->weapon,ent);
}
int fire3(Entity *ent) {
	return getGlobal(L,ent->weapon.name.c_str())["fire3"](&ent->weapon,ent);
}
int fire4(Entity *ent) {
	return getGlobal(L,ent->weapon.name.c_str())["fire4"](&ent->weapon,ent);
}
void copy_prev_key() {
	memcpy(prev_key,SDL_GetKeyboardState(0),SDL_NUM_SCANCODES);
}
void gameloop() {
	getGlobal(L,"Level")["update"]();
	getGlobal(L,"level")["update"]();
	update_entities();
	update_bodies();
	update_intervals();
	effect::update();
	copy_prev_key();
}
short get_scancode(string k) {
	if(k=="up")		return SDL_SCANCODE_W;
	if(k=="down")	return SDL_SCANCODE_S;
	if(k=="left")	return SDL_SCANCODE_A;
	if(k=="right")	return SDL_SCANCODE_D;
	if(k=="jump")	return SDL_SCANCODE_SPACE;
	if(k=="action")	return SDL_SCANCODE_E;
	if(k=="1")		return SDL_SCANCODE_1;
	if(k=="2")		return SDL_SCANCODE_2;
	if(k=="3")		return SDL_SCANCODE_3;
	if(k=="4")		return SDL_SCANCODE_4;
	if(k=="5")		return SDL_SCANCODE_5;
	if(k=="6")		return SDL_SCANCODE_6;
	if(k=="7")		return SDL_SCANCODE_7;
	if(k=="8")		return SDL_SCANCODE_8;
	if(k=="9")		return SDL_SCANCODE_9;
	if(k=="0")		return SDL_SCANCODE_0;
	return -1;
}
void create_body_userdata(b2Body *b) {
	b->SetLuaUserData(new LuaRef(newTable(L)));
}
void create_entity_userdata(Entity *e) {
	e->lua_userdata=new LuaRef(newTable(L));
	for(auto b : e->bodies)
		create_body_userdata(b.second);
}
void create_userdata() {
	for(auto b : bodies)
		create_body_userdata(b.second);
	for(auto e : entities)
		create_entity_userdata(e.second);
}
bool get_key(string k) {
	short r=get_scancode(k);
	if(r!=-1)return key[r];
	else {
		if(k=="fire1" || k=="fire")
			return mouse.state==2&&mouse.b==SDL_BUTTON_LEFT;
		if(k=="fire2")	return mouse.state==2&&mouse.b==SDL_BUTTON_RIGHT;
	}
	return 0;
}
bool get_press_key(string k) {
	short r=get_scancode(k);
	if(r!=-1)return (key[r] && !prev_key[r]);
	else {
		if(k=="fire1" || k=="fire")
			return mouse.state==1&&mouse.b==SDL_BUTTON_LEFT;
		if(k=="fire2")	return mouse.state==1&&mouse.b==SDL_BUTTON_RIGHT;
	}
	return 0;
}
bool get_release_key(string k) {
	short r=get_scancode(k);
	if(r!=-1)return (!key[r] && prev_key[r]);
	else {
		if(k=="fire1" || k=="fire")
			return mouse.state==3&&mouse.b==SDL_BUTTON_LEFT;
		if(k=="fire2")	return mouse.state==3&&mouse.b==SDL_BUTTON_RIGHT;
	}
	return 0;
}
void bind() {
#define KEY(key) SDL_GetKeyboardState(key)
	getGlobalNamespace(L)
	.addFunction("body",&get_body) //возвращает тело
	.addFunction("joint",&get_joint)	//возвращает соединение
	.addFunction("entity",&get_entity)	//возвращает сущность
	.addFunction("bullet",&get_bullet)
	.addFunction("gettext",&get_text)	//для локализации
	.addFunction("print",&print)
	.addFunction("loadlevel",&level)	//мгновенно грузит уровень
	.addFunction("create_thread",&create_thread)
	.beginNamespace("world")
	.addFunction("who",&whois)	//принимает тело, возвращает сущность
	.addFunction("set_gravity",&set_gravity)	//глобальная гравитация. по умолчанию (0,-9.8)
	.addFunction("create_body",&create_body)	//создать тело по шаблону
	.addFunction("create_entity",&create_entity)	//создать существо по типу
	.addFunction("destroy_body",&destroy_body)
	.addFunction("destroy_entity",&destroy_entity)
	.addFunction("destroy_joint",&destroy_joint)
	.addFunction("create_bullet",&simple_bullet)	//создает тело, придает ему скорость и направление
	.addFunction("explosion",&explosion)	//просто взрыв без повреждений
	.addFunction("explosion_callbacks",&explosion_callbacks)	//взрыв с колбэками.
	.addFunction("raycast_callbacks",&raycast_callbacks)	//трассировка лучей на расстояние с колбэками.
	.addFunction("bb_collide",&bb_collide)		//различные варианты столкновений.
	.addFunction("eb_collide",&eb_collide)		//e-сущность
	.addFunction("ee_collide",&ee_collide)		//b-тело
	.addFunction("lb_collide",&lb_collide)		//l-все тела на уровне
	.addFunction("le_collide",&le_collide)		//s-все существа на уровне
	.addFunction("sb_collide",&sb_collide)		//all-реагирует на сенсоры
	.addFunction("bb_all_collide",&bb_all_collide)
	.addFunction("eb_all_collide",&eb_all_collide)
	.addFunction("ee_all_collide",&ee_all_collide)
	.addFunction("lb_all_collide",&lb_all_collide)
	.addFunction("le_all_collide",&le_all_collide)
	.addFunction("sb_all_collide",&sb_all_collide)
	.addProperty("time_scale",&time_scale)		//скорость игры. по умолчанию 1
	.addProperty("position_iterations",&position_iterations)	//точность физики
	.addProperty("velocity_iterations",&velocity_iterations)	//точность физики
	.endNamespace()
	.beginNamespace("game")
	.beginNamespace("camera")
	.addProperty("x",&cx)
	.addProperty("y",&cy)
	.addProperty("zoom",&zoom)
	.addFunction("center",&center)	//центрирование камеры на координаты уровня
	.addFunction("center_body",&center_body)	//центрирование камеры на тело
	.addProperty("locked",&camera_locked)	//блокировка камеры. если заблокирована то двигается на главным героем и двигать нельзя.
	.addProperty("angle",&mouse_angle)	//угол поворота мыши относительно центра экрана
	.endNamespace()
	.addProperty("time",&get_time)		//количество миллисекунд со старта уровня
	.addFunction("key",&get_key)		//нажата ли клавиша
	.addFunction("press_key",&get_press_key)	//когда клавишу только нажимают
	.addFunction("release_key",&get_release_key)	//когда клавишу отпускают
	.addFunction("interval",&get_interval)	//интервал для какого то действия. Возвращает 1 если время прошло.
	.addProperty("bodies",&bodies)	//типо все тела на уровне возвращает
	.endNamespace()
	.beginNamespace("graphics")
	.beginClass<Color>("Color")
	.addConstructor<void(*)(uint8_t,uint8_t,uint8_t,uint8_t)>()
	.addConstructor<void(*)(uint8_t,uint8_t,uint8_t)>()
	.addProperty("r",&Color::r)
	.addProperty("g",&Color::g)
	.addProperty("b",&Color::b)
	.addProperty("a",&Color::a)
	.addFunction("set",&Color::set)
	.endClass()
	.addFunction("preload",&load_texture)	//загрузка текстуры. Позволяет избежать фризов в игре, если все загрузить сразу
	.addFunction("texture",&find_texture)	//текстура по ID
	.addProperty("show_textures",&show_textures)	//показывать ли текстуры. не знаю для чего это зожет пригодиться
	.addFunction("effect",&effect::create)	//создание граффического эффекта в заданных координатах.
	.addFunction("set_texture_shader",&set_texture_shader)
	.beginNamespace("display")
	.addProperty("w",&SW,0)
	.addProperty("h",&SH,0)
	.endNamespace()
	.endNamespace()
	.beginNamespace("music")
	.addFunction("play",&play_music)
	.addFunction("stop",&stop_music)
	.addFunction("resume",&resume_music)
	.addFunction("pause",&pause_music)
	.endNamespace()
	.beginNamespace("sound")
	.addFunction("play",&play_sound)
	.addFunction("play_without_save",&play_ws_sound)
	.addFunction("play_distance",&play_distance_sound)
	.endNamespace()
	.beginClass<b2Joint>("Joint")
	.addProperty("a",&b2Joint::m_bodyA,0)
	.addProperty("b",&b2Joint::m_bodyB,0)
	.addProperty("motor",&b2Joint::IsMotorEnabled,&b2Joint::EnableMotor)
	.addProperty("motor_speed",&b2Joint::GetMotorSpeed,&b2Joint::SetMotorSpeed)
	.addProperty("max_torque",&b2Joint::GetMaxMotorTorque,&b2Joint::SetMaxMotorTorque)
	.addProperty("limit",&b2Joint::IsLimitEnabled,&b2Joint::EnableLimit)
	.addProperty("lower_limit",&b2Joint::GetLowerLimit,&b2Joint::SetLowerLimit)
	.addProperty("upper_limit",&b2Joint::GetUpperLimit,&b2Joint::SetUpperLimit)
	.addProperty("angle",&b2Joint::GetJointAngle)
	.addProperty("reference_angle",&b2Joint::GetReferenceAngle)
	.addProperty("damping",&b2Joint::GetDamping,&b2Joint::SetDamping)
	.addProperty("stifness",&b2Joint::GetStiffness,&b2Joint::SetStiffness)
	.addProperty("max_force",&b2Joint::GetMaxMotorForce,&b2Joint::SetMaxMotorForce)
	.addProperty("ratio",&b2Joint::GetRatio,&b2Joint::SetRatio)
	.addFunction("force_x",&b2Joint::GetReactionForceX)
	.addFunction("force_y",&b2Joint::GetReactionForceY)
	.addFunction("torque",&b2Joint::GetReactionTorque)
	.addProperty("length",&b2Joint::GetCurrentLength)
	.addProperty("length_a",&b2Joint::GetCurrentLengthA)
	.addProperty("length_b",&b2Joint::GetCurrentLengthB)
	.addProperty("min_length",&b2Joint::GetMinLength,&b2Joint::SetMinLength)
	.addProperty("max_length",&b2Joint::GetMaxLength,&b2Joint::SetMaxLength)
	.addProperty("id",&b2Joint::GetID)
	.endClass()
	.beginClass<b2Body>("Body")
	.addProperty("x",&b2Body::GetX,&b2Body::SetX)
	.addProperty("y",&b2Body::GetY,&b2Body::SetY)
	.addProperty("angle",&b2Body::GetAngle,&b2Body::SetAngle)
	.addProperty("vangle",&b2Body::GetAngularVelocity,&b2Body::SetAngularVelocity)
	.addProperty("vx",&b2Body::GetLinearVelocityX,&b2Body::SetLinearVelocityX)
	.addProperty("vy",&b2Body::GetLinearVelocityY,&b2Body::SetLinearVelocityY)
	.addProperty("v",&b2Body::GetLinearVelocityL)
	.addProperty("gravity_scale",&b2Body::GetGravityScale, &b2Body::SetGravityScale)
	.addProperty("mass",&b2Body::GetMass)
	.addProperty("inertia",&b2Body::GetInertia)
	.addProperty("enabled",&b2Body::IsEnabled,&b2Body::SetEnabled)
	.addProperty("fixed_angle",&b2Body::IsFixedRotation,&b2Body::SetFixedRotation)
	.addProperty("awake",&b2Body::IsAwake,&b2Body::SetAwake)
	.addProperty("id",&b2Body::GetID)
	.addProperty("bullet",&b2Body::IsBullet,&b2Body::SetBullet)
	.addFunction("apply_force",&b2Body::Force)
	.addFunction("apply_center_force",&b2Body::CenterForce)
	.addFunction("apply_torque",&b2Body::Torque)
	.addFunction("apply_impulse",&b2Body::Impulse)
	.addFunction("apply_center_impulse",&b2Body::CenterImpulse)
	.addFunction("apply_angular_impulse",&b2Body::AngularImpulse)
	.addFunction("set_texture",&b2Body::SetTexture)
	.addProperty("userdata",&b2Body::GetLuaUserData,&b2Body::SetLuaUserData)
	.endClass()
	.beginClass<Entity>("Entity")
	.addProperty("x",&Entity::getx,&Entity::setx)
	.addProperty("y",&Entity::gety,&Entity::sety)
	.addProperty("weapon",&Entity::weapon)
	.addFunction("set_weapon",&Entity::set_weapon)
	.addProperty("health",&Entity::health)
	.addProperty("max_health",&Entity::max_health)
	.addProperty("id",&Entity::id,0)
	.addProperty("vx",&Entity::get_vx)
	.addProperty("vy",&Entity::get_vy)
	.addProperty("userdata",&Entity::lua_userdata)
	.addFunction("body",&Entity::get_body)
	.addFunction("joint",&Entity::get_joint)
	.addFunction("destroy_body",&Entity::destroy_body)
	.addFunction("destroy_joint",&Entity::destroy_joint)
	.addFunction("fire1",&Entity::fire1)
	.addFunction("fire2",&Entity::fire2)
	.addFunction("fire3",&Entity::fire3)
	.addFunction("fire4",&Entity::fire4)
	.addFunction("harm",&Entity::harm)
	.addFunction("heal",&Entity::heal)
	.addFunction("focus",&Entity::focus)
	.addFunction("focus_on_entity",&Entity::focus_on_entity)
	.addFunction("focus_on_body",&Entity::focus_on_body)
	.endClass()
	.beginClass<Bullet>("Bullet")
	.addProperty("count",&Bullet::count)
	.addProperty("max",&Bullet::max)
	.addFunction("add",&Bullet::add)
	.addFunction("del",&Bullet::del)
	.addFunction("full",&Bullet::full)
	.endClass()
	.beginClass<GPU_Image>("Texture")
	.addProperty("w",&GPU_Image::w,0)
	.addProperty("h",&GPU_Image::h,0)
	.endClass()
	.beginClass<Weapon>("Weapon")
	.addProperty("dx",&Weapon::dx)
	.addProperty("dy",&Weapon::dy)
	.addProperty("bullet1",&Weapon::bullet1)
	.addProperty("bullet2",&Weapon::bullet2)
	.addProperty("name",&Weapon::name)
	.addProperty("texture",&Weapon::get_texture,&Weapon::set_texture)
	.addProperty("point_x",&Weapon::point_x)
	.addProperty("point_y",&Weapon::point_y)
	.addProperty("angle",&Weapon::angle)
	.endClass()
	.beginClass<GLSLtype>("GLSLtype")	//базовый тип GLSL
	.addFunction("update",&GLSLtype::update)
	.addProperty("location",&GLSLtype::loc,0)
	.endClass()
	.deriveClass<GLSLfloat,GLSLtype>("GLSLfloat")
	.addFunction("set",&GLSLfloat::set)
	.addProperty("value",&GLSLfloat::value)
	.endClass()
	.deriveClass<GLSLint,GLSLtype>("GLSLint")
	.addFunction("set",&GLSLint::set)
	.addProperty("value",&GLSLint::value)
	.endClass()
	.deriveClass<GLSLuint,GLSLtype>("GLSLuint")
	.addFunction("set",&GLSLuint::set)
	.addProperty("value",&GLSLuint::value)
	.endClass()
	.deriveClass<GLSLtex,GLSLtype>("GLSLtex")	//текстура
	.addFunction("set",&GLSLtex::set)
	.addProperty("value",&GLSLtex::tex)
	.endClass()
	.deriveClass<GLSLvec2,GLSLtype>("GLSLvec2")	//двумерный вектор
	.addFunction("set",&GLSLvec2::set)
	.addProperty("x",&GLSLvec2::x)
	.addProperty("y",&GLSLvec2::y)
	.addProperty("r",&GLSLvec2::x)
	.addProperty("g",&GLSLvec2::y)
	.endClass()
	.deriveClass<GLSLvec3,GLSLtype>("GLSLvec3")	//трехмерный вектор
	.addFunction("set",&GLSLvec3::set)
	.addProperty("x",&GLSLvec3::x)
	.addProperty("y",&GLSLvec3::y)
	.addProperty("z",&GLSLvec3::z)
	.addProperty("r",&GLSLvec3::x)
	.addProperty("g",&GLSLvec3::y)
	.addProperty("b",&GLSLvec3::z)
	.endClass()
	.deriveClass<GLSLvec4,GLSLtype>("GLSLvec4")	//четырехмерный вектор
	.addFunction("set",&GLSLvec4::set)
	.addProperty("x",&GLSLvec4::x)
	.addProperty("y",&GLSLvec4::y)
	.addProperty("z",&GLSLvec4::z)
	.addProperty("w",&GLSLvec4::w)
	.addProperty("r",&GLSLvec4::x)
	.addProperty("g",&GLSLvec4::y)
	.addProperty("b",&GLSLvec4::z)
	.addProperty("a",&GLSLvec4::w)
	.endClass()
	.beginClass<Shader>("Shader")
	.addConstructor <void (*) (string,string)> ()
	.addFunction("add_tex",&Shader::add_tex)
	.addFunction("add_float",&Shader::add_float)
	.addFunction("add_int",&Shader::add_int)
	.addFunction("add_uint",&Shader::add_uint)
	.addFunction("add_vec2",&Shader::add_vec2)
	.addFunction("add_vec3",&Shader::add_vec3)
	.addFunction("add_vec4",&Shader::add_vec4)
	.addFunction("add_mat2",&Shader::add_mat2)
	.addFunction("add_mat3",&Shader::add_mat3)
	.addFunction("add_mat4",&Shader::add_mat4)
	.endClass();
}
void init(string name) { //
	copy_prev_key();
	L = luaL_newstate();
	luaL_openlibs(L);
	bind();
	create_userdata();
	dostring(
		"player=entity('player')\n"
		"Level={}\n"
		"Level.init=function() end\n"
		"Level.update=function() end\n"

		"level={}\n"
		"level.init=function() end\n"
		"level.update=function() end\n"

		"Body={}\n"
		"Body.init=function() end\n"
		"Body.update=function() end\n"
		"Body.collide = function() end\n"

		"Weapon={}\n"
		"Weapon.init=function() end\n"
		"Weapon.update=function() end\n"
		"Weapon.fire1=function() return 0 end\n"
		"Weapon.fire2=function() return 0 end\n"
		"Weapon.fire3=function() return 0 end\n"
		"Weapon.fire4=function() return 0 end\n"

		"Entity={}\n"
		"Entity.init=function() end\n"
		"Entity.update=function() end\n"
		"Entity.collide_body = function() end\n"
		"Entity.collide_entity=function() end\n"

		"function extend(parent)\n"
		"local child = {}\n"
		"setmetatable(child,{__index = parent})\n"
		"return child\n"
		"end\n"
	);
	doscript("common/level");
	doscript("common/entity");
	doscript("common/weapon");
	doscript("levels/"+name);
	loaded.emplace_back("level");
	loaded.emplace_back("Level");
	loaded.emplace_back("Entity");
	loaded.emplace_back("Weapon");
	loaded.emplace_back(name);
	getGlobal(L,"Level")["init"]();
	getGlobal(L,"level")["init"]();
	init_entities();
	init_bodies();
}
void quit() {
	if(L)lua_close(L);
}
void save_luaref(XMLNode n,LuaRef value) { //сохраняет в XMLNode переменную
	if(!is_filled(value))return;
	if(value.isBool()) {
		n.addAttribute("type","boolean");
		n.addAttribute("value",value.cast<bool>());
	} else if(value.isNumber()) {
		n.addAttribute("type","number");
		n.addAttribute("value",value.cast<float>());
	} else if(value.isNil()) {
		n.addAttribute("type","nil");
	} else if(value.isString()) {
		n.addAttribute("type","string");
		n.addAttribute("value",value.cast<string>());
	} else if(value.isTable()) {
		n.addAttribute("type","table");
		int count=0;
		value.push(L);
		push(L,Nil());
		while(lua_next(L,-2)!=0) {
			LuaRef key=LuaRef::fromStack(L,-2);
			LuaRef val=LuaRef::fromStack(L,-1);
			if((key.isString() || key.isNumber()) && is_filled(val)) {
				XMLNode child=n.addChild("value");
				child.addAttribute("name",key.cast<string>());
				save_luaref(child,val);
				count++;
			}
			lua_pop(L,1);
		}
		n.addAttribute("count",count);
	}
}
LuaRef load_luaref(XMLNode n) { //Загружает переменную из XMLNode
	if(n.getAttribute("type")) {
		string type=n.getAttribute("type");
		if(type=="boolean")
			return LuaRef(L,stoi(n.getAttribute("value"))?true:false);
		else if(type=="number")
			return LuaRef(L,stof(n.getAttribute("value")));
		else if(type=="string")
			return LuaRef(L,n.getAttribute("value"));
		else if(type=="table") {
			LuaRef c(newTable(L));
			int count=stoi(n.getAttribute("count"));
			for(int q=0; q<count; q++) {
				XMLNode child=n.getChildNode("value",q);
				LuaRef ch(L,load_luaref(child));
				string s=child.getAttribute("name");
				c[s]=ch;
			}
			return c;
		}
	}
	return newTable(L);
}
bool is_filled(LuaRef value) {
	if(value.isBool()||value.isNumber()||value.isString())
		return true;
	if(value.isTable()) {
		value.push(L);
		push(L,Nil());
		while(lua_next(L,-2)!=0) {
			LuaRef key=LuaRef::fromStack(L,-2);
			LuaRef val=LuaRef::fromStack(L,-1);
			if((key.isString() || key.isNumber())&&is_filled(val))
				return true;
			lua_pop(L,1);
		}
	}
	return false;
}
};
