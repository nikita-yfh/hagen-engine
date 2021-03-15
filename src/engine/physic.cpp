#include "physic.hpp"
#include "level.hpp"
#include "utility.hpp"
#include "sdl.hpp"
using namespace std;
b2Body *create_body(string type,string id,float x,float y){
	XMLNode bd=XMLNode::openFileHelper(("templates/"+type+".xml").c_str(),"body");
	b2Body *body=read_body(bd,{x,y},1);
	load_body_textures(body);
	if(id=="")id=free_id();
	bodies[id]=body;
	return body;
}
Entity *create_entity(string type,string id,float x,float y){
	Entity *ent=new Entity(type,x,y);
	load_entity_textures(ent);
	if(id=="")id=free_entity_id();
	entities[id]=ent;
	return ent;
}
void set_gravity(float x,float y){
	world.SetGravity({x,y});
}
void destroy_body(string id){
	bodies.erase(id);
}

void destroy_joint(string id){
	joints.erase(id);
}

void destroy_entity(string id){
	entities.erase(id);
}
