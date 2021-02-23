#include "game.h"
#include "box2d.h"
#include "SDL.h"
#include "level.h"
#include "physic.h"
#include "render.h"
#include "lua.hpp"
#include "sdl.h"
bool game() {
	bool run=1;
    int fps=SDL_GetTicks();
	while(run) {
		SDL_PollEvent(&e);
		switch(e.type) {
		case SDL_QUIT:
			run=0;
			break;
		case SDL_WINDOWEVENT:
            SDL_GetWindowSize(window,&SW,&SH);
            break;
		}
		draw();
		SDL_Delay(1);
		lua_gameloop();
		int step=SDL_GetTicks()-fps;
		fps=SDL_GetTicks();
		world.Step(step/1000.0f,10,10);
	}
	return 0;
}
