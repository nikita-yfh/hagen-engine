#include "utility.hpp"
#include "physic.hpp"
#include "level.hpp"
#include "render.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "sdl.hpp"
#include <stdlib.h>
#include <fstream>
#include <cmath>
using namespace std;

b2Body *get_body(string id) {
	if(bodies.find(id)!=bodies.end())
		return bodies[id];
	return nullptr;
}

b2Joint *get_joint(string id) {
	if(joints.find(id)!=joints.end())
		return joints[id];
	return nullptr;
}

Entity *get_entity(string id) {
	if(entities.find(id)!=entities.end())
		return entities[id];
	return nullptr;
}

Bullet *get_bullet(string id) {
	return &bullets[id];
}

bool exist_file(string path) {
	bool isExist = false;
	ifstream fin(path);
	if(fin.is_open())
		isExist = true;
	fin.close();
	return isExist;
}
string free_id() {
	for(int q=0;; q++) {
		string id="_Body"+to_string(q);
		bool ok=1;
		for(auto bd : bodies)
			if(bd.first==id)ok=0;
		if(ok)return id;
	}
}
string free_entity_id() {
	for(int q=0;; q++) {
		string id="_Entity"+to_string(q);
		bool ok=1;
		for(auto en : entities)
			if(en.first==id)ok=0;
		if(ok)return id;
	}
}

Entity *whois(b2Body *body) {
	for(auto &e : entities)
		for(auto &b : e.second->bodies)
			if(b.second==body)
				return e.second;
	return nullptr;
}
float vec_angle(b2Vec2 p) {
	if(p.x>=0)
		return M_PI+atan(p.y/p.x);
	else if(p.y>0)
		return 2*M_PI+atan(p.y/p.x);
	else
		return atan(p.y/p.x);
}
b2Vec2 bis(b2Vec2 v1,b2Vec2 v2,float length) {
	v1.Normalize();
	if(v1.Length()==0) {
		v2=v2.Skew();
		v2.Normalize();
		return length*v2;
	}
	v2.Normalize();
	if(v1==-v2)
		v1+=0.001*v1.Skew();
	b2Vec2 sum=v1+v2;
	sum.Normalize();
	if(v1.x*v2.y-v1.y*v2.x<0)
		sum=-sum;
	float angle=vec_angle(v1)-vec_angle(v2)-M_PI;
	sum*=abs(length/cos(angle/2));

	return sum;
}
b2Vec2 point2_per(b2Vec2 v1,b2Vec2 v2,float width) {
	b2Vec2 d=v2-v1;
	b2Vec2 per=d.Skew();
	per.Normalize();
	per*=width;
	return per;
}
bool bigger_angle(b2Vec2 v1,b2Vec2 v2) {
	return v1.x*v2.y>v1.y*v2.x;
}

string get_level_name(string name){
	XMLNode lvl=XMLNode::openFileHelper((saves+name+".xml").c_str(),"level");
	return lvl.getAttribute("name");
}

bool load_value(XMLNode node, const char *name,float &value) {
	XMLNode value_n=node.getChildNode(name);
	if(value_n.isEmpty())return 0;
	value=value_n.getAttributef("value");
	return 1;
};
bool load_value(XMLNode node, const char *name,bool &value) {
	XMLNode value_n=node.getChildNode(name);
	if(value_n.isEmpty())return 0;
	value=value_n.getAttributei("value");
	return 1;
};
bool load_value(XMLNode node, const char *name,ImVec2 &value) {
	XMLNode value_n=node.getChildNode(name);
	if(value_n.isEmpty())return 0;
	value.x=value_n.getAttributef("x");
	value.y=value_n.getAttributef("y");
	return 1;
};


bool load_value(XMLNode node, const char *name,b2Vec2 &value) {
	XMLNode value_n=node.getChildNode(name);
	if(value_n.isEmpty())return 0;
	value.x=value_n.getAttributef("x");
	value.y=value_n.getAttributef("y");
	return 1;
};
bool load_value(XMLNode node, const char *name,b2Transform &value) {
	XMLNode value_n=node.getChildNode(name);
	if(value_n.isEmpty())return 0;
	value.p.x=value_n.getAttributef("x");
	value.p.y=value_n.getAttributef("y");
	value.q.c=value_n.getAttributef("cos");
	value.q.s=value_n.getAttributef("sin");
	return 1;
};
bool load_value(XMLNode node, const char *name,b2Sweep &value) {
	XMLNode value_n=node.getChildNode(name);
	if(value_n.isEmpty())return 0;
	value.a=value_n.getAttributef("a1");
	value.a0=value_n.getAttributef("a0");
	value.c0.x=value_n.getAttributef("c0x");
	value.c0.y=value_n.getAttributef("c0y");
	value.c.x=value_n.getAttributef("c1x");
	value.c.y=value_n.getAttributef("c1y");
	return 1;
};

bool load_value(XMLNode node, const char *name,Color &value) {
	XMLNode value_n=node.getChildNode(name);
	if(value_n.isEmpty())return 0;
	value.r=value_n.getAttributef("r");
	value.g=value_n.getAttributef("g");
	value.b=value_n.getAttributef("b");
	value.a=value_n.getAttributef("a");
	return 1;
};


void save_value(XMLNode node, const char *name,b2Vec2 &value) {
	XMLNode value_n=node.addChild(name);
	value_n.addAttribute("x",value.x);
	value_n.addAttribute("y",value.y);
};

void save_value(XMLNode node, const char *name,float &value) {
	XMLNode value_n=node.addChild(name);
	value_n.addAttribute("value",value);
};

void save_value(XMLNode node, const char *name,bool &value) {
	XMLNode value_n=node.addChild(name);
	value_n.addAttribute("value",value);
};

void save_value(XMLNode node, const char *name,b2Transform &value) {
	XMLNode value_n=node.addChild(name);
	value_n.addAttribute("x",value.p.x);
	value_n.addAttribute("y",value.p.y);
	value_n.addAttribute("sin",value.q.s);
	value_n.addAttribute("cos",value.q.c);
};

void save_value(XMLNode node, const char *name,b2Sweep &value) {
	XMLNode value_n=node.addChild(name);
	value_n.addAttribute("a0",value.a0);
	value_n.addAttribute("a1",value.a);
	value_n.addAttribute("c0x",value.c0.x);
	value_n.addAttribute("c0y",value.c0.y);
	value_n.addAttribute("c1x",value.c.x);
	value_n.addAttribute("c1y",value.c.y);
};

void save_value(XMLNode node, const char *name,Color &value) {
	XMLNode value_n=node.addChild(name);
	value_n.addAttribute("r",value.r);
	value_n.addAttribute("g",value.g);
	value_n.addAttribute("b",value.b);
	value_n.addAttribute("a",value.a);
};
vector<string>list_files(string dir) {
	vector<string>list;
#ifdef WIN32
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile((dir+"/*").c_str(), &fd);
	if(hFind != INVALID_HANDLE_VALUE) {
		do {
			if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))list.push_back(fd.cFileName);
		} while(::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
}
#else
	char var[128];
	FILE *fp = popen(string("ls "+dir).c_str(),"r");
	while (fgets(var, sizeof(var), fp) != NULL) {
		list.push_back(string(var));
		list[list.size()-1].pop_back();
	}
	pclose(fp);
#endif
return list;
}
string get_time() {
	int ms=SDL_GetTicks()%1000;
	int sec=SDL_GetTicks()/1000;

	return format("[ %d.%d ]",sec,ms);
}
void info_log(string text) {
	interface.console.AddLog(get_time()+" [I] "+text);
	cout<<get_time()<<" [I] "<<text<<endl;
}

void warning_log(string text) {
	interface.console.AddLog(get_time()+" [W] "+text);
	cout<<get_time()<<" [W] "<<text<<endl;
}
void error_log(string text) {
	interface.console.AddLog(get_time()+" [E] "+text);
	cout<<get_time()<<" [E] "<<text<<endl;
}
