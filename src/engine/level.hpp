#pragma once
#include "box2d.h"
#include "xmlParser.h"
#include <string>
#include <vector>
void close_level();
void load_level(std::string name);
void open_file(std::string path);
extern b2Body *read_body(XMLNode bd);
extern b2Joint *read_joint(XMLNode jn);
extern unsigned short int levelw;
extern unsigned short int levelh;
extern std::vector<b2Body*>bodies;
extern std::vector<b2Joint*>joints;
extern b2World world;
extern float startx,starty,endx,endy;
