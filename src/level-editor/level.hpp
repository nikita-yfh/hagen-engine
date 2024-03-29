#pragma once
#include "level.hpp"
#include <string>
#include <vector>
#include "object.hpp"
#include "body.hpp"
#include "joint.hpp"
#include "xmlParser.h"
struct Level {
	float w=20.0f,h=20.0f;
	std::string background="default.png";
	std::vector<Body*>bodies;
	std::vector<Joint*>joints;
	std::vector<Entity*>entities;
	bool save_file(std::string path,bool all);
	bool open_file(std::string path);
	void save_joints(XMLNode&);
	void load_joints(XMLNode&);
};
extern Level level;
