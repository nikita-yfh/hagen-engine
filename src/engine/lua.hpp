#pragma once
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"
#include "Vector.h"
#include "entity.hpp"
#include <vector>
#include <string>
using namespace std;
namespace lua{
extern lua_State *L;

void gameloop();
void init(string);
void quit();
void init_weapon(string weapon);
void fire1(Entity *ent);
void fire2(Entity *ent);
};
