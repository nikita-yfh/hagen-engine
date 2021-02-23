#pragma once
#include "box2d.h"
extern float cx,cy;
extern float zoom;
void center(float x,float y);
void center_body(b2Body *body);
void camera_lock();
void camera_unlock();
float worldx(float dx);
float worldy(float dy);
float drawx(float wx);
float drawy(float dy);
int drawix(float wx);
int drawiy(float dy);
