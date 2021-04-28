#include "weapon.hpp"
#include "main.hpp"
#include "sdl.hpp"
#include "physic.hpp"
#include <fstream>
map<string,Weapon>weapons;
map<string,Bullet>bullets;
void Bullet::add(int c){
	if(count==-1)return;
	count+=c;
	if(count>max)count=max;
}
bool Bullet::full(){
	return (count==max || count==-1 || max==-1);
}

b2Body *simple_bullet(Entity *e, string type, float impulse){
	float &angle=e->weapon_angle;
	b2Body *bullet=create_body(type,"",e->getx(),e->gety());
	bullet->SetAngle(angle);
	bullet->SetLinearVelocity(e->get_first()->GetLinearVelocity());
	bullet->CenterImpulse(cos(angle)*impulse,sin(angle)*impulse);
	e->get_first()->CenterImpulse(-cos(angle)*impulse,-sin(angle)*impulse);
	return bullet;
}

