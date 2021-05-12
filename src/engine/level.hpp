#pragma once
#include "box2d.h"
#include "xmlParser.h"
#include "entity.hpp"
#include <string>
#include <map>
void close();
void load(string name);
void open_file(string path);
extern b2Body *read_body(XMLNode bd,b2Vec2 delta= {0,0},bool temp=0);
extern b2Joint *read_joint(XMLNode jn,string &id,b2Vec2 delta= {0,0},Entity *ent=0);
extern unsigned short int levelw;
extern unsigned short int levelh;
extern map<string,b2Body*>bodies;
extern map<string,b2Joint*>joints;
extern map<string,Entity*>entities;
extern b2World *world;
extern string background;

void save_world_state(string name);
void load_world_state(string name);
