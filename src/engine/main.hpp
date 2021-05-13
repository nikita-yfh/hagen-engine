#pragma once
#include <string>
using namespace std;
extern string prefix;
extern string saves;
#define TYPE(a,b) (static_cast<a>(b))

struct Settings {
	void save();
	void load();
	void def();
	int SW;
	int SH;
	bool fullscreen;
	int fps;
	int music_volume;
	int sound_volume;
	int sound_freq;
	string language;
	bool loaded=false;
};
extern Settings settings;
