#pragma once
#include "SDL.h"
#include "entity.hpp"
#include <string>
#include <map>
#include "SDL_gpu.h"
#include "SDL_mixer.h"
#include "SDL_FontCache.h"
#include "box2d.h"
#include "xmlParser.h"
#define GAME_KEY_COUNT 21
struct Color {
	Color();
	Color(int,int,int);
	Color(int,int,int,int);
	Color(XMLNode);
	void set(int,int,int,int);
	uint8_t r,g,b,a;
	SDL_Color color();
	void load(XMLNode node);
};
struct Rect : GPU_Rect {
	Rect();
	Rect(float,float,float,float);
	void set(float,float,float,float);
	void load(XMLNode node);
};
extern SDL_Event e;
extern GPU_Target *ren;
extern const uint8_t *key;
extern int SW;
extern int SH;
void init(const char* title,int w,int h);
void quit();
void panic(string message);
GPU_Image *find_texture(string id);
extern map<string,GPU_Image*>textures;
void set_cursor(string name);
void load_textures();
void configure_textures();
void destroy_all();
GPU_Image *load_texture(string str);
void load_body_textures(b2Body *body);
void load_entity_textures(Entity *ent);
void load_font(FC_Font *&font, XMLNode node,string color);
void play_music(string name);
void stop_music();
void pause_music();
void resume_music();
void play_sound(string name);
void play_ws_sound(string name);
void play_distance_sound(string name,float x,float y);
