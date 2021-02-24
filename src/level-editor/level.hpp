#pragma once
#include "level.hpp"
#include <string>
#include <vector>
#include "object.hpp"
#include "body.hpp"
#include "joint.hpp"
#include "xmlParser.h"
struct Level {
	unsigned short int w=20,h=20;
	std::string background="default.png";
	std::vector<Body*>bodies;
	std::vector<Joint*>joints;
	Point start= {3,3,"_start"};
	Point end{5,3,"_end"};
	bool save_file(std::string path);
	bool open_file(std::string path);
	void save_joints(XMLNode&);
	void load_joints(XMLNode&);
};
extern Level level;
