#include "utility.hpp"
#include "physic.hpp"
#include "level.hpp"
#include "render.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "sdl.hpp"
#include <stdlib.h>
#include <fstream>
#include <cmath>
using namespace std;

b2Body *get_body(string id) {
	if(bodies.find(id)!=bodies.end())
		return bodies[id];
	return nullptr;
}

b2Joint *get_joint(string id) {
	if(joints.find(id)!=joints.end())
		return joints[id];
	return nullptr;
}

Entity *get_entity(string id) {
	if(entities.find(id)!=entities.end())
		return entities[id];
	return nullptr;
}

Weapon *get_weapon(string id) {
	return &weapons[id];
}

Bullet *get_bullet(string id) {
	return &bullets[id];
}

bool exist_file(string path) {
	bool isExist = false;
	ifstream fin(path);
	if(fin.is_open())
		isExist = true;
	fin.close();
	return isExist;
}
string free_id() {
	for(int q=0;; q++) {
		string id="_Body"+to_string(q);
		bool ok=1;
		for(auto bd : bodies)
			if(bd.first==id)ok=0;
		if(ok)return id;
	}
}
string free_entity_id() {
	for(int q=0;; q++) {
		string id="_Entity"+to_string(q);
		bool ok=1;
		for(auto en : entities)
			if(en.first==id)ok=0;
		if(ok)return id;
	}
}

Entity *whois(b2Body *body) {
	for(auto &e : entities)
		for(auto &b : e.second->bodies)
			if(b.second==body)
				return e.second;
	return nullptr;
}
float vec_angle(b2Vec2 p) {
	if(p.x>=0)
		return M_PI+atan(p.y/p.x);
	else if(p.y>0)
		return 2*M_PI+atan(p.y/p.x);
	else
		return atan(p.y/p.x);
}
b2Vec2 bis(b2Vec2 v1,b2Vec2 v2,float length) {
	v1.Normalize();
	if(v1.Length()==0){
		v2=v2.Skew();
		v2.Normalize();
		return length*v2;
	}
	v2.Normalize();
	if(v1==-v2)
		v1+=0.001*v1.Skew();
	b2Vec2 sum=v1+v2;
	sum.Normalize();
	if(v1.x*v2.y-v1.y*v2.x<0)
		sum=-sum;
	float angle=vec_angle(v1)-vec_angle(v2)-M_PI;
	sum*=abs(length/cos(angle/2));

	return sum;
}
b2Vec2 point2_per(b2Vec2 v1,b2Vec2 v2,float width){
	b2Vec2 d=v2-v1;
	b2Vec2 per=d.Skew();
	per.Normalize();
	per*=width;
	return per;
}
bool bigger_angle(b2Vec2 v1,b2Vec2 v2){
	return v1.x*v2.y>v1.y*v2.x;
}
