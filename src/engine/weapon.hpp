#pragma once
#include <box2d.h>
#include <SDL_gpu.h>
#include <string>
#include <xmlParser.h>
#include <map>
#include "entity.hpp"
#include "lua.hpp"
using namespace std;
struct Bullet {
	int count; //if count<0,bullets are infinity
	int max;
	void add(int c);
	void del(int c);
	bool full();
};
extern map<string,Bullet>bullets;
Bullet *get_bullet(string id);
b2Body *simple_bullet(Entity *e, string type, float impulse);
void explosion(float x,float y,float radius,float power);
void explosion_callbacks(float x,float y,float radius,float power,luabridge::LuaRef c_entity, luabridge::LuaRef c_body);
void raycast_callbacks(float x,float y,float radius,luabridge::LuaRef c_entity, luabridge::LuaRef c_body);
