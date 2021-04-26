#include "weapon.hpp"
#include "main.hpp"
#include "sdl.hpp"
#include <fstream>
map<string,Weapon>weapons;
map<string,Bullet>bullets;
void Bullet::add(int c){
	if(count==-1)return;
	count+=c;
	if(count>max)count=max;
}
