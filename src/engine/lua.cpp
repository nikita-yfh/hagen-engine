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
#include <detail/Userdata.h>
using namespace luabridge;
using namespace std;
uint8_t prev_key[SDL_NUM_SCANCODES];
namespace lua {
lua_State *L;
string need_load;
map<unsigned int,unsigned int>timers;
vector<string>loaded;
float game_timer=0;
int prev_timer=0;
bool get_interval(unsigned int ms) {
	if(SDL_GetTicks()-timers[ms]>ms/time_scale) {
		return 1;
	}
	return 0;
}
void update_intervals() {
	for(auto &t : timers) {
		if(SDL_GetTicks()-t.second>t.first/time_scale)
			t.second=SDL_GetTicks();
	}
	while(prev_timer<SDL_GetTicks()+1){
		prev_timer++;
		game_timer+=time_scale;
	}
}
//НАЧАЛО КОСТЫЛЕЙ И ОСНОВНЫХ ПРИЧИН БАГОВ
void set_mask(Color &c) {
	scene_mask=c;
}
void print(LuaRef r) {
	interface.console.out(r.tostring());
	cout<<r<<endl;
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
void init_body(b2Body *body){
	if(B_DATA(body,script).size()){
		bool ok=0;
		for(string l : loaded)
			if(B_DATA(body,script)==l)
				ok=1;
		if(!ok){
			luaL_dostring(L,(B_DATA(body,script)+"=extend(Body)\n").c_str());
			doscript("bodies/"+B_DATA(body,script));
			loaded.emplace_back(B_DATA(body,script));
		}
		getGlobal(L,B_DATA(body,script).c_str())["init"](body);
	}
}
void init_bodies() {
	for(auto body : bodies){
		init_body(body.second);
	}
}

void update_bodies() {
	for(auto body : bodies) {
		if(B_DATA(body.second,script).size()){
			if(getGlobal(L,B_DATA(body.second,script).c_str())["update"](body.second)){
				destroy_body(body.second);
				return;
			}
		}
	}
}
void init_entity(Entity *entity){
	bool ok=0;
	for(string l : loaded)
		if(entity->type==l)
			ok=1;
	if(!ok){
		luaL_dostring(L,(entity->type+"=extend(Entity)\n").c_str());
		doscript("entities/"+entity->type);
		loaded.emplace_back(entity->type);
	}
	getGlobal(L,entity->type.c_str())["init"](entity);
	getGlobal(L,"Entity")["init"](entity);
}
void init_entities() {
	for(auto entity : entities)
		init_entity(entity.second);
}
void init_weapon(string weapon) {
	luaL_dostring(L,(weapon+"=extend(Weapon)\n").c_str());
	doscript("weapon/"+weapon);
	getGlobal(L,"Weapon")["init"](&weapons[weapon]);
	getGlobal(L,weapon.c_str())["init"](&weapons[weapon]);
}
void update_entities() {
	for(auto entity : entities) {
		if(getGlobal(L,"Entity")["update"](entity.second)||
	getGlobal(L,entity.second->type.c_str())["update"](entity.second)){
			destroy_entity(entity.second);
			return;
		}
		if(weapons.find(entity.second->weapon)!=weapons.end()) {
			getGlobal(L,"Weapon")["update"](&weapons[entity.second->weapon],entity.second);
			getGlobal(L,entity.second->weapon.c_str())["update"](&weapons[entity.second->weapon],entity.second);
		}
	}
}

int fire1(Entity *ent) { return getGlobal(L,ent->weapon.c_str())["fire1"](&weapons[ent->weapon],ent); }
int fire2(Entity *ent) { return getGlobal(L,ent->weapon.c_str())["fire2"](&weapons[ent->weapon],ent); }
int fire3(Entity *ent) { return getGlobal(L,ent->weapon.c_str())["fire3"](&weapons[ent->weapon],ent); }
int fire4(Entity *ent) { return getGlobal(L,ent->weapon.c_str())["fire4"](&weapons[ent->weapon],ent); }
void copy_prev_key() {
	memcpy(prev_key,SDL_GetKeyboardState(0),SDL_NUM_SCANCODES);
}
void gameloop() {
	getGlobal(L,"Level")["update"]();
	getGlobal(L,"level")["update"]();
	update_entities();
	update_bodies();
	update_intervals();
	update_effects();
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
void create_body_userdata(b2Body *b){
	b->SetLuaUserData(new LuaRef(newTable(L)));
}
void create_joint_userdata(b2Joint *j){
	j->SetLuaUserData(new LuaRef(newTable(L)));
}
void create_entity_userdata(Entity *e){
	e->lua_userdata=new LuaRef(newTable(L));
	for(auto b : e->bodies)
		create_body_userdata(b.second);
	for(auto j : e->joints)
		create_joint_userdata(j.second);
}
void create_userdata(){
	for(auto b : bodies)
		create_body_userdata(b.second);
	for(auto j : joints)
		create_joint_userdata(j.second);
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
		.addFunction("body",&get_body)
		.addFunction("joint",&get_joint)
		.addFunction("entity",&get_entity)
		.addFunction("weapon",&get_weapon)
		.addFunction("bullet",&get_bullet)
		.addFunction("gettext",&get_text)
		.addFunction("print",&print)
		.addFunction("loadlevel",&level)
		.beginNamespace("world")
			.addFunction("who",&whois)
			.addFunction("set_gravity",&set_gravity)
			.addFunction("create_body",&create_body)
			.addFunction("create_entity",&create_entity)
			.addFunction("destroy_body",&destroy_body)
			.addFunction("destroy_entity",&destroy_entity)
			.addFunction("destroy_joint",&destroy_joint)
			.addFunction("create_bullet",&simple_bullet)
			.addFunction("bb_collide",&bb_collide)
			.addFunction("eb_collide",&eb_collide)
			.addFunction("ee_collide",&ee_collide)
			.addFunction("lb_collide",&lb_collide)
			.addFunction("le_collide",&le_collide)
			.addFunction("sb_collide",&sb_collide)
			.addFunction("bb_all_collide",&bb_all_collide)
			.addFunction("eb_all_collide",&eb_all_collide)
			.addFunction("ee_all_collide",&ee_all_collide)
			.addFunction("lb_all_collide",&lb_all_collide)
			.addFunction("le_all_collide",&le_all_collide)
			.addFunction("sb_all_collide",&sb_all_collide)
			.addProperty("time_scale",&time_scale)
			.addProperty("position_iterations",&position_iterations)
			.addProperty("velocity_iterations",&velocity_iterations)
		.endNamespace()
		.beginNamespace("game")
			.beginNamespace("camera")
				.addProperty("x",&cx)
				.addProperty("y",&cy)
				.addProperty("dx",&dcx)
				.addProperty("dy",&dcy)
				.addProperty("zoom",&zoom)
				.addFunction("center",&center)
				.addFunction("center_body",&center_body)
				.addProperty("locked",&camera_locked)
				.addProperty("angle",&mouse_angle)
			.endNamespace()
			.addProperty("timer",&game_timer,0)
			.addFunction("key",&get_key)
			.addFunction("press_key",&get_press_key)
			.addFunction("release_key",&get_release_key)
			.addFunction("interval",&get_interval)
			.addProperty("bodies",&bodies)
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
			.addFunction("preload",&load_texture)
			.addFunction("texture",&find_texture)
			.addProperty("show_textures",&show_textures)
			.addFunction("effect",&create_effect)
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
			.addProperty("userdata",&b2Joint::GetLuaUserData,&b2Joint::SetLuaUserData)
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
			.addProperty("weapon",&Entity::get_weapon,&Entity::set_weapon)
			.addProperty("weapon_x",&Entity::weapon_x,0)
			.addProperty("weapon_y",&Entity::weapon_y,0)
			.addProperty("health",&Entity::health)
			.addProperty("id",&Entity::id,0)
			.addProperty("vx",&Entity::get_vx)
			.addProperty("vy",&Entity::get_vy)
			.addProperty("userdata",&Entity::lua_userdata)
			.addProperty("weapon_angle",&Entity::weapon_angle)
			.addFunction("body",&Entity::get_body)
			.addFunction("joint",&Entity::get_joint)
			.addFunction("destroy_body",&Entity::destroy_body)
			.addFunction("destroy_joint",&Entity::destroy_joint)
			.addFunction("fire1",&Entity::fire1)
			.addFunction("fire2",&Entity::fire2)
			.addFunction("fire3",&Entity::fire3)
			.addFunction("fire4",&Entity::fire4)
			.addFunction("harm",&Entity::harm)
			.addFunction("focus",&Entity::focus)
			.addFunction("focus_on_entity",&Entity::focus_on_entity)
			.addFunction("focus_on_body",&Entity::focus_on_body)
		.endClass()
		.beginClass<Bullet>("Bullet")
			.addProperty("count",&Bullet::count)
			.addProperty("max",&Bullet::max)
			.addFunction("add",&Bullet::add)
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
		.endClass();
}
void init(string name) {
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
	getGlobal(L,"Level")["init"]();
	getGlobal(L,"level")["init"]();
	init_entities();
	init_bodies();
}
void quit() {
	if(L)lua_close(L);
}
int get_time(){
	return game_timer;
}
};
