#include "lua.hpp"
#include "physic.hpp"
#include "render.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "level.hpp"
#include "sdl.hpp"
#include "utility.hpp"
#include "text.hpp"
#include "main.hpp"
#include "console.hpp"
#include <player.hpp>
#include <detail/Userdata.h>
using namespace luabridge;
using namespace std;
namespace lua{
lua_State *L;
map<unsigned int,unsigned int>timers;
bool get_interval(unsigned int ms){
	if(SDL_GetTicks()-timers[ms]>ms){
		return 1;
	}
	return 0;
}
void update_intervals(){
	for(auto &t : timers){
		if(SDL_GetTicks()-t.second>t.first)
			t.second=SDL_GetTicks();
	}
}
//НАЧАЛО КОСТЫЛЕЙ И ОСНОВНЫХ ПРИЧИН БАГОВ
void set_mask(Color &c) {
	scene_mask=c;
}
void print(LuaRef r){
	console.out(r.tostring());
}
Color &get_mask() {
	return scene_mask;
}
void dofile(string file){
	if(!exist_file(file))return;
	if(luaL_dofile(L, file.c_str())){
		throw string(lua_tostring(L, -1));
	}
}

void dostring(string text){
	if(luaL_dostring(L, text.c_str())){
		throw string(lua_tostring(L, -1));
	}
}
void doscript(string file){
	dofile(prefix+file+".lua");
}
void init_entities(){
	for(auto entity : entities){
		luaL_dostring(L,(entity.second->type+"=extend(Entity)\n").c_str());
		doscript("entities/"+entity.second->type);
		getGlobal(L,entity.second->type.c_str())["init"](entity.second);
	}
}
void init_weapon(string weapon){
	luaL_dostring(L,(weapon+"=extend(Weapon)\n").c_str());
	doscript("weapon/"+weapon);
	getGlobal(L,weapon.c_str())["init"](&weapons[weapon]);
}
void update_entities(){
	for(auto entity : entities){
		getGlobal(L,entity.second->type.c_str())["update"](entity.second);
		if(weapons.find(entity.second->weapon)!=weapons.end())
		getGlobal(L,entity.second->weapon.c_str())["update"](&weapons[entity.second->weapon],entity.second);
	}
}

int fire1(Entity *ent){return getGlobal(L,ent->weapon.c_str())["fire1"](&weapons[ent->weapon],ent);}
int fire2(Entity *ent){return getGlobal(L,ent->weapon.c_str())["fire2"](&weapons[ent->weapon],ent);}
void gameloop() {
	getGlobal(L,"Global")["update"]();
	getGlobal(L,"Level")["update"]();
	update_entities();
	update_intervals();
}

bool get_key(string k){
	if(k=="up")		return key[SDL_SCANCODE_W];
	if(k=="down")	return key[SDL_SCANCODE_S];
	if(k=="left")	return key[SDL_SCANCODE_A];
	if(k=="right")	return key[SDL_SCANCODE_D];
	if(k=="jump")	return key[SDL_SCANCODE_SPACE];
	if(k=="1")		return key[SDL_SCANCODE_1];
	if(k=="2")		return key[SDL_SCANCODE_2];
	if(k=="3")		return key[SDL_SCANCODE_3];
	if(k=="4")		return key[SDL_SCANCODE_4];
	if(k=="5")		return key[SDL_SCANCODE_5];
	if(k=="6")		return key[SDL_SCANCODE_6];
	if(k=="7")		return key[SDL_SCANCODE_7];
	if(k=="8")		return key[SDL_SCANCODE_8];
	if(k=="9")		return key[SDL_SCANCODE_9];
	if(k=="0")		return key[SDL_SCANCODE_0];
	if(k=="fire" || k=="fire1")
					return mouse.state&&mouse.b==SDL_BUTTON_LEFT;
	if(k=="fire2")	return mouse.state&&mouse.b==SDL_BUTTON_RIGHT;
	throw logic_error("\""+k+"\" is not a key");
}
Player *get_player(){return &player;}
void bind() {
	#define KEY(key) SDL_GetKeyboardState(key)
	getGlobalNamespace(L)
		.addFunction("body",&get_body)
		.addFunction("joint",&get_joint)
		.addFunction("entity",&get_entity)
		.addFunction("weapon",&get_weapon)
		.addFunction("gettext",&get_text)
		.addFunction("print",&print)
		.addFunction("get_player",&get_player)
		.beginNamespace("world")
			.addFunction("set_gravity",&set_gravity)
			.addFunction("create_body",&create_body)
			.addFunction("create_entity",&create_entity)
			.addFunction("destroy_body",&destroy_body)
			.addFunction("destroy_entity",&destroy_entity)
			.addFunction("destroy_joint",&destroy_joint)
			.addFunction("collide",&collide)
			.addFunction("collide",&entity_collide)
			.addFunction("collide",&ee_collide)
			.addFunction("collide",&level_collide)
			.addFunction("collide",&level_entity_collide)
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
			.addProperty("timer",&SDL_GetTicks)
			.addFunction("key",&get_key)
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
		.endClass()
		.beginClass<b2Body>("Body")
			.addProperty("x",&b2Body::GetX,&b2Body::SetX)
			.addProperty("y",&b2Body::GetY,&b2Body::SetY)
			.addProperty("angle",&b2Body::GetAngle,&b2Body::SetAngle)
			.addProperty("vangle",&b2Body::GetAngularVelocity,&b2Body::SetAngularVelocity)
			.addProperty("vx",&b2Body::GetLinearVelocityX,&b2Body::SetLinearVelocityX)
			.addProperty("vy",&b2Body::GetLinearVelocityY,&b2Body::SetLinearVelocityY)
			.addProperty("gravity_scale",&b2Body::GetGravityScale, &b2Body::SetGravityScale)
			.addProperty("mass",&b2Body::GetMass)
			.addProperty("inertia",&b2Body::GetInertia)
			.addProperty("enabled",&b2Body::IsEnabled,&b2Body::SetEnabled)
			.addProperty("fixed_angle",&b2Body::IsFixedRotation,&b2Body::SetFixedRotation)
			.addProperty("awake",&b2Body::IsAwake,&b2Body::SetAwake)
			.addProperty("bullet",&b2Body::IsBullet,&b2Body::SetBullet)
			.addFunction("apply_force",&b2Body::Force)
			.addFunction("apply_center_force",&b2Body::CenterForce)
			.addFunction("apply_torque",&b2Body::Torque)
			.addFunction("apply_impulse",&b2Body::Impulse)
			.addFunction("apply_center_impulse",&b2Body::CenterImpulse)
			.addFunction("apply_angular_impulse",&b2Body::AngularImpulse)
		.endClass()
		.beginClass<Entity>("Entity")
			.addProperty("x",&Entity::getx,&Entity::setx)
			.addProperty("y",&Entity::gety,&Entity::sety)
			.addProperty("weapon",&Entity::get_weapon,&Entity::set_weapon)
			.addProperty("weapon_x",&Entity::weapon_x,0)
			.addProperty("weapon_y",&Entity::weapon_y,0)
			.addProperty("health",&Entity::health)
			.addProperty("weapon_angle",&Entity::weapon_angle)
			.addFunction("body",&Entity::get_body)
			.addFunction("joint",&Entity::get_joint)
			.addFunction("destroy_body",&Entity::destroy_body)
			.addFunction("destroy_joint",&Entity::destroy_joint)
			.addFunction("fire1",&Entity::fire1)
			.addFunction("fire2",&Entity::fire2)
		.endClass()
		.beginClass<Player>("Player")
			.addProperty("lives",&Player::lives)
			.addFunction("bullet",&Player::get_bullet)
		.endClass()
		.beginClass<Bullet>("Bullet")
			.addProperty("count",&Bullet::count)
			.addProperty("max",&Bullet::max)
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
	L = luaL_newstate();
	luaL_openlibs(L);
	bind();
	dostring(
		"Level={}\n"
		"Level.init=function()\n"
		"end\n"
		"Level.update=function()\n"
		"end\n"

		"Global={}\n"
		"Global.init=function()\n"
		"end\n"
		"Global.update=function()\n"
		"end\n"

		"Weapon={}\n"
		"Weapon.init=function()\n"
		"end\n"
		"Weapon.update=function()\n"
		"end\n"
		"Weapon.fire1=function()\n"
		"end\n"
		"Weapon.fire2=function()\n"
		"end\n"

		"Entity={}\n"
		"Entity.init=function()\n"
		"end\n"
		"Entity.update=function()\n"
		"end\n"

		"function extend(parent)\n"
			"local child = {}\n"
			"setmetatable(child,{__index = parent})\n"
			"return child\n"
		"end\n"
		"player=get_player()\n"
	);
	doscript("common/global");
	doscript("levels/"+name);
	getGlobal(L,"Global")["init"]();
	getGlobal(L,"Level")["init"]();
	init_entities();
}
void quit() {
	lua_close(L);
}
};
