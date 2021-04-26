#include "physic.hpp"
#include "level.hpp"
#include "utility.hpp"
#include "sdl.hpp"
#include "main.hpp"
#include "lua.hpp"
float time_scale=1.0f;
int velocity_iterations=10;
int position_iterations=10;
using namespace std;
b2Body *create_body(string type,string id,float x,float y) {
	XMLNode bd=XMLNode::openFileHelper((prefix+"templates/"+type+".xml").c_str(),"body");
	b2Body *body=read_body(bd, {x,y},1);
	load_body_textures(body);
	if(id=="")id=free_id();
	bodies[id]=body;
	B_DATA(body,id)=id;
	lua::init_body(body);
	return body;
}
Entity *create_entity(string type,string id,float x,float y) {
	Entity *ent=new Entity(type,x,y);
	load_entity_textures(ent);
	if(id=="")id=free_entity_id();
	entities[id]=ent;
	ent->id=id;
	return ent;
}
void set_gravity(float x,float y) {
	world->SetGravity({x,y});
}
void destroy_body(b2Body *body) {
	for(auto &b : bodies){
		if(b.second==body){
			bodies.erase(b.first);
			world->DestroyBody(body);
			return;
		}
	}
}

void destroy_joint(b2Joint *joint) {
	for(auto &j : joints){
		if(j.second==joint){
			joints.erase(j.first);
			world->DestroyJoint(joint);
			return;
		}
	}
}

void destroy_entity(Entity *entity) {
	for(auto &e : entities){
		if(e.second==entity){
			entities.erase(e.first);
			return;
		}
	}
}
bool bb_all_collide(b2Body *b1,b2Body *b2) {
	for(b2ContactEdge *c=b1->GetContactList(); c; c=c->next) {
		if(c->other==b2 && c->contact->IsTouching())
			return true;
	}
	return false;
}
b2Body *eb_all_collide(Entity *entity,b2Body *b) {
	for(auto &b2 : entity->bodies) {
		if(bb_all_collide(b2.second,b))
			return b2.second;
	}
	return nullptr;
}
bool ee_all_collide(Entity *e1,Entity *e2) {
	for(auto &b1 : e1->bodies) {
		if(eb_all_collide(e2,b1.second))
			return true;
	}
	return false;
}
b2Body *lb_all_collide(b2Body *body) {
	for(auto &b1 : bodies) {
		if(bb_all_collide(b1.second,body))
			return b1.second;
	}
	return nullptr;
}
b2Body *le_all_collide(Entity *e) {
	for(auto &body : e->bodies) {
		if(lb_all_collide(body.second))
			return body.second;
	}
	return nullptr;
}

Entity *sb_all_collide(b2Body *body) {
	for(auto &e : entities) {
		if(eb_all_collide(e.second,body))
			return e.second;
	}
	return nullptr;
}

bool bb_collide(b2Body *b1,b2Body *b2) {
	for(b2ContactEdge *c=b1->GetContactList(); c; c=c->next) {
		if(c->other==b2 && c->contact->IsTouching()
				&& !c->contact->m_fixtureA->IsSensor()
				&& !c->contact->m_fixtureB->IsSensor()
				&& c->contact->IsEnabled())
			return true;
	}
	return false;
}
b2Body *eb_collide(Entity *entity,b2Body *b) {
	for(auto &b2 : entity->bodies) {
		if(bb_collide(b2.second,b))
			return b2.second;
	}
	return nullptr;
}
bool ee_collide(Entity *e1,Entity *e2) {
	for(auto &b1 : e1->bodies) {
		if(eb_collide(e2,b1.second))
			return true;
	}
	return false;
}
b2Body *lb_collide(b2Body *body) {
	for(auto &b1 : bodies) {
		if(bb_collide(b1.second,body))
			return b1.second;
	}
	return nullptr;
}
b2Body *le_collide(Entity *e) {
	for(auto &body : e->bodies) {
		if(lb_collide(body.second))
			return body.second;
	}
	return nullptr;
}
Entity *sb_collide(b2Body *body) {
	for(auto &e : entities) {
		if(eb_collide(e.second,body))
			return e.second;
	}
	return nullptr;
}
