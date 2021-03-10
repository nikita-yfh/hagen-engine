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
	float x;
	float y;
	std::string type;
	std::map<std::string,b2Body*>bodies;
	std::map<std::string,b2Joint*>joints;
	b2Body *get_body(std::string id);
	b2Joint *get_joint(std::string id);
};
