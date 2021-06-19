#pragma once
#include <string>
using namespace std;
extern string prefix;
extern string saves;
#define TYPE(a,b) (static_cast<a>(b))


#ifdef ANDROID
#ifndef TAG
#define TAG "HAGEN"
#endif
#endif

#ifndef TOUCH
#ifdef ANDROID
#define TOUCH 1
#endif
#endif

#ifndef LOG_MAX
#define LOG_MAX 100
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
