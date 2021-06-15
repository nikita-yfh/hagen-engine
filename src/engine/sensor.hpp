#pragma once

struct Sensor {
	GPU_Rect pos;
	unsigned short key;
	bool active;
	bool disabled;
	bool update(float mx,float my);
};

extern vector<Sensor>sensors;

void check_sensors(float x,float y);
