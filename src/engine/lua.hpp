#pragma once
#include "LuaBridge.h"
#include "Vector.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "level.h"
#include <vector>
#include <string>

extern lua_State *level_script;
extern std::string level_script_name;

b2Body *GetBody(int id);
void lua_gameloop();
void lua_init(std::string);
void lua_quit();

extern bool mainscript_enabled;
