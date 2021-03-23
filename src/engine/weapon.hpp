#pragma once
#include <box2d.h>
#include <SDL_gpu.h>
#include <string>
#include <unordered_map>
#include <xmlParser.h>
using namespace std;
struct Weapon{
	Weapon(){}
	Weapon(XMLNode node);
	string texture;
	float damage;
	string get_name();
	float dx;
	float dy;
};
struct Firearms : Weapon{
	Firearms(){}
	Firearms(XMLNode node);
	float max_spread;
	unsigned short magazine;
	string bullet;
	unsigned short reload_time;
};
void load_weapon(string id);
extern unordered_map<string,Weapon>weapons;
