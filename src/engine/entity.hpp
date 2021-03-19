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
	Weapon *weapon;
	float health;
	string type;
	string id1;
	unordered_map<string,b2Body*>bodies;
	unordered_map<string,b2Joint*>joints;
	b2Body *get_body(string id);
	float getx() const;
	float gety() const;
	void setx(float x);
	void sety(float y);
	b2Body *get_first() const;
	b2Joint *get_joint(string id);
	void destroy_body(string id);
	void destroy_joint(string id);
	void set_weapon(string id);
};
struct EntityAll{
	float veapon_x;
	float veapon_y;
	float dx;
	float dy;
};
