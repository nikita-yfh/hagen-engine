#pragma once
#include "SDL.h"
#include "entity.hpp"
#include <string>
#include <map>
#include "SDL_gpu.h"
#include "box2d.h"
#define GAME_KEY_COUNT 21
extern SDL_Event e;
extern GPU_Target *ren;
extern const uint8_t *key;
extern int SW;
extern int SH;
void init(const char* title,int w,int h);
void quit();
void panic(string name,string message);
GPU_Image *find_texture(string id);
extern map<string,GPU_Image*>textures;
extern GPU_Image *background;
void load_cursor();
void load_textures();
void configure_textures();
void destroy_textures();
void load_background(string path);
void load_texture(string str);
void load_body_textures(b2Body *body);
void load_entity_textures(Entity *ent);
