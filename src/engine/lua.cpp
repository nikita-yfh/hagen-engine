#include "lua.hpp"
#include "lua_draw.hpp"
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
#include <Map.h>
using namespace luabridge;
using namespace std;
namespace lua {
lua_State *L=nullptr;
string need_load;
map<unsigned int,unsigned int>timers;
vector<string>loaded;
float game_time=0;
int prev_time=0;
float time_scale=1.0f;
bool get_interval(unsigned int ms) {
	if(get_time()-timers[ms]>ms/time_scale) {
		return 1;
	}
	return 0;
}
static void update_intervals() {
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

#if defined(ANDROID)
static const char* os="Android";
#elif defined(__linux__)
static const char* os="Linux";
#elif defined(_WIN32)
static const char* os="Windows";
#elif defined(__APPLE__)
static const char* os="MacOS";
#elif defined(__unix__)
static const char* os="Unix";
#else
static const char* os="Unknown";
#endif

vector<float> luaval_to_vector_float(lua_State *L,int n) {
	vector<float>vec;
	int len = lua_objlen(L,n);
	for (int i=1; i <= len; i++) {
		lua_pushinteger(L,i);
		lua_gettable(L,n);
		float v= lua_tonumber(L,-1);
		if (v)
			vec.push_back(v);
		lua_pop(L,1);
	}
	return vec;
}
static float get_ddpi(){
	float f;
	if(SDL_GetDisplayDPI(0,&f,0,0))
		panic(SDL_GetError());
	return f;
}
static float get_vdpi(){
	float f;
	if(SDL_GetDisplayDPI(0,0,0,&f))
		panic(SDL_GetError());
	return f;
}
static float get_hdpi(){
	float f;
	if(SDL_GetDisplayDPI(0,0,&f,0))
		panic(SDL_GetError());
	return f;
}
static float get_dpi(){
	float f1,f2;
	if(SDL_GetDisplayDPI(0,0,&f1,&f2))
		panic(SDL_GetError());
	return (f1+f2)/2.0f;
}
static float get_refresh_rate(){
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0,&mode);
	return mode.refresh_rate;
}
//НАЧАЛО КОСТЫЛЕЙ И ОСНОВНЫХ ПРИЧИН БАГОВ
static void print(LuaRef r) {
	info_log(r.tostring());
}
void clear_loaded_list() {
	loaded.clear();
}
static void dofile(string file) {
	string s=RWget(file.c_str());
	if(luaL_dostring(L,s.c_str())) {
		panic(string(lua_tostring(L, -1)));
	}else if(s.size())
		info_log("Loaded script "+file);
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
		panic(lua_tostring(L, -1));
	}
}
static void doscript(string file) {
	dofile(prefix+file+".lua");
}
static void level(string str) {
	need_load=str;
}
static void load(string str) {
	need_load="[LOAD]"+str;
}
static int parse_string(vector<LuaRef>&vals,string &exec){
	trim(exec);
	string args;
	if(exec.find("(")!=string::npos&&exec.find(")")!=string::npos){
		args.assign(exec,exec.find("(")+1,exec.find(")")-exec.find("(")-1);
		exec.assign(exec,0,exec.find("("));
		trim(exec);
	}else return 0;
	string buf;
	auto done=[&](string &buf,bool s){
		if(s) vals.push_back(LuaRef(L,buf));
		else{
			if(is_valid_float(buf))
				vals.push_back(LuaRef(L,stof(buf)));
			else if(buf=="nil")
				vals.push_back(LuaRef(L));
			else if(buf=="false")
				vals.push_back(LuaRef(L,false));
			else if(buf=="true")
				vals.push_back(LuaRef(L,false));
			else return 1;
		}
		buf.clear();
		return 0;
	};
	uint8_t mode=1;
	for(char c : args){
		if(c=='"'||c=='\''){
			if(mode==2)mode=3;
			else if(mode==4){
				if(done(buf,1))
					return 1;
				mode=3;
			}else if(mode==1)mode=2;
			else return 1;
		}else if(mode==2 || mode==4){
			mode=4;
			buf.push_back(c);
		}else if(mode==3) mode=1;
		else if(mode==0 || mode==1){
			if(c==' ' || c==','){
				if(mode==0 && done(buf,0))
					return 1;
				mode=1;
			}else{
				buf.push_back(c);
				mode=0;
			}
		}else return 1;
	}
	if(mode==0 && done(buf,0))
		return 1;
	else if(mode!=3)return 1;
	return 0;
}
static bool unique(string &exec){
	for(string l : loaded)
		if(exec==l)
			return 0;
	return 1;
}
void init_body(b2Body *body,bool ex) {
	string &exec=B_DATA(body,script);
	if(!exec.size()) return;
	vector<LuaRef>lua_args;
	parse_string(lua_args,exec);
	if(unique(exec)) {
		dostring(exec+"=extend(Body)\n");
		doscript("bodies/"+exec);
	}
	if(ex){
		auto exec_func=[&](){
			Stack<b2Body*>::push(L, body);
			for(LuaRef val : lua_args)
				Stack<LuaRef>::push(L, val);
			LuaException::pcall(L, lua_args.size()+1, 1);
			LuaRef::fromStack(L);
		};
		getGlobal(L,"Body")["init"].push(L);
		exec_func();
		getGlobal(L,exec.c_str())["init"].push(L);
		exec_func();
	}
}
static void init_bodies() {
	for(auto body : bodies) {
		init_body(body.second);
	}
}

static void update_bodies() {
	for(auto body : bodies) {
		if(B_DATA(body.second,script).size()) {
			if(getGlobal(L,"Body")["update"](body.second)||
					getGlobal(L,B_DATA(body.second,script).c_str())["update"](body.second)) {
				destroy_body(body.second);
				return;
			}
		}
	}
}
void init_entity(Entity *entity,bool ex) {
	string &exec=entity->type;
	if(!exec.size())return;
	vector<LuaRef>lua_args;
	parse_string(lua_args,exec);
	if(unique(exec)) {
		luaL_dostring(L,(entity->type+"=extend(Entity)\n").c_str());
		doscript("entities/"+entity->type);
	}
	if(ex) {
		auto exec_func=[&](){
			Stack<Entity*>::push(L, entity);
			for(LuaRef val : lua_args)
				Stack<LuaRef>::push(L, val);
			LuaException::pcall(L, lua_args.size()+1, 1);
			LuaRef::fromStack(L);
		};
		getGlobal(L,"Entity")["init"].push(L);
		exec_func();
		getGlobal(L,exec.c_str())["init"].push(L);
		exec_func();
	}
}
static void init_entities() {
	for(auto entity : entities)
		init_entity(entity.second);
}
void init_weapon(Entity *entity,bool ex) {
	string &exec=entity->weapon.name;
	if(!exec.size())return;
	vector<LuaRef>lua_args;
	parse_string(lua_args,exec);
	if(unique(exec)) {
		luaL_dostring(L,(exec+"=extend(Weapon)").c_str());
		doscript("weapon/"+exec);
	}
	if(ex) {
		auto exec_func=[&](){
			Stack<Weapon*>::push(L, &entity->weapon);
			Stack<Entity*>::push(L, entity);
			for(LuaRef val : lua_args)
				Stack<LuaRef>::push(L, val);
			LuaException::pcall(L, lua_args.size()+2, 1);
			LuaRef::fromStack(L);
		};
		getGlobal(L,"Weapon")["init"].push(L);
		exec_func();
		getGlobal(L,exec.c_str())["init"].push(L);
		exec_func();
	}
}
static void update_entities() {
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
void gameloop() {
	text::update();
	getGlobal(L,"Level")["update"]();
	getGlobal(L,"level")["update"]();
	update_entities();
	update_bodies();
	update_intervals();
	effect::update();
}
void create_body_userdata(b2Body *b) {
	b->SetLuaUserData(new LuaRef(newTable(L)));
}
void create_entity_userdata(Entity *e) {
	e->lua_userdata=new LuaRef(newTable(L));
	for(auto b : e->bodies)
		create_body_userdata(b.second);
}
static void create_userdata() {
	for(auto b : bodies)
		create_body_userdata(b.second);
	for(auto e : entities)
		create_entity_userdata(e.second);
}
static bool get_key(string k) {
	if(interface.mainmenu.shown)return 0;
	short r=get_scancode(k);
	if(r!=-1)return key(r);
	else {
		if(k=="fire1" || k=="fire")
			return mouse.state==2&&mouse.b==SDL_BUTTON_LEFT;
		if(k=="fire2")	return mouse.state==2&&mouse.b==SDL_BUTTON_RIGHT;
	}
	return 0;
}
static bool get_press_key(string k) {
	if(interface.mainmenu.shown)return 0;
	short r=get_scancode(k);
	if(r!=-1)return (key(r) && !prev_key[r]);
	else {
		if(k=="fire1" || k=="fire")
			return mouse.state==1&&mouse.b==SDL_BUTTON_LEFT;
		if(k=="fire2")	return mouse.state==1&&mouse.b==SDL_BUTTON_RIGHT;
	}
	return 0;
}
static bool get_release_key(string k) {
	if(interface.mainmenu.shown)return 0;
	short r=get_scancode(k);
	if(r!=-1)return (!key(r) && prev_key[r]);
	else {
		if(k=="fire1" || k=="fire")
			return mouse.state==3&&mouse.b==SDL_BUTTON_LEFT;
		if(k=="fire2")	return mouse.state==3&&mouse.b==SDL_BUTTON_RIGHT;
	}
	return 0;
}
static int add_loaded(lua_State *L){
	assert(lua_gettop(L)==1);
	LuaRef value=LuaRef::fromStack(L,1);
	if(value.isNumber()||value.isString()){
		string s=value.tostring();
		for(string l : loaded)
			if(l==s)
				return 0;
		loaded.emplace_back(s);
	}
	return 0;
}

struct RayCastCallback : public b2RayCastCallback {
	LuaRef *callback;
	float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override {
		assert(callback->isFunction());
		return (*callback)(fixture,point.x,point.y,normal.x,normal.y,fraction);
	}
	RayCastCallback(LuaRef value){
		callback=new LuaRef(value);
	}
	~RayCastCallback(){
		delete callback;
	}
};
void raycast(float x1,float y1,float x2,float y2,LuaRef callback) {
	RayCastCallback c(callback);
	world->RayCast(&c, {x1,y1},{x2,y2});
}
static void bind() {
#define KEY(key) SDL_GetKeyboardState(key)
	getGlobalNamespace(L)
	.beginNamespace("system")
	.addFunction("exit",quit)
	.addFunction("__add_loaded",add_loaded)
	.addFunction("restart",restart)
	.addProperty("os",&os,0)
	.endNamespace()
	.beginClass<Color>("Color")
	.addConstructor<void(*)(uint8_t,uint8_t,uint8_t,uint8_t)>()
	.addProperty("r",&Color::r)
	.addProperty("g",&Color::g)
	.addProperty("b",&Color::b)
	.addProperty("a",&Color::a)
	.addFunction("set",&Color::set)
	.endClass()
	.addFunction("body",&get_body) //возвращает тело
	.addFunction("joint",&get_joint)	//возвращает соединение
	.addFunction("entity",&get_entity)	//возвращает сущность
	.addFunction("bullet",&get_bullet)
	.addFunction("print",&print)
	.addFunction("loadlevel",&level)	//мгновенно грузит уровень
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
	.addFunction("raycast",&raycast)
	.endNamespace()
	.beginNamespace("game")
	.beginNamespace("camera")
	.addProperty("x",&cx)
	.addProperty("y",&cy)
	.addProperty("ix",&ix)
	.addProperty("iy",&iy)
	.addProperty("zoom",&zoom)
	.addFunction("center",&center)	//центрирование камеры на координаты уровня
	.addFunction("center_body",&center_body)	//центрирование камеры на тело
	.addFunction("center_input",&center_input)
	.addFunction("center_input_body",&center_input_body)
	.addProperty("angle",&mouse_angle)	//угол поворота мыши относительно центра экрана
	.endNamespace()
	.addProperty("time",&get_time)		//количество миллисекунд со старта уровня
	.addFunction("key",&get_key)		//нажата ли клавиша
	.addFunction("press_key",&get_press_key)	//когда клавишу только нажимают
	.addFunction("release_key",&get_release_key)	//когда клавишу отпускают
	.addFunction("interval",&get_interval)	//интервал для какого то действия. Возвращает 1 если время прошло.
	.addProperty("bodies",&bodies)	//типа все тела на уровне возвращает
	.addFunction("save",&save_world_state)
	.addFunction("load",&load)
	.addProperty("interface",&interface.game_interface.shown)
	.endNamespace()
	.beginNamespace("text")
	.addFunction("get",&text::get)
	.addFunction("add_tip",&text::add_tip)
	.addFunction("add_subtitles",&text::add_subtitles)
	.addFunction("defcolor",&text::defcolor)
	.endNamespace()
	.beginNamespace("mouse")
	.addProperty("x",&mouse.x,0)
	.addProperty("y",&mouse.y,0)
	.addProperty("button",&mouse.b,0)
	.addProperty("state",&mouse.state,0)
	.endNamespace()
	.beginNamespace("graphics")
	.addFunction("drawx",&drawx)
	.addFunction("drawy",&drawy)
	.addFunction("preload",&load_texture)	//загрузка текстуры. Позволяет избежать фризов в игре, если все загрузить сразу
	.addFunction("texture",&find_texture)	//текстура по ID
	.addFunction("effect",&effect::create)	//создание граффического эффекта в заданных координатах.
	.addFunction("set_texture_shader",&set_texture_shader)
	.addFunction("unset_texture_shader",&unset_texture_shader)
	.addFunction("get_shader",&get_shader)
	.addProperty("texture_scale",&tex_scale)
	.addProperty("weapon_scale",&weapon_scale)
	.addProperty("effect_scale",&effect_scale)
	.beginNamespace("display")
	.addProperty("w",&SW,0)
	.addProperty("h",&SH,0)
	.addProperty("dpi",&get_dpi)
	.addProperty("ddpi",&get_ddpi)
	.addProperty("vdpi",&get_vdpi)
	.addProperty("hdpi",&get_hdpi)
	.addProperty("refresh_rate",&get_refresh_rate)
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
	.beginClass<b2Fixture>("Fixture")
	.addProperty("body",&b2Fixture::m_body,0)
	.addProperty("friction",&b2Fixture::GetFriction,&b2Fixture::SetFriction)
	.addProperty("density",&b2Fixture::GetDensity,&b2Fixture::SetDensity)
	.addProperty("restitution",&b2Fixture::GetRestitution,&b2Fixture::SetRestitution)
	.addProperty("mode",&b2Fixture::IsSensor,&b2Fixture::SetSensor)
	.addProperty("id",&b2Fixture::GetID,&b2Fixture::SetID)
	.addProperty("texture",&b2Fixture::GetTexture,&b2Fixture::SetTexture)
	.addProperty("angle",&b2Fixture::GetAngle,&b2Fixture::SetAngle)
	.addProperty("expand",&b2Fixture::IsExpand,&b2Fixture::SetExpand)
	.addProperty("layer",&b2Fixture::GetLayer,&b2Fixture::SetLayer)
	.endClass()
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
	.beginNamespace("body_type")
	.addConstant("static",(int)b2_staticBody)
	.addConstant("kinematic",(int)b2_kinematicBody)
	.addConstant("dynamic",(int)b2_dynamicBody)
	.endNamespace()
	.beginClass<b2Body>("Body")
	.addProperty("x",&b2Body::GetX,&b2Body::SetX)
	.addProperty("y",&b2Body::GetY,&b2Body::SetY)
	.addProperty("angle",&b2Body::GetAngle,&b2Body::SetAngle)
	.addProperty("awake",&b2Body::IsAwake,&b2Body::SetAwake)
	.addProperty("enabled",&b2Body::IsEnabled,&b2Body::SetEnabled)
	.addProperty("bullet",&b2Body::IsBullet,&b2Body::SetBullet)
	.addProperty("vangle",&b2Body::GetAngularVelocity,&b2Body::SetAngularVelocity)
	.addProperty("vx",&b2Body::GetLinearVelocityX,&b2Body::SetLinearVelocityX)
	.addProperty("vy",&b2Body::GetLinearVelocityY,&b2Body::SetLinearVelocityY)
	.addProperty("v",&b2Body::GetLinearVelocityL)
	.addProperty("gravity_scale",&b2Body::GetGravityScale, &b2Body::SetGravityScale)
	.addProperty("mass",&b2Body::GetMass)
	.addProperty("inertia",&b2Body::GetInertia)
	.addProperty("type",&b2Body::GetType,&b2Body::SetType)
	.addProperty("enabled",&b2Body::IsEnabled,&b2Body::SetEnabled)
	.addProperty("fixed_angle",&b2Body::IsFixedRotation,&b2Body::SetFixedRotation)
	.addProperty("awake",&b2Body::IsAwake,&b2Body::SetAwake)
	.addProperty("id",&b2Body::GetID)
	.addProperty("bullet",&b2Body::IsBullet,&b2Body::SetBullet)
	.addFunction("advance",&b2Body::Advance)
	.addFunction("reset_mass_data",&b2Body::ResetMassData)
	.addProperty("linear_damping",&b2Body::GetLinearDamping,&b2Body::SetLinearDamping)
	.addProperty("angular_damping",&b2Body::GetAngularDamping,&b2Body::SetAngularDamping)
	.addFunction("apply_force",&b2Body::Force)
	.addFunction("apply_center_force",&b2Body::CenterForce)
	.addFunction("apply_torque",&b2Body::Torque)
	.addFunction("apply_impulse",&b2Body::Impulse)
	.addFunction("apply_center_impulse",&b2Body::CenterImpulse)
	.addFunction("apply_angular_impulse",&b2Body::AngularImpulse)
	.addProperty("userdata",&b2Body::GetLuaUserData,&b2Body::SetLuaUserData)
	.addFunction("fixture",&b2Body::GetFixture)
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
	.addFunction("set_tex",&GLSLtex::set_tex)
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
	bind_imgui();
	bind_graphics();
}
static vector<string>get_scripts_list(){
	vector<string>l;
	XMLNode file=open_xml((prefix+"config/scripts.xml").c_str(),"scripts");
	int count=file.getAttributei("count");
	for(int q=0;q<count;q++){
		XMLNode s=file.getChildNode("script",q);
		string name=s.getAttribute("name");
		if(name.find(".")!=string::npos)
			name.erase(name.begin()+name.find("."),name.end());
		l.emplace_back(name);
	}
	return l;
}
void init() {
	close();
	copy_prev_key();
	mouse.clear();
	prev_time=SDL_GetTicks();
	timers.clear();
	L = luaL_newstate();
	luaL_openlibs(L);
	bind();
	dostring(
		"setmetatable(_G, {\n"
			"__newindex = function(array, index, value)\n"
				"rawset(array, index, value)\n"
				"system.__add_loaded(index)\n"
			"end\n"
		"})\n"
		"Level={}\n"
		"Level.init=function() end\n"
		"Level.update=function() end\n"
		"Level.newgame=function() end\n"

		"level={}\n"
		"level.init=function() end\n"
		"level.update=function() end\n"
		"level.newgame=function()"
		"Level.newgame()\n"
		"end\n"

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
	vector<string>scripts=get_scripts_list();
	for(string name : scripts)
		doscript("scripts/"+name);
}

void init_main_menu() {
	init();
	getGlobal(L,"init_main_menu")();
}

void init_level(string name,bool n) {
	init();
	create_userdata();
	doscript("levels/"+name);
	if(n)
		getGlobal(L,"level")["newgame"]();
	init_entities();
	init_bodies();
	getGlobal(L,"Level")["init"]();
	getGlobal(L,"level")["init"]();
}
void close() {
	if(L)lua_close(L);
	L=nullptr;
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
			lua_pop(L,1);
			if((key.isString() || key.isNumber()) && is_filled(val)) {
				XMLNode child=n.addChild("value");
				child.addAttribute("name",key.cast<string>());
				save_luaref(child,val);
				count++;
			}
		}
		n.addAttribute("count",count);
		lua_pop(L, 1);
	}
}
LuaRef load_luaref(XMLNode n) { //Загружает переменную из XMLNode
	if(n.getAttribute("type")) {
		string type=n.getAttribute("type");
		if(type=="boolean")
			return LuaRef(L,n.getAttributei("value")?true:false);
		else if(type=="number")
			return LuaRef(L,n.getAttributef("value"));
		else if(type=="string")
			return LuaRef(L,n.getAttribute("value"));
		else if(type=="table") {
			LuaRef c(newTable(L));
			int count=n.getAttributei("count");
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
	bool ok=0;
	if(value.isBool()||value.isNumber()||value.isString())
		return true;
	if(value.isTable()) {
		value.push(L);
		push(L,Nil());
		while(lua_next(L,-2)!=0) {
			LuaRef key=LuaRef::fromStack(L,-2);
			LuaRef val=LuaRef::fromStack(L,-1);
			if((key.isString() || key.isNumber())&&is_filled(val))
				ok=1;
			lua_pop(L,1);
		}
		lua_pop(L,1);
	}
	return ok;
}
};
