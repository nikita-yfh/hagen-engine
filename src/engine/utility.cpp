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
	return bodies[id];
}

b2Joint *get_joint(string id) {
	return joints[id];
}

Entity *get_entity(string id) {
	return entities[id];
}
bool exist_file(std::string path) {
	bool isExist = false;
	std::ifstream fin(path);
	if(fin.is_open())
		isExist = true;
	fin.close();
	return isExist;
}
string free_id(){
	for(int q=0;;q++){
		string id="_Body"+to_string(q);
		bool ok=1;
		for(auto bd : bodies){	
			if(bd.first==id)ok=0;
		}
		if(ok)return id;
	}
}
