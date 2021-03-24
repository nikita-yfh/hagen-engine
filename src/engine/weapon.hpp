#pragma once
#include <box2d.h>
#include <SDL_gpu.h>
#include <string>
#include <unordered_map>
#include "LuaBridge.h"
#include <xmlParser.h>
using namespace std;
using namespace luabridge;
struct Weapon{
	string texture;
	float dx;
	float dy;
};
void load_weapon(string id);
extern unordered_map<string,Weapon>weapons;
