#pragma once
#include "box2d.h"
#include "sdl.hpp"
#include "entity.hpp"
#include "weapon.hpp"
#include "interface.hpp"
#include <string>
void close_level();
void load_level(string name);
b2Body *get_body(string id);
b2Joint *get_joint(string id);
Entity *get_entity(string id);
Bullet *get_bullet(string id);
bool exist_file(string path);
string free_id();
string free_entity_id();
Entity *whois(b2Body *body);
b2Vec2 bis(b2Vec2 v1,b2Vec2 v2,float length);
b2Vec2 point2_per(b2Vec2 v1,b2Vec2 v2,float width);
float vec_angle(b2Vec2 p);
bool bigger_angle(b2Vec2 v1,b2Vec2 v2);

bool load_value(XMLNode node, const char *name,float &value);
bool load_value(XMLNode node, const char *name,bool &value);
bool load_value(XMLNode node, const char *name,ImVec2 &value);
bool load_value(XMLNode node, const char *name,b2Vec2 &value);
bool load_value(XMLNode node, const char *name,b2Transform &value);
bool load_value(XMLNode node, const char *name,b2Sweep &value);
bool load_value(XMLNode node, const char *name,Color &value);

void save_value(XMLNode node, const char *name,b2Vec2 &value);
void save_value(XMLNode node, const char *name,float &value);
void save_value(XMLNode node, const char *name,bool &value);
void save_value(XMLNode node, const char *name,b2Transform &value);
void save_value(XMLNode node, const char *name,b2Sweep &value);
void save_value(XMLNode node, const char *name,Color &value);

vector<string>list_files(string dir);
