#pragma once
#include <string>
using namespace std;
extern string prefix;
extern string saves;
#define TYPE(a,b) (static_cast<a>(b))

#ifdef ANDROID
#define TOUCH
#endif

struct Settings {
	void save();
	void load();
	void def();
	int SW;
	int SH;
	bool fullscreen;
	int music_volume;
	int sound_volume;
	string language;
	bool loaded=false;
};
extern Settings settings;
