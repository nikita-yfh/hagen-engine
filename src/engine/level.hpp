#pragma once
#include "box2d.h"
#include "xmlParser.h"
#include "entity.hpp"
#include <string>
#include <map>
void close_level();
void load_level(std::string name);
void open_file(std::string path);
extern b2Body *read_body(XMLNode bd,std::string &id,b2Vec2 delta={0,0});
extern b2Joint *read_joint(XMLNode jn,std::string &id,b2Vec2 delta={0,0},Entity *ent=0);
extern unsigned short int levelw;
extern unsigned short int levelh;
extern std::map<std::string,b2Body*>bodies;
extern std::map<std::string,b2Joint*>joints;
extern std::map<std::string,Entity*>entities;
extern b2World world;
