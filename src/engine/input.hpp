#pragma once
#include <vector>
#include "xmlParser.h"
#include "sdl.hpp"
#include "main.hpp"

extern SDL_Event e;

void copy_prev_key();
extern uint8_t prev_key[SDL_NUM_SCANCODES];

short get_scancode(string k);

float mouse_angle();

struct Mouse {
	enum{
		None,
		Down,
		Press,
		Up
	};
	uint8_t state=0;
	uint8_t b=0;
	int x,y;
	float angle;
	void update();
	void clear();
	float g_angle();
	bool sensor_press=0;
#ifdef TOUCH
	uint16_t id=-1;
#endif
};
extern Mouse mouse;
#ifdef TOUCH
struct Sensor {
	Sensor();
	Sensor(XMLNode node);
	GPU_Rect pos;
	GPU_Rect image;
	uint16_t id;
	short key;
	bool active,pactive;
	bool enabled;
	bool update();
	void draw();
};

extern vector<Sensor>sensors;
namespace sensor{
bool update();

void load();

void draw();

bool get(short key);
bool pget(short key);
};
#endif
bool key(short code);
bool pkey(short code);

float get_angle(float x,float y);
