#pragma once
#include <box2d.h>
#include <SDL_gpu.h>
#include <string>
#include <unordered_map>
#include <xmlParser.h>
#include "entity.hpp"
struct Weapon{
	Weapon(string _name);
	void set(XMLNode node);
	float rate;
	string name;
	virtual void atack(Entity *ent){};
	virtual void reload(){};
	GPU_Image *texture;
	GPU_Image *fire_animation;
	GPU_Image *reload_animation;
	int dx;
	int dy;
};
struct Firearms : Weapon{
	Firearms(XMLNode node);
	void atack(Entity *ent);
};
void load_weapon();
extern unordered_map<string,Weapon*>weapons;
