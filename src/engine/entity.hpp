#pragma once
#include "box2d.h"
#include <string>
#include <map>
#include <vector>
using namespace std;

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"
struct Weapon {
	float dx=0;
	float dy=0;
	string bullet1;
	string bullet2;
	string name;
	string texture;
	float angle=0;
	float point_x=0;
	float point_y=0;
	void set_texture(string texture);
	string get_texture() const;
};
struct Entity {
	Entity();
	Entity(string type,float x,float y);
	~Entity();
	Weapon weapon;
	float health=100;
	float max_health=100;
	string type;
	map<string,b2Body*>bodies;
	map<string,b2Joint*>joints;
	float dx;
	float dy;
	string id;
	bool created=false;
	b2Body *get_body(string id);
	void set_weapon(string id);
	string get_weapon() const;
	float get_vx() const;
	float get_vy() const;
	float getx() const;
	float gety() const;
	void setx(float x);
	void sety(float y);
	b2Body *get_first() const;
	b2Joint *get_joint(string id);
	void destroy_body(string id);
	void destroy_joint(string id);
	int fire1();
	int fire2();
	int fire3();
	int fire4();
	void harm(int d);
	void heal(int d);
	luabridge::LuaRef *lua_userdata;
	void focus(float x,float y);
	void focus_on_entity(Entity *e);
	void focus_on_body(b2Body *b);
};
