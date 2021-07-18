#include "main.hpp"
#include "lua.hpp"
#include "utility.hpp"
#include "physic.hpp"
#include "level.hpp"
#include "box2d.h"
#include "sdl.hpp"
#include <cstring>
#include "text.hpp"
#include "weapon.hpp"
#include "camera.hpp"
#include <chrono>
#include "render.hpp"
#include "imgui_impl_sdl.h"
string prefix="game/";
string saves="saves/";
string program_name;
using namespace luabridge;

static void collectargs(int argc, char * argv[]){
	for(int q=0;q<argc;q++){
		program_name += argv[q];
		program_name += " ";
	}
	if(argc>1)levelname=argv[1];
	if(argc>2)prefix=argv[2];

}
#include "dialog.hpp"
int main(int argc, char * argv[]) {
	info_log("Welcome to Hagen Engine!");
	collectargs(argc,argv);
#ifdef ANDROID
	saves=SDL_AndroidGetInternalStoragePath();
	saves+="/";
	prefix="";
#endif
	init();
	lua::init();
	text::load_config();
	if(levelname.size())
		load_level(levelname,true);
	else
		interface.mainmenu.show();
	auto time=chrono::high_resolution_clock::now();
	while(1) {
		int t=SDL_GetTicks();
		if(lua::need_load.size()) {
			if(lua::need_load.find("[LOAD]")==0){
				lua::need_load.erase(lua::need_load.begin(),lua::need_load.begin()+6);
				load_world_state(lua::need_load);
			}else
				load_level(lua::need_load,interface.mainmenu.shown);
			lua::need_load="";
		}
		copy_prev_key();
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_QUIT)
				quit();
			mouse.update();
			interface.update();
			interface.update_imgui();
		}
		interface.new_frame();
		draw1();
		//info_log(format("DRAW1: %d",SDL_GetTicks()-t));
		auto step=chrono::high_resolution_clock::now()-time;
		time = chrono::high_resolution_clock::now();
		if(!interface.shown() && lua::time_scale) {
			world->Step(chrono::duration_cast<chrono::microseconds>(step).count()/1000000.0f*lua::time_scale,
						velocity_iterations,position_iterations);
			update_fluid();
		}
		if(!interface.shown())
			lua::gameloop();
		draw2();
	}
	return 0;
}

static void mkdir(const char* path){
	char s[1024];
	sprintf(s,"mkdir %s",path);
	if(system(s))
		panic(format("Error while creating directory %s",path));
	else
		info_log(format("Created directory %s",path));
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
	XMLNode game=Main.addChild("game");
	game.addAttribute("language",language);
	XMLError e=Main.writeToFile((saves+"settings.xml").c_str());
	if(e!=XMLError::eXMLErrorNone)
		panic(format("%s: %s",e,"settings.xml"));
	else
		info_log("Saved settings to settings.xml");
}
void Settings::load() {
	XMLNode Main=open_xml((saves+"settings.xml").c_str(),"settings");
	if(Main.isEmpty()) {
#ifndef ANDROID
		mkdir(saves.c_str());
#endif
		def();
		save();
		load();
		return;
	}
	XMLNode graphics=Main.getChildNode("graphics");
	SW=graphics.getAttributei("w");
	SH=graphics.getAttributei("h");
	fullscreen=graphics.getAttributei("fullscreen");
	XMLNode sound=Main.getChildNode("sound");
	sound_volume=sound.getAttributei("sound_volume");
	music_volume=sound.getAttributei("music_volume");
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
	info_log("Restored default settings");
}
