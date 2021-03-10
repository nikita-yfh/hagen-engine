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
#define TYPE(a,b) (static_cast<a>(b))
#define DEBUG
bool game() {
	bool run=1;
	int fps=SDL_GetTicks();
	bool jump=0;
	while(run) {
		while(SDL_PollEvent(&e)){
			switch(e.type) {
			case SDL_QUIT:
				run=0;
				break;
#ifdef DEBUG
			static bool press=0;
			case SDL_MOUSEBUTTONDOWN:
				if(e.button.button==SDL_BUTTON_RIGHT){
					press=1;
				}
			case SDL_MOUSEMOTION:
				if(press){
					if(abs(e.motion.xrel)<100){
						cx+=e.motion.xrel*3;
						cy+=e.motion.yrel*3;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				press=0;
				break;
#endif
			}
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
