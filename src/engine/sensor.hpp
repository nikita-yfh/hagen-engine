#pragma once
#include <vector>
#include "xmlParser.h"
#include "sdl.hpp"

namespace sensors{

struct Sensor {
	Sensor();
	Sensor(XMLNode node);
	GPU_Rect pos;
	GPU_Rect image;
	short key;
	bool active,pactive;
	bool enabled;
	bool update(int x,int y,uint8_t state);
	void draw();
};

extern vector<Sensor>sensors;

bool update(int x,int y,uint8_t state);

void load();

void draw();

void disable();
void enable();

bool get(short key);
bool pget(short key);
};
