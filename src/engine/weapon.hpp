#pragma once
#include <box2d.h>
#include <SDL_gpu.h>
#include <string>
#include <unordered_map>
#include <xmlParser.h>
using namespace std;
struct Weapon{
	string texture;
	float dx;
	float dy;
};
void load_weapon(string id);
extern unordered_map<string,Weapon>weapons;
