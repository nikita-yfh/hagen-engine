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
struct Color {
	Color();
	Color(int,int,int);
	Color(int,int,int,int);
	Color(SDL_Color);
	void set(int,int,int,int);
	uint8_t r,g,b,a;
	SDL_Color color();
};
struct Rect : GPU_Rect {
	Rect();
	Rect(float,float,float,float);
	void set(float,float,float,float);
	void load(XMLNode node,float h);
};
extern GPU_Target *ren;
extern int &SW;
extern int &SH;
void init();
void quit();
void panic(string message);
GPU_Image *find_texture(string id);
extern map<string,GPU_Image*>textures;
void set_cursor(string name);
void unset_cursor();
void set_center_cursor(string name);
void load_textures();
void destroy_all();
GPU_Image *load_texture(string str);
void load_body_textures(b2Body *body);
void load_entity_textures(Entity *ent);
void load_font(FC_Font *&font, XMLNode node,string color,float h);
void play_music(string name);
void stop_music();
void pause_music();
void resume_music();
void play_sound(string name);
void play_ws_sound(string name);
void play_distance_sound(string name,float distance);
void clear_sounds();
