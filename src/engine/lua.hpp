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
namespace lua{
extern lua_State *L;

void gameloop();
void init(string);
void quit();
};
