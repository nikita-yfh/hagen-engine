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
	void set_texture(string tex);
	string get_texture() const;
};
extern unordered_map<string,Weapon>weapons;
