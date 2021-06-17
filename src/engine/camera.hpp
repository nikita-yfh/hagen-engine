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
	uint8_t g_state=0;
	uint8_t state=0;
	//0-ничего
	//1-нажатие
	//2-удерживание
	//3-отпускание
	uint8_t b=0;
	int x,y;
	float angle;
	bool update();
	void clear();
	float g_angle();
	bool sensor_press=0;
};
extern Mouse mouse;
