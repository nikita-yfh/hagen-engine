#pragma once
#include "box2d.h"
#include "lua.hpp"
#include <string>
#include <map>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"
struct Entity {
	Entity();
	Entity(std::string type,float x,float y);
	~Entity();
	float health;
	float dx;
	float dy;
	std::string type;
	std::string id1;
	std::map<std::string,b2Body*>bodies;
	std::map<std::string,b2Joint*>joints;
	b2Body *get_body(std::string id);
	float getx() const;
	float gety() const;
	void setx(float x);
	void sety(float y);
	b2Body *get_first() const;
	b2Joint *get_joint(std::string id);
};
