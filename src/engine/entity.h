#pragma once
#include "box2d.h"
struct Entity {
	Entity();
	int health;
	float *x;
	float *y;
	b2Body *base;
	b2Body *wheel;
	b2RevoluteJoint *joint;
};
