#include "main.hpp"
#include "lua.hpp"
#include "utility.hpp"
#include "physic.hpp"
#include "level.hpp"
#include "box2d.h"
#include "sdl.hpp"
#include <cstring>
#include "game.hpp"
#include "text.hpp"
#include "weapon.hpp"
string prefix="game/";
string saves="saves/";
using namespace luabridge;

int main(int argc, char * argv[]) {
	info_log("Welcome to Hagen Engine!");
	if(argc>2)prefix=argv[2];
#ifdef ANDROID
	saves=SDL_AndroidGetInternalStoragePath();
	saves+="/";
	prefix="";
#endif
	try {
		init();
	} catch(...) {
		error_log("Error while init engine");
		settings.def();
		settings.save();
	}
	text::load_config();
	if(argc>1) load_level(argv[1],true);
	else
		interface.mainmenu.show();
	game();
	quit();
	return 0;
}


Settings settings;

void Settings::save() {
	XMLNode Main=XMLNode::createXMLTopNode("settings");
	XMLNode graphics=Main.addChild("graphics");
	graphics.addAttribute("w",SW);
	graphics.addAttribute("h",SH);
	graphics.addAttribute("fullscreen",fullscreen);
	XMLNode sound=Main.addChild("sound");
	sound.addAttribute("sound_volume",sound_volume);
	sound.addAttribute("music_volume",music_volume);
	sound.addAttribute("frequency",sound_freq);
	XMLNode game=Main.addChild("game");
	game.addAttribute("language",language);
	Main.writeToFile((saves+"settings.xml").c_str());
	info_log("Saved settings to settings.xml");
}
void Settings::load() {
	XMLNode Main=open_xml((saves+"settings.xml").c_str(),"settings");
	XMLNode graphics=Main.getChildNode("graphics");
	SW=graphics.getAttributei("w");
	SH=graphics.getAttributei("h");
	fullscreen=graphics.getAttributei("fullscreen");
	XMLNode sound=Main.getChildNode("sound");
	sound_volume=sound.getAttributei("sound_volume");
	music_volume=sound.getAttributei("music_volume");
	sound_freq=sound.getAttributei("frequency");
	XMLNode game=Main.getChildNode("game");
	language=game.getAttribute("language");
	info_log("Loaded settings from settings.xml successfully");
}
void Settings::def() {
	fullscreen=true;
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0,&mode);
	SW=mode.w;
	SH=mode.h;
	sound_volume=100;
	music_volume=100;
	language="en";
	sound_freq=22050;
	info_log("Restored default settings");
}
