#pragma once
#include "box2d.h"
#include "weapon.hpp"
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
struct Entity {
	Entity();
	Entity(string type,float x,float y);
	~Entity();
	string weapon;
	float health;
	string type;
	unordered_map<string,b2Body*>bodies;
	unordered_map<string,b2Joint*>joints;
	float weapon_x=0;
	float weapon_y=0;
	float weapon_angle;
	float dx;
	float dy;
	b2Body *get_body(string id);
	void set_weapon(string id);
	string get_weapon() const;
	float getx() const;
	float gety() const;
	void setx(float x);
	void sety(float y);
	b2Body *get_first() const;
	b2Joint *get_joint(string id);
	void destroy_body(string id);
	void destroy_joint(string id);
	void fire1();
	void fire2();
};
