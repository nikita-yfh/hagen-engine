#pragma once
#include "box2d.h"
#include "entity.hpp"
#include "weapon.hpp"
#include <stdint.h>
#include <map>
#include <string>
struct Bullets {
};
struct Player {
	uint8_t lives=3;
	uint8_t max_lives=5;
	map<string,Bullet>bullets;
	Bullet *get_bullet(string id);
};

extern Player player;
