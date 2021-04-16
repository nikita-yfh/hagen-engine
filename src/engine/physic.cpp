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
bool bb_all_collide(b2Body *b1,b2Body *b2){
	for(b2ContactEdge *c=b1->GetContactList();c;c=c->next){
		if(c->other==b2 && c->contact->IsTouching())
			return 1;
	}
	return 0;
}
bool eb_all_collide(Entity *entity,b2Body *b){
	for(auto &b2 : entity->bodies){
		if(bb_all_collide(b2.second,b))
			return 1;
	}
	return 0;
}
bool ee_all_collide(Entity *e1,Entity *e2){
	for(auto &b1 : e1->bodies){
		if(eb_all_collide(e2,b1.second))
			return 1;
	}
	return 0;
}
bool lb_all_collide(b2Body *body){
	for(auto &b1 : bodies){
		if(bb_collide(b1.second,body))
			return 1;
	}
	return 0;
}
bool le_all_collide(Entity *e){
	for(auto &body : e->bodies){
		if(lb_all_collide(body.second))
			return 1;
	}
	return 0;
}

bool bb_collide(b2Body *b1,b2Body *b2){
	for(b2ContactEdge *c=b1->GetContactList();c;c=c->next){
		if(c->other==b2 && c->contact->IsTouching()
			&& !c->contact->m_fixtureA->IsSensor()
			&& !c->contact->m_fixtureB->IsSensor())
			return 1;
	}
	return 0;
}
bool eb_collide(Entity *entity,b2Body *b){
	for(auto &b2 : entity->bodies){
		if(bb_collide(b2.second,b))
			return 1;
	}
	return 0;
}
bool ee_collide(Entity *e1,Entity *e2){
	for(auto &b1 : e1->bodies){
		if(eb_collide(e2,b1.second))
			return 1;
	}
	return 0;
}
bool lb_collide(b2Body *body){
	for(auto &b1 : bodies){
		if(bb_collide(b1.second,body))
			return 1;
	}
	return 0;
}
bool le_collide(Entity *e){
	for(auto &body : e->bodies){
		if(lb_collide(body.second))
			return 1;
	}
	return 0;
}
