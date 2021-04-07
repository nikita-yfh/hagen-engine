#pragma once
#include "box2d.h"
#include "SDL_gpu.h"
#include "entity.hpp"
#include <string>
void close_level();
void load_level(string name);
b2Body *get_body(string id);
b2Joint *get_joint(string id);
Weapon *get_weapon(string id);
Entity *get_entity(string id);
bool exist_file(string path);
string free_id();
string free_entity_id();
