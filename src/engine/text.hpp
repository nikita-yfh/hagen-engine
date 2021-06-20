#pragma once
#include <string>
#include <map>
#include "sdl.hpp"
#include "b2_math.h"
using namespace std;
namespace text {
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
void add_subtitles(string text);
void add_subtitles_color(string text,Color color);
void load_config();
void draw();
void update();
struct Subtitles {
	Subtitles(string text,Color color);
	void set(string text,Color color);
	string text;
	Color color;
	float timer;
};

struct Tip {
	Tip(float x,float y,string text,Color color,float time);
	void set(float x,float y,string text,Color color,float time);
	b2Vec2 pos;
	string text;
	Color color;
	float timer;
};
};
