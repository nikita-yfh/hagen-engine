#pragma once
#include <vector>
#include "xmlParser.h"
#include "sdl.hpp"

extern SDL_Event e;

void copy_prev_key();
extern uint8_t prev_key[SDL_NUM_SCANCODES];

short get_scancode(string k);

float mouse_angle();

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
namespace sensor{
bool update(int x,int y,uint8_t state);

void load();

void draw();

bool get(short key);
bool pget(short key);
};
bool key(short code);
bool pkey(short code);

float get_angle(float x,float y);
