#pragma once
#include "box2d.h"
#include "SDL_gpu.h"
#include <string>
void close_level();
void load_level(std::string name);
b2Body *get_body(std::string id);
bool exist_file(std::string path);
