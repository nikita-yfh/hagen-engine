#pragma once
#include "box2d.h"
#include "SDL.h"
#include "sdl.hpp"
void draw_bgr();
void fixture_draw(b2Body *body,b2Fixture *fix);
void body_draw(b2Body *body);
void draw();
extern Color scene_mask;
extern bool show_textures;
