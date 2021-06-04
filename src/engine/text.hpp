#pragma once
#include <string>
#include <map>
#include "sdl.hpp"
#include "b2_math.h"
using namespace std;
namespace text{
extern string language;
string get(string id);
const char* getc(string id);
void preload();
void clear_locale();
void clear_text();
void add_tip_color(float x,float y,string text,Color color);
void add_tip(float x,float y,string text);
void add_tip_color_time(float x,float y,string text,Color color,float time);
void add_tip_time(float x,float y,string text,float time);
void load_config();
void draw();
void update();
struct Text{
	string text;
	Color color;
	float timer;
};

struct Tip : Text{
	Tip(float x,float y,string text,Color color);
	Tip(float x,float y,string text);
	Tip(float x,float y,string text,Color color,float time);
	Tip(float x,float y,string text,float time);
	void set(float x,float y,string text,Color color,float time);
	b2Vec2 pos;
};
};
