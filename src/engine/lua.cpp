#include "lua.hpp"
#include "physic.hpp"
#include "render.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "level.hpp"
#include "sdl.hpp"
#include "utility.hpp"
#include <detail/Userdata.h>
using namespace luabridge;
using namespace std;
string L_name;
lua_State *L;
//НАЧАЛО КОСТЫЛЕЙ И ОСНОВНЫХ ПРИЧИН БАГОВ
void set_mask(Color &c) {
	scene_mask=c;
}
Color &get_mask() {
	return scene_mask;
}

void lua_init_entities(){
	for(auto entity : entities){
		luaL_dostring(L,(entity.second->type+"=extend(Entity)\n").c_str());
		luaL_dofile(L, ("scripts/"+entity.second->type+".lua").c_str());
		getGlobal(L,entity.second->type.c_str())["init"](entity.second);
	}
}
void lua_update_entities(){
	for(auto entity : entities){
		getGlobal(L,entity.second->type.c_str())["update"](entity.second);
	}
}

void lua_gameloop() {
	try {
		getGlobal(L,"Level")["update"]();
		lua_update_entities();
	} catch(exception &e) {
		panic("Lua error in \""+L_name+"\"",e.what());
	} /*catch(LuaException &e) {
		panic("Lua error in \""+L_name+"\"",e.what());
	}*/
}

void lua_load_entity_script(std::string type){
	luaL_dofile(L,("entities/"+type+".lua").c_str());
}

bool get_key(string k){
	if(k=="up")		return key[SDL_SCANCODE_W];
	if(k=="down")	return key[SDL_SCANCODE_S];
	if(k=="left")	return key[SDL_SCANCODE_A];
	if(k=="right")	return key[SDL_SCANCODE_D];
	if(k=="jump")	return key[SDL_SCANCODE_SPACE];
	if(k=="1")	return key[SDL_SCANCODE_1];
	if(k=="2")	return key[SDL_SCANCODE_2];
	throw runtime_error("\""+k+"\" is not a key");
}

void lua_bind() {
	#define KEY(key) SDL_GetKeyboardState(key)
	getGlobalNamespace(L)
		.addFunction("body",&get_body)
		.addFunction("joint",&get_joint)
		.addFunction("entity",&get_entity)
		.beginNamespace("game")
			.beginNamespace("camera")
				.addProperty("x",&cx)
				.addProperty("y",&cy)
				.addProperty("zoom",&zoom)
				.addFunction("center",&center)
				.addFunction("center_body",&center_body)
				.addProperty("locked",&camera_locked)
			.endNamespace()
			.addProperty("timer",&SDL_GetTicks)
			.addFunction("key",&get_key)
			.beginClass<Color>("Color")
				.addConstructor<void(*)(uint8_t,uint8_t,uint8_t,uint8_t)>()
				.addConstructor<void(*)(uint8_t,uint8_t,uint8_t)>()
				.addProperty("r",&Color::r)
				.addProperty("g",&Color::g)
				.addProperty("b",&Color::b)
				.addProperty("a",&Color::a)
			.addFunction("set",&Color::set)
			.endClass()
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
			.addProperty("health",&Entity::health)
			.addFunction("body",&Entity::get_body)
			.addFunction("joint",&Entity::get_joint)
		.endClass();
}
void lua_init(string name) {
	try {
		L_name="levels/"+name+".lua";
		L = luaL_newstate();
		luaL_openlibs(L);
		lua_bind();
		luaL_dostring(L,
			"Level={}\n"
			"function extend(parent)\n"
				"local child = {}\n"
				"setmetatable(child,{__index = parent})\n"
				"return child\n"
			"end\n"
			"Entity={}\n"
		);
		luaL_dofile(L, L_name.c_str());
		auto l_init=getGlobal(L,"Level")["init"];
		l_init();
		lua_init_entities();
	} catch(LuaException &e) {
		panic("Lua error in \""+L_name+"\"",e.what());
	} catch(logic_error& e) {
		panic("Lua error in \""+L_name+"\"",e.what());
	}
}
void lua_quit() {
	lua_close(L);
}
