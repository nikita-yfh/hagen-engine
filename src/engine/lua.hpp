#pragma once
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"
#include "xmlParser.h"
#include "Vector.h"
#include "entity.hpp"
#include <vector>
#include <string>
using namespace std;
using namespace luabridge;
namespace lua {
extern string need_load;
extern lua_State *L;
bool get_interval(unsigned int ms);
void gameloop();
void init(string);
void quit();
void init_weapon(string weapon,bool ex=1);
void init_body(b2Body *body,bool ex=1);
void init_entity(Entity *entity,bool ex=1);
void clear_loaded_list();
int fire1(Entity *ent);
int fire2(Entity *ent);
int fire3(Entity *ent);
int fire4(Entity *ent);

void create_entity_userdata(Entity *e);
void create_body_userdata(b2Body *b);
void create_joint_userdata(b2Joint *j);
int get_time();

vector<string>get_table_keys(string name);

void save_luaref(XMLNode n,LuaRef value);
LuaRef load_luaref(XMLNode n);
};
