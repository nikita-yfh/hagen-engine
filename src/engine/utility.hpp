#pragma once
#include "box2d.h"
#include "SDL_gpu.h"
#include "entity.hpp"
#include <string>
void close_level();
void load_level(std::string name);
b2Body *get_body(std::string id);
b2Joint *get_joint(std::string id);
Entity *get_entity(std::string id);
bool exist_file(std::string path);
