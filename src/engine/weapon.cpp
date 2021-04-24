#include "weapon.hpp"
#include "main.hpp"
#include "sdl.hpp"
#include <fstream>
unordered_map<string,Weapon>weapons;
void Bullet::add(int c){
	if(count==-1)return;
	count+=c;
	if(count>max)count=max;
}
