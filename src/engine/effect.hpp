#pragma once
#include "sdl.hpp"
#include <string>
#include <map>
#include <vector>
using namespace std;



struct Effect{
	Effect(){}
	Effect(string name);
	vector<GPU_Image*>anim;
	uint16_t period;
	void load(string name);
};

struct ShowEffect{
	ShowEffect(){}
	ShowEffect(Effect *e,float _x,float _y);
	Effect *effect;
	float x;
	float y;
	int begin_time;
	bool del();
};
void load_effect(string name);
void create_effect(string name,float x,float y);
void update_effects();
extern map<string,Effect>loaded_effects;
extern vector<ShowEffect>effects;
