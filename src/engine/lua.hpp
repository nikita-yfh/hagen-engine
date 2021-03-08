#pragma once
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"
#include "Vector.h"
#include <vector>
#include <string>

extern lua_State *L;
extern std::string L_name;

void lua_gameloop();
void lua_init(std::string);
void lua_quit();

extern bool mainscript_enabled;
