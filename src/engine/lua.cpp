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
bool mainscript_enabled=1;
string L_name;
lua_State *L;
//НАЧАЛО КОСТЫЛЕЙ И ОСНОВНЫХ ПРИЧИН БАГОВ
void set_mask(Color &c) {
	scene_mask=c;
}
Color &get_mask() {
	return scene_mask;
}

void lua_gameloop() {
	try {
		if(mainscript_enabled) {
			getGlobal(L,"Level")["update"]();
		}
	} catch(exception &e) {
		panic("Lua error in \""+L_name+"\"",e.what());
	}
}

void lua_load_entity_script(std::string type){
	luaL_dofile(L,("entities/"+type+".lua").c_str());
}

void lua_fill_bodies() {
	for(int q=0; q<bodies.size(); q++) {
		string id=bodies[q]->GetID();
		if(id.size()==0)throw string("Empty body ID");
		if(id.find(" ")!=string::npos)throw string("Spaces in body ID \""+id+"\"");
		if(isdigit(id[0]))throw string("First character of body ID \""+id+"\" is digit");
		getGlobalNamespace(L)
		.beginNamespace("bodies")
		.addProperty<b2Body*>(id.c_str(),&bodies[q])
		.endNamespace();
	}
}
void lua_fill_joints() {
	for(int q=0; q<joints.size(); q++) {
		string id=joints[q]->GetID();
		if(id.size()==0)throw string("Empty joint ID");
		if(id.find(" ")!=string::npos)throw string("Spaces in joint ID \""+id+"\"");
		if(isdigit(id[0]))throw string("First character of joint ID \""+id+"\" is digit");
		getGlobalNamespace(L)
		.beginNamespace("joints")
		.addProperty(id.c_str(),&joints[q])
		.endNamespace();
	}
}

void lua_bind() {
	getGlobalNamespace(L)
		.beginNamespace("game")
			.beginNamespace("camera")
				.addProperty("x",&cx)
				.addProperty("y",&cy)
				.addProperty("zoom",&zoom)
				.addFunction("center",&center)
				.addFunction("center_body",&center_body)
			.endNamespace()
		.addProperty("timer",&SDL_GetTicks)
			.beginClass<Color>("Color")
				.addConstructor<void(*)(uint8_t,uint8_t,uint8_t,uint8_t)>()
				.addConstructor<void(*)(uint8_t,uint8_t,uint8_t)>()
				.addProperty("r",&Color::r)
				.addProperty("g",&Color::g)
				.addProperty("b",&Color::b)
				.addProperty("a",&Color::a)
			.addFunction("set",&Color::set)
			.endClass()
			.addProperty("mask",&get_mask,&set_mask)
		.endNamespace()
		.beginClass<b2Joint>("Joint")
			.addProperty("a",&b2Joint::m_bodyA,0)
			.addProperty("b",&b2Joint::m_bodyB,0)
			.addProperty("id",&b2Joint::GetID,&b2Joint::SetID)
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
			.addProperty("x",&b2Body::GetX)
			.addProperty("y",&b2Body::GetY)
			.addProperty("angle",&b2Body::GetAngle)
			.addProperty("vangle",&b2Body::GetAngularVelocity,&b2Body::SetAngularVelocity)
			.addProperty("vx",&b2Body::GetLinearVelocityX,&b2Body::SetLinearVelocityX)
			.addProperty("vy",&b2Body::GetLinearVelocityY,&b2Body::SetLinearVelocityY)
			.addProperty("gravity_scale",&b2Body::GetGravityScale, &b2Body::SetGravityScale)
			.addProperty("mass",&b2Body::GetMass)
			.addProperty("inertia",&b2Body::GetInertia)
			.addProperty("id",&b2Body::GetID, &b2Body::SetID)
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
		.endClass();
		/*.beginClass<Entity>("Entity")
			.addProperty("x",&Entity::x)
			.addProperty("y",&Entity::y)
			.addProperty("health",&Entity::health)
		.endClass();*/
}

void lua_init(string name) {
	try {
		L_name="levels/"+name+".lua";
		if(!exist_file(L_name))
			mainscript_enabled=0;
		else {
			L = luaL_newstate();
			luaL_openlibs(L);
			lua_bind();
			lua_fill_bodies();
			lua_fill_joints();
			luaL_dostring(L,
				"Level={}\n"
				"function extend(parent)\n"
					"local child = {}\n"
					"setmetatable(child,{__index = parent})\n"
					"return child\n"
				"end\n"
				"Entity={\n"
					"x=10;\n"
					"y=10;\n"
					"health=100;\n"
				"};\n"
			);
			get_entity("player").l["z"].append<int>(6);
			luaL_dofile(L, L_name.c_str());
			getGlobal(L,"Level")["init"]();
		}
	} catch(exception &e) {
		panic("Lua error in \""+L_name+"\"",e.what());
	}
}
void lua_quit() {
	if(mainscript_enabled)
		lua_close(L);
}
