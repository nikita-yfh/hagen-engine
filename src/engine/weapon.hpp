#pragma once
#include <box2d.h>
#include <SDL_gpu.h>
#include <string>
#include <xmlParser.h>
#include <map>
#include "entity.hpp"
using namespace std;
struct Weapon {
	float dx;
	float dy;
	string bullet1;
	string bullet2;
};
struct Bullet {
	int count; //if count=-1,bullets are infinity
	int max;
	void add(int c);
	bool full();
};
extern map<string,Weapon>weapons;
extern map<string,Bullet>bullets;
Bullet *get_bullet(string id);
b2Body *simple_bullet(Weapon *w, Entity *e, string type, float impulse);
