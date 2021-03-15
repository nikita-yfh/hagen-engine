#pragma once
#include "box2d.h"
#include "lua.hpp"
#include <string>
#include <map>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"
using namespace std;
struct Entity {
	Entity();
	Entity(string type,float x,float y);
	~Entity();
	float health;
	float dx;
	float dy;
	string type;
	string id1;
	map<string,b2Body*>bodies;
	map<string,b2Joint*>joints;
	b2Body *get_body(string id);
	float getx() const;
	float gety() const;
	void setx(float x);
	void sety(float y);
	b2Body *get_first() const;
	b2Joint *get_joint(string id);
	void destroy_body(string id);
	void destroy_joint(string id);
};
