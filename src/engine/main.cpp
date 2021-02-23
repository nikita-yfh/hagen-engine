#include "lua.hpp"
#include "utility.hpp"
#include "physic.hpp"
#include "box2d.h"
#include "sdl.hpp"
#include <cstring>
#include "game.hpp"
using namespace std;
using namespace luabridge;

int main( int argc, char * argv[] ) {
	init("Engine",800,600);
	world.SetContactListener(new contactListener());
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	configure_textures();
	load_cursor();
	if(argc>1) load_level(argv[1]);
	else       load_level("default");
	game();
	quit();
	return 0;
}
