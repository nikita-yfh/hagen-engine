#pragma once
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "box2d.h"
#include <cstring>
#include <stdlib.h>
#include <vector>
#include "physic.hpp"
#include "level.hpp"
void draw_bgr();
void fixture_draw(b2Body *body,b2Fixture *fix);
void body_draw(b2Body *body);
void draw();
struct Color {
	Color(int,int,int);
	Color(int,int,int,int);
	void set(int,int,int,int);
	uint8_t r,g,b,a;
	SDL_Color color();
};
extern Color scene_mask;
