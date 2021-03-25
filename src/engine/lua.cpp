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
#include <detail/Userdata.h>
using namespace luabridge;
using namespace std;
lua_State *L;
map<unsigned int,unsigned int>timers;
bool get_interval(unsigned int ms){
	if(SDL_GetTicks()-timers[ms]>ms){
		timers[ms]=SDL_GetTicks();
		return 1;
	}
	return 0;
}
//НАЧАЛО КОСТЫЛЕЙ И ОСНОВНЫХ ПРИЧИН БАГОВ
void set_mask(Color &c) {
	scene_mask=c;
}
Color &get_mask() {
	return scene_mask;
}
void dofile(string file){
	if(luaL_dofile(L, file.c_str())){
		throw string(lua_tostring(L, -1));
	}
}
void doscript(string file){
	dofile(prefix+"scripts/"+file+".lua");
}
void lua_init_entities(){
	for(auto entity : entities){
		luaL_dostring(L,(entity.second->type+"=extend(Entity)\n").c_str());
		doscript(entity.second->type);
		getGlobal(L,entity.second->type.c_str())["init"](entity.second);
	}
}
void lua_update_entities(){
	for(auto entity : entities){
		getGlobal(L,entity.second->type.c_str())["update"](entity.second);
	}
}

void lua_gameloop() {
	getGlobal(L,"Global")["update"]();
	getGlobal(L,"Level")["update"]();
	lua_update_entities();
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
	if(k=="fire" || k=="fire1")
					return mouse.state&&mouse.b==SDL_BUTTON_LEFT;
	if(k=="fire2")	return mouse.state&&mouse.b==SDL_BUTTON_RIGHT;
	throw logic_error("\""+k+"\" is not a key");
}
void lua_bind() {
	#define KEY(key) SDL_GetKeyboardState(key)
	getGlobalNamespace(L)
		.addFunction("body",&get_body)
		.addFunction("joint",&get_joint)
		.addFunction("entity",&get_entity)
		.addFunction("gettext",&get_text)
		.beginNamespace("world")
			.addFunction("set_gravity",&set_gravity)
			.addFunction("create_body",&create_body)
			.addFunction("create_entity",&create_entity)
			.addFunction("destroy_body",&destroy_body)
			.addFunction("destroy_entity",&destroy_entity)
			.addFunction("destroy_joint",&destroy_joint)
		.endNamespace()
		.beginNamespace("game")
			.beginNamespace("camera")
				.addProperty("x",&cx)
				.addProperty("y",&cy)
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
			.addProperty("weapon",&Entity::weapon,0)
			.addProperty("health",&Entity::health)
			.addFunction("body",&Entity::get_body)
			.addFunction("joint",&Entity::get_joint)
			.addFunction("destroy_body",&Entity::destroy_body)
			.addFunction("destroy_joint",&Entity::destroy_joint)
		.endClass()
		.beginClass<GPU_Image>("Texture")
			.addProperty("w",&GPU_Image::w,0)
			.addProperty("h",&GPU_Image::h,0)
		.endClass()
		.beginClass<Weapon>("Weapon")
			.addProperty("dx",&Weapon::dx)
			.addProperty("dy",&Weapon::dy)
			.addProperty("texture",&Weapon::texture)
		.endClass();
		/*.beginClass<Weapon>("Weapon")
			.addProperty("dx",&Weapon::dx)
			.addProperty("dy",&Weapon::dy)
			.addProperty("texture",&Weapon::texture)
			.addProperty("rate",&Weapon::rate)
			.addProperty("damage",&Weapon::damage)
		.endClass();*/
}
void lua_init(string name) {
	L = luaL_newstate();
	luaL_openlibs(L);
	lua_bind();
	doscript("init");
	doscript("global");
	doscript(name);
	getGlobal(L,"Global")["init"]();
	getGlobal(L,"Level")["init"]();
	//////////////////////////
	doscript("pistol");
	getGlobal(L,"pistol")["init"]();
	//////////////////////////
	lua_init_entities();
}
void lua_quit() {
	lua_close(L);
}
