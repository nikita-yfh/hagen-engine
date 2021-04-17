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
namespace lua {
extern string need_load;
extern lua_State *L;

void gameloop();
void init(string);
void quit();
void init_weapon(string weapon);
int fire1(Entity *ent);
int fire2(Entity *ent);
int fire3(Entity *ent);
int fire4(Entity *ent);
};
