#pragma once
#include "box2d.h"
#include "lua.hpp"
#include "weapon.hpp"
#include <string>
#include <unordered_map>
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
	string weapon;
	float health;
	string type;
	string id1;
	unordered_map<string,b2Body*>bodies;
	unordered_map<string,b2Joint*>joints;
	b2Body *get_body(string id);
	float getx() const;
	float gety() const;
	float get_weapon_x() const;
	float get_weapon_y() const;
	float set_weapon_x() const;
	float set_weapon_y() const;
	void setx(float x);
	void sety(float y);
	b2Body *get_first() const;
	b2Joint *get_joint(string id);
	void destroy_body(string id);
	void destroy_joint(string id);
	float weapon_x=0;
	float weapon_y=0;
	float dx;
	float dy;
};
