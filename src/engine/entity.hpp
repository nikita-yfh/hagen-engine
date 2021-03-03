#pragma once
#include "box2d.h"
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"
#include <vector>
#include <string>
struct Entity {
	Entity();
	Entity(std::string xml,std::string lua);
	~Entity();
	int health;
	std::vector<b2Body*>bodies;
	std::vector<b2Joint*>joints;
	lua_State *script;
};
