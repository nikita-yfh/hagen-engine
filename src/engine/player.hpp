#pragma once
#include "box2d.h"
#include "entity.hpp"
#include <stdint.h>
struct Player: public Entity {
	Player();
	uint8_t lives;
};
