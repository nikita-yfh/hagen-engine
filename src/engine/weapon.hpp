#pragma once
#include <box2d.h>
#include <SDL_gpu.h>
#include <string>
#include <unordered_map>
#include <xmlParser.h>
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
};
extern unordered_map<string,Weapon>weapons;
