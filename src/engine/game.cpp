#include "game.hpp"
#include "box2d.h"
#include "SDL.h"
#include "level.hpp"
#include "physic.hpp"
#include "render.hpp"
#include "lua.hpp"
#include "camera.hpp"
#include "sdl.hpp"
#include "utility.hpp"
#include "interface.hpp"
#include "main.hpp"
#include <chrono>
#define TYPE(a,b) (static_cast<a>(b))
bool game() {
	interface.load_config();
	interface.init_imgui();
	auto time=chrono::high_resolution_clock::now();
	while(1) {
		if(lua::need_load.size()) {
			load_level(lua::need_load);
			lua::need_load="";
		}
		while(SDL_PollEvent(&e)) {
			if(e.type==SDL_QUIT)
				quit();
			interface.update();
		}
		mouse.update();
		if(!interface.shown())
			lua::gameloop();
		draw();
		auto step=chrono::high_resolution_clock::now()-time;
		time = chrono::high_resolution_clock::now();
		if(!interface.shown()) {
			world->Step(chrono::duration_cast<chrono::microseconds>(step).count()/1000000.0f*lua::time_scale,
						velocity_iterations,position_iterations);
			update_fluid();
		}
	}
	return 0;
}
