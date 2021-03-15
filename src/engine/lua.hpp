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
using namespace std;
extern lua_State *L;
extern string L_name;

void lua_gameloop();
void lua_init(string);
void lua_quit();
