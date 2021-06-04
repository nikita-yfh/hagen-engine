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
void load_config();
void draw();
struct Text{
	string text;
	Color color;
};

struct Tip : Text{
	Tip(float x,float y,string text,Color color);
	Tip(float x,float y,string text);
	b2Vec2 pos;
};
};
