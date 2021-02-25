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
