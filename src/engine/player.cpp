#include "player.hpp"
Player player;
Bullet *Player::get_bullet(string id) {
	return &bullets[id];
}
