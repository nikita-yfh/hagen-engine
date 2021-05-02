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
	if(bodies.find(id)!=bodies.end())
		return bodies[id];
	return nullptr;
}

b2Joint *get_joint(string id) {
	if(joints.find(id)!=joints.end())
		return joints[id];
	return nullptr;
}

Entity *get_entity(string id) {
	if(entities.find(id)!=entities.end())
		return entities[id];
	return nullptr;
}

Weapon *get_weapon(string id) {
	return &weapons[id];
}

Bullet *get_bullet(string id) {
	return &bullets[id];
}

bool exist_file(string path) {
	bool isExist = false;
	ifstream fin(path);
	if(fin.is_open())
		isExist = true;
	fin.close();
	return isExist;
}
string free_id() {
	for(int q=0;; q++) {
		string id="_Body"+to_string(q);
		bool ok=1;
		for(auto bd : bodies)
			if(bd.first==id)ok=0;
		if(ok)return id;
	}
}
string free_entity_id() {
	for(int q=0;; q++) {
		string id="_Entity"+to_string(q);
		bool ok=1;
		for(auto en : entities)
			if(en.first==id)ok=0;
		if(ok)return id;
	}
}

Entity *whois(b2Body *body) {
	for(auto &e : entities)
		for(auto &b : e.second->bodies)
			if(b.second==body)
				return e.second;
	return nullptr;
}
