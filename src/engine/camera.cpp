#include "camera.hpp"
#include "sdl.hpp"
#include "interface.hpp"
#include "utility.hpp"
#include <iostream>
Mouse mouse;
float cx=0,cy=0;
float zoom=100;
bool camera_locked=1;
void center_body(b2Body *body) {
	center(body->GetX(),body->GetY());
}
void center(float x,float y) {
	cx=x-SW/2/zoom;
	cy=y-SH/2/zoom;
}
float worldx(float dx) {
	return dx/zoom+cx;
}
float worldy(float dy) {
	return dy/zoom+cy;
}
float drawx(float wx) {
	return (wx-cx)*zoom;
}
float drawy(float wy) {
	return (wy-cy)*zoom;
}
b2Vec2 drawv(b2Vec2 v) {
	return zoom*(v-b2Vec2(cx,cy));
}
int drawix(float wx) {
	return (wx-cx)*zoom;
}
int drawiy(float wy) {
	return (wy-cy)*zoom;
}
float rotatex(b2Vec2 vec,float a) {
	return vec.x*cos(a)-vec.y*sin(a);
}
float rotatey(b2Vec2 vec,float a) {
	return vec.y*cos(a)+vec.x*sin(a);
}
b2Vec2 rotate(b2Vec2 vec,float a) {
	return {rotatex(vec,a),rotatey(vec,a)};
}
float mouse_angle() {
	if(interface.mainmenu.shown)return 0.0f;
	float x=SW/2-mouse.x;
	float y=SH/2-mouse.y;
	return get_angle(x,y);
}
float get_angle(float x,float y) {
	float a;
	if(x>=0)
		a= M_PI+atan(y/x);
	else if(y>0)
		a= 2*M_PI+atan(y/x);
	else
		a= atan(y/x);
	return a;
}
bool Mouse::update() {
#ifdef ANDROID
	int mx=e.tfinger.x*SW;
	int my=e.tfinger.y*SH;
#else
	int mx=e.button.x;
	int my=e.button.y;
#endif
	if(mx<0||mx>=SW||my<0||my>=SH) {
		mx=x;
		my=y;
	}
	if(state==1)
		state=2;
	else if(state==3)
		state=0;
	if(state!=2 && state !=3) {
		if(e.type==SDL_MOUSEBUTTONDOWN) {
			state=1;
			b=e.button.button;
		} else if(e.type==SDL_FINGERDOWN) {
			state=1;
			b=1;
		}
	} else if((e.type==SDL_MOUSEBUTTONUP || e.type==SDL_FINGERUP) && state !=0)
		state=3;
	x=mx;
	y=my;
	return 0;
}
bool Mouse::in_rect(GPU_Rect r) {
	return r.x<x && r.y<y && r.x+r.w>x && r.y+r.h>y;
}
void Mouse::clear() {
	state=0;
	b=0;
}
