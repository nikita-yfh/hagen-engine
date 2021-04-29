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
#include <chrono>
#define TYPE(a,b) (static_cast<a>(b))
bool game() {
	bool run=1;
	interface.load_config();
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
				if(e.key.keysym.sym==SDLK_9){
					explosion(entities["player"]->getx() ,entities["player"]->gety()-1,500,100000000);
				}
				break;
			}
			interface.update();
		}
		mouse.update();
		if(!interface.console.shown && !interface.pause.shown)
			lua::gameloop();
		draw();
		auto step=chrono::high_resolution_clock::now()-fps;
		fps = chrono::high_resolution_clock::now();
		if(!interface.pause.shown)
			world->Step(chrono::duration_cast<chrono::microseconds>(step).count()/1000000.0f*time_scale,
						velocity_iterations,position_iterations);
	}
	return 0;
}
