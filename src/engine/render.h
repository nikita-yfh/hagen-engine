#pragma once
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "box2d.h"
#include <cstring>
#include <stdlib.h>
#include <vector>
#include "physic.h"
#include "level.h"
void draw_bgr();
void fixture_draw(b2Body *body,b2Fixture *fix);
void body_draw(b2Body *body);
void draw();
