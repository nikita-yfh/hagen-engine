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
	if(argc>2)prefix=argv[2];
	try {
		init();
	}catch(...){
		settings.def();
		settings.save();
	}
	try{
		interface.load_config();
		interface.init_imgui();
		if(argc>1) load_level(argv[1]);
		else       load_level("ground1");
		game();
		quit();
		return 0;
	} catch(XMLError er) {
		panic(XMLNode::getError(er));
	} catch(string &er) {
		panic(er);
	} catch(LuaException &er) {
		panic(er.what());
	} catch(exception &er) {
		panic(er.what());
	} catch(...) {
		panic("Unknown error");
	}
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
}
void Settings::load() {
	try {
		XMLNode Main=XMLNode::openFileHelper((saves+"settings.xml").c_str(),"settings");
		XMLNode graphics=Main.getChildNode("graphics");
		SW=stoi(graphics.getAttribute("w"));
		SH=stoi(graphics.getAttribute("h"));
		fullscreen=stoi(graphics.getAttribute("fullscreen"));
		XMLNode sound=Main.getChildNode("sound");
		sound_volume=stoi(sound.getAttribute("sound_volume"));
		music_volume=stoi(sound.getAttribute("music_volume"));
		sound_freq=stoi(sound.getAttribute("frequency"));
		XMLNode game=Main.getChildNode("game");
		language=game.getAttribute("language");
	} catch(...) {
		def();
		save();
	}
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
}
