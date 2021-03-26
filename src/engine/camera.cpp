#include "camera.hpp"
#include "sdl.hpp"
float cx=0,cy=0;
float zoom=100;
bool camera_locked=1;
Mouse mouse;
void center_body(b2Body *body) {
	center(body->GetX(),body->GetY());
}
void center(float x,float y) {
	cx=-x*zoom+SW/2;
	cy=-y*zoom+SH/2;
}
float worldx(float dx) {
	return dx/zoom-cx;
}
float worldy(float dy) {
	return dy/zoom-cy;
}
float drawx(float wx) {
	return wx*zoom+cx;
}
float drawy(float wy) {
	return wy*zoom+cy;
}
b2Vec2 drawv(b2Vec2 v) {
	return zoom*v+b2Vec2(cx,cy);
}
int drawix(float wx) {
	return wx*zoom+cx;
}
int drawiy(float wy) {
	return wy*zoom+cy;
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
float mouse_angle(){
	float x=SW/2-mouse.x;
	float y=SH/2-mouse.y;
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
	int mx=e.button.x;
	int my=e.button.y;
	if(mx<0||mx>=SW||my<0||my>=SH){
		mx=x;
		my=y;
	}
	if(e.type==SDL_MOUSEBUTTONDOWN) {
		state=1;
		b=e.button.button;
	}
	else if(e.type==SDL_MOUSEBUTTONUP)
		state=0;
	x=mx;
	y=my;
	return 0;
}
