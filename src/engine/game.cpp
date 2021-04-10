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
#define DEBUG
bool game() {
	bool run=1;
	interface.load_config();
	auto fps=chrono::high_resolution_clock::now();
	load_texture("interface/live1.png");
	load_texture("interface/live2.png");
	while(run) {
		while(SDL_PollEvent(&e)){
			switch(e.type) {
			case SDL_QUIT:
				run=0;
				break;
			}
			mouse.update();
			interface.update();
		}
		if(!interface.console.shown)
			lua::gameloop();
		draw();
		auto step=chrono::high_resolution_clock::now()-fps;
		fps = chrono::high_resolution_clock::now();
		world.Step(chrono::duration_cast<chrono::microseconds>(step).count()/1000000.0f,10,10);
	}
	return 0;
}
