#pragma once
#include "box2d.h"
#include "entity.h"
#include <stdint.h>
struct Player: public Entity {
	Player();
	uint8_t lives;
	uint8_t hp;
};
