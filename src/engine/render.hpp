#pragma once
#include "box2d.h"
#include "SDL.h"
#include "sdl.hpp"
extern float tex_scale;
extern float weapon_scale;
extern float effect_scale;
void draw();
extern Color scene_mask;
extern bool show_textures;
void init_target();
void delete_target();
