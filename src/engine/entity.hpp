#pragma once
#include "box2d.h"
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge.h"
#include <vector>
#include <string>
struct Entity {
	Entity();
	Entity(std::string type,std::string id,float x,float y);
	~Entity();
	float health;
	b2Vec2 p;
	std::string id;
	std::string type;
	std::vector<b2Body*>bodies;
	std::vector<b2Joint*>joints;
};
