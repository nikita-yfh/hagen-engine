#pragma once
#include <box2d.h>
#include <SDL_gpu.h>
#include <string>
#include <unordered_map>
#include <xmlParser.h>
using namespace std;
struct Weapon{
	float rate;
	string name;
	virtual void reload(){};
	GPU_Image *texture;
	int dx;
	int dy;
};
void load_weapon();
extern unordered_map<string,Weapon>weapons;
