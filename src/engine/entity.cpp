#include "entity.hpp"
#include "level.hpp"
#include "physic.hpp"
#include "weapon.hpp"
#include "main.hpp"
#include "sdl.hpp"
#include "lua.hpp"
#include "camera.hpp"
#include "utility.hpp"
using namespace std;
void Weapon::set_texture(string tex) {
	load_texture(tex);
	texture=tex;
}
string Weapon::get_texture() const {
	return texture;
}
Entity::Entity() {};
Entity::Entity(string _type,float xp,float yp) {
	type=_type;
	XMLNode xml=open_xml((prefix+"entities/"+type+".xml").c_str(),"physics");
	{
		//bodies
		XMLNode bds=xml.getChildNode("bodies");
		int bodies_count=bds.getAttributei("count");
		bodies.clear();
		for(int q=0; q<bodies_count; q++) {
			XMLNode bd=bds.getChildNode("body",q);
			string str=bd.getAttribute("id");
			b2Body *b=read_body(bd, {xp,yp});
			bodies[str]=b;
			bodies[str]->m_userData->id=str;
		}
	}
	{
		//joints
		XMLNode js=xml.getChildNode("joints");
		int joints_count=js.getAttributei("count");
		joints.clear();
		for(int q=0; q<joints_count; q++) {
			XMLNode ch=js.getChildNode("joint",q);
			if((string)ch.getAttribute("type")!="GearJoint") {
				string str;
				b2Joint *j=read_joint(ch,str, {xp,yp},this);
				joints[str]=j;
			}
		}
		for(int q=0; q<joints_count; q++) {
			XMLNode ch=js.getChildNode("joint",q);
			if((string)ch.getAttribute("type")=="GearJoint") {
				string str;
				b2Joint *j= read_joint(ch,str, {xp,yp},this);
				joints[str]=j;
			}
		}
	}
	dx=xp-get_first()->GetPosition().x;
	dy=yp-get_first()->GetPosition().y;
}
Entity::~Entity() {
	for(auto &j : joints)
		world->DestroyJoint(j.second);
	for(auto &b : bodies)
		world->DestroyBody(b.second);
	joints.clear();
	bodies.clear();
}

b2Body *Entity::get_first() const {
	return bodies.begin()->second;
}

b2Body *Entity::get_body(string id) {
	return bodies[id];
}

b2Joint *Entity::get_joint(string id) {
	return joints[id];
}
float Entity::getx() const {
	return get_first()->GetX()+dx;
}
float Entity::gety() const {
	return get_first()->GetY()+dy;
}
void Entity::setx(float x) {
	b2Vec2 pos(x-getx(),0);
	for(auto b : bodies) {
		b.second->SetTransform(b.second->GetPosition()+pos,b.second->GetAngle());
	}
}
void Entity::sety(float y) {
	b2Vec2 pos(0,y-gety());
	for(auto b : bodies) {
		b.second->SetTransform(b.second->GetPosition()+pos,b.second->GetAngle());
	}
}

void Entity::destroy_body(string id) {
	bodies.erase(id);
}

void Entity::destroy_joint(string id) {
	joints.erase(id);
}
void Entity::set_weapon(string id) {
	weapon.bullet1=weapon.bullet2="";
	weapon.dx=0;
	weapon.dy=0;
	weapon.texture="";
	weapon.name=id;
	lua::init_weapon(this);
}
int Entity::fire1() {
	return lua::fire1(this);
}
int Entity::fire2() {
	return lua::fire2(this);
}
int Entity::fire3() {
	return lua::fire3(this);
}
int Entity::fire4() {
	return lua::fire4(this);
}
void Entity::harm(int d) {
	health-=d;
	if(health<0)health=0;
}
void Entity::heal(int d) {
	health+=d;
	if(health>max_health)health=max_health;
}

void Entity::focus(float x,float y) {
	weapon.angle=get_angle(getx()-x,gety()-y);
}

void Entity::focus_on_entity(Entity *e) {
	focus(e->getx(),e->gety());
}

void Entity::focus_on_body(b2Body *b) {
	focus(b->GetX(),b->GetY());
}

float Entity::get_vx() const {
	return get_first()->GetLinearVelocityX();
}
float Entity::get_vy() const {
	return get_first()->GetLinearVelocityY();
}

