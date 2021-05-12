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
	bool run=1;
	interface.load_config();
	interface.init_imgui();
	auto fps=chrono::high_resolution_clock::now();
	while(run) {
		if(lua::need_load.size()) {
			load_level(lua::need_load);
			lua::need_load="";
		}
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
			case SDL_QUIT:
				run=0;
				break;
			case SDL_KEYDOWN:
				if(key[SDL_SCANCODE_O])
					save_world_state("save");
				if(key[SDL_SCANCODE_P])
					load_world_state("save");
			}
			interface.update();
		}
		mouse.update();
		if(!interface.shown())
			lua::gameloop();
		draw();
		auto step=chrono::high_resolution_clock::now()-fps;
		fps = chrono::high_resolution_clock::now();
		if(!interface.shown()){
			world->Step(chrono::duration_cast<chrono::microseconds>(step).count()/1000000.0f*lua::time_scale,
						velocity_iterations,position_iterations);
			update_fluid();
		}
	}
	return 0;
}
