#pragma once
extern "C" {
#include "luajit-2.0/lua.h"
#include "luajit-2.0/lauxlib.h"
#include "luajit-2.0/lualib.h"
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
void init_body(b2Body *body);
void init_entity(Entity *entity);
void clear_loaded_list();
int fire1(Entity *ent);
int fire2(Entity *ent);
int fire3(Entity *ent);
int fire4(Entity *ent);

void create_entity_userdata(Entity *e);
void create_body_userdata(b2Body *b);
void create_joint_userdata(b2Joint *j);
};
