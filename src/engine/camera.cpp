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
