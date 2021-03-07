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

b2Joint *get_joint(string id) {
	for(int q=0; q<joints.size(); q++)
		if(id==J_DATA(joints[q],id))return joints[q];
	throw string("\""+id+ "\" is not a joint");
}

Entity &get_entity(string id) {
	for(int q=0; q<entities.size(); q++)
		if(id==entities[q].id)return entities[q];
	throw string("\""+id+ "\" is not a entity");
}
bool exist_file(std::string path) {
	bool isExist = false;
	std::ifstream fin(path);
	if(fin.is_open())
		isExist = true;
	fin.close();
	return isExist;
}
