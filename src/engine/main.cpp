#include "lua.hpp"
#include "utility.hpp"
#include "physic.hpp"
#include "level.hpp"
#include "box2d.h"
#include "sdl.hpp"
#include <cstring>
#include "game.hpp"
#include <assert.h>
using namespace std;
using namespace luabridge;

int main( int argc, char * argv[] ) {
	init("Engine",1024,768);
	configure_textures();
	load_cursor();
	if(argc>1) load_level(argv[1]);
	else       load_level("default");
	game();
	quit();
	return 0;
}
