#pragma once
#include "box2d.h"
#include "lua.hpp"
#include <vector>
#include <string>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"
struct Entity {
	Entity();
	Entity(std::string type,std::string id,float x,float y);
	~Entity();
	float health;
	float x;
	float y;
	std::string id;
	std::string type;
	std::vector<b2Body*>bodies;
	std::vector<b2Joint*>joints;
	b2Body *get_body(std::string id);
	b2Joint *get_joint(std::string id);
	luabridge::LuaRef l=luabridge::newTable(L);
};
