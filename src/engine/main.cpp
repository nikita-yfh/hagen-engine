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
using namespace luabridge;

int main(int argc, char * argv[]) {
	try {
		if(argc>2)prefix=argv[2];
		init("Engine",1024,768);
		configure_textures();
		load_cursor();
		if(argc>1) load_level(argv[1]);
		else       load_level("default");

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
	}
}
