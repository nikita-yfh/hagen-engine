#include "entity.hpp"
#include "level.hpp"
#include "physic.hpp"
#include "main.hpp"
#include "sdl.hpp"
#include "lua.hpp"
using namespace std;
Entity::Entity() {};
Entity::Entity(string _type,float xp,float yp) {
	type=_type;
	XMLNode xml=XMLNode::openFileHelper((prefix+"entities/"+type+".xml").c_str(),"physics");
	{
		//bodies
		XMLNode bds=xml.getChildNode("bodies");
		int bodies_count=stoi(bds.getAttribute("count"));
		bodies.clear();
		for(int q=0; q<bodies_count; q++) {
			XMLNode bd=bds.getChildNode("body",q);
			string str=bd.getAttribute("id");
			b2Body *b=read_body(bd,{xp,yp});
			bodies[str]=b;
		}
	}
	{
		//joints
		XMLNode js=xml.getChildNode("joints");
		int joints_count=stoi(js.getAttribute("count"));
		joints.clear();
		for(int q=0; q<joints_count; q++) {
			XMLNode ch=js.getChildNode("joint",q);
			if((string)ch.getAttribute("type")!="GearJoint"){
				string str;
				b2Joint *j=read_joint(ch,str,{xp,yp},this);
				joints[str]=j;
			}
		}
		for(int q=0; q<joints_count; q++) {
			XMLNode ch=js.getChildNode("joint",q);
			if((string)ch.getAttribute("type")=="GearJoint"){
				string str;
				b2Joint *j=read_joint(ch,str,{xp,yp},this);
				joints[str]=j;
			}
		}
	}
	dx=xp-get_first()->GetPosition().x;
	dy=yp-get_first()->GetPosition().y;
}
Entity::~Entity(){}

b2Body *Entity::get_first() const{
	return bodies.begin()->second;
}

b2Body *Entity::get_body(string id) {
	return bodies[id];
}

b2Joint *Entity::get_joint(string id) {
	return joints[id];
}
float Entity::getx() const{
	return get_first()->GetX()+dx;
}
float Entity::gety() const{
	return get_first()->GetY()+dy;
}
void Entity::setx(float x){
	b2Vec2 pos(x-getx(),0);
	for(auto b : bodies){
		b.second->SetTransform(b.second->GetPosition()+pos,b.second->GetAngle());
	}
}
void Entity::sety(float y){
	b2Vec2 pos(0,y-gety());
	for(auto b : bodies){
		b.second->SetTransform(b.second->GetPosition()+pos,b.second->GetAngle());
	}
}

void Entity::destroy_body(string id){
	bodies.erase(id);
}

void Entity::destroy_joint(string id){
	joints.erase(id);
}
void Entity::set_weapon(string id){
	if(weapons.find(id)==weapons.end()){
		weapons[id]=Weapon();
		lua::init_weapon(id);
	}
	load_texture(id+".png");
	weapon=id;
}
string Entity::get_weapon() const{
	return weapon;
}
int Entity::fire1(){return lua::fire1(this);}
int Entity::fire2(){return lua::fire2(this);}
