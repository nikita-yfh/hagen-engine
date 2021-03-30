#include "physic.hpp"
#include "level.hpp"
#include "utility.hpp"
#include "sdl.hpp"
#include "main.hpp"
using namespace std;
b2Body *create_body(string type,string id,float x,float y){
	XMLNode bd=XMLNode::openFileHelper((prefix+"templates/"+type+".xml").c_str(),"body");
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
bool collide(b2Body *b1,b2Body *b2){
	for(b2ContactEdge *c=b1->GetContactList();c;c=c->next){
		if(c->other==b2 && c->contact->IsTouching())
			return 1;
	}
	return 0;
}
bool entity_collide(Entity *entity,b2Body *b){
	for(auto &b2 : entity->bodies){
		if(collide(b2.second,b))
			return 1;
	}
	return 0;
}
bool ee_collide(Entity *e1,Entity *e2){
	for(auto &b1 : e1->bodies){
		if(entity_collide(e2,b1.second))
			return 1;
	}
	return 0;
}
bool level_collide(b2Body *body){
	for(auto &b1 : bodies){
		if(collide(b1.second,body))
			return 1;
	}
	return 0;
}
bool level_entity_collide(Entity *e){
	for(auto &body : e->bodies){
		if(level_collide(body.second))
			return 1;
	}
	return 0;
}
