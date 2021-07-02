#pragma once
#include <string>
#include <map>
#include "sdl.hpp"
#include "b2_math.h"
#include "lua.hpp"
using namespace std;
namespace text {
extern string language;
string get(string id);
const char* getc(string id);
void preload();
void clear_locale();
void clear_text();
Color defcolor();
int add_tip(lua_State *L);
int add_subtitles(lua_State *L);
void load_config();
void draw();
void update();
struct Subtitles {
	Subtitles(string text,Color color,float time);
	void set(string text,Color color,float time);
	string text;
	Color color;
	float timer;
};

struct Tip {
	Tip(float x,float y,string text,Color color,float time);
	void set(float x,float y,string text,Color color,float time);
	void draw();
	b2Vec2 pos;
	string text;
	Color color;
	float timer;
};
};
