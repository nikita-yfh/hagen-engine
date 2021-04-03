#pragma once
#include "box2d.h"
#include "entity.hpp"
#include "weapon.hpp"
#include <stdint.h>
#include <map>
#include <string>
struct Player{
	Player();
	uint8_t lives;
	map<string,Bullet>bullets;
};
extern Player player;
