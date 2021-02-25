#include "utility.hpp"
#include "physic.hpp"
#include "level.hpp"
#include "render.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "sdl.hpp"
#include <stdlib.h>
#include <fstream>
using namespace std;

b2Body *get_body(string id) {
	for(int q=0; q<bodies.size(); q++)
		if(id==B_DATA(bodies[q],id))return bodies[q];
	throw string("\""+id+ "\" is not a body");
}
bool exist_file(std::string path) {
	bool isExist = false;
	std::ifstream fin(path);
	if(fin.is_open())
		isExist = true;
	fin.close();
	return isExist;
}
