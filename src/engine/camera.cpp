#include "camera.hpp"
#include "sdl.hpp"
float cx=0,cy=0;
float zoom=100;
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
