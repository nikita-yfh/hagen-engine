#pragma once
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"
#include "Vector.h"
#include "level.hpp"
#include <vector>
#include <string>

extern lua_State *L;
extern std::string L_name;

b2Body *GetBody(int id);
void lua_gameloop();
void lua_init(std::string);
void lua_quit();

extern bool mainscript_enabled;
