#pragma once
#include "box2d.h"
#include "sdl.hpp"
extern float cx,cy;
extern float zoom;
void center(float x,float y);
void center_body(b2Body *body);
extern bool camera_locked;
float worldx(float dx);
float worldy(float dy);
float drawx(float wx);
float drawy(float dy);
b2Vec2 drawv(b2Vec2 v);
int drawix(float wx);
int drawiy(float dy);
float rotatex(b2Vec2 vec,float a);
float rotatey(b2Vec2 vec,float a);
b2Vec2 rotate(b2Vec2 vec,float a);
float mouse_angle();
float get_angle(float x,float y);
struct Mouse {
	uint8_t state=0;
	uint8_t b=0;
	int x,y;
	bool update();
	bool in_rect(GPU_Rect rect);
};
extern Mouse mouse;
