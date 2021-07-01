#include "utility.hpp"
#include "physic.hpp"
#include "level.hpp"
#include "render.hpp"
#include "sdl.hpp"
#include <stdlib.h>
#include <fstream>
#include <cmath>
#ifdef ANDROID
#include <sys/stat.h>
#include <sys/types.h>
#include <android/log.h>
#endif
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

string get_level_name(string name) {
	XMLNode lvl=open_xml((saves+name+".xml").c_str(),"level");
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


bool load_value(XMLNode node, const char *name,Rect4 &value) {
	XMLNode value_n=node.getChildNode(name);
	if(value_n.isEmpty())return 0;
	value.load(value_n);
	return 1;
};

bool load_value(XMLNode node, const char *name,GPU_Rect &value) {
	XMLNode value_n=node.getChildNode(name);
	if(value_n.isEmpty())return 0;
	value.x=load_scaled_float(value_n,"x");
	value.y=load_scaled_float(value_n,"y");
	value.w=load_scaled_float(value_n,"w");
	value.h=load_scaled_float(value_n,"h");
	return 1;
};

bool is_valid_float(string str) {
	bool point=0;
	for(int q=0; q<str.size(); q++) {
		if(!isdigit(str[q])) {
			if(point==0&&str[q]=='.')
				point=1;
			else
				return 0;
		}
	}
	return 1;
}

float load_scaled_float(XMLNode node, const char *name) {	//грузит число из строки с дополнительными фичами
	string str=node.getAttribute(name);
	if(str.size()==0)return 0.0f;//строка не может быть пустой
	uint8_t mode1=0; //0-none,1-SW,2-SH, умножение на SW или SH, типа 0.5w
	uint8_t mode2=0; //0-none,1-SW,2-SH, вычитание числа из SW или SH, типа w-200
	char e=tolower(str[str.size()-1]);	//последний символ

	if(isdigit(e)||e=='.')mode1=0;
	else if(e=='w')mode1=1;
	else if(e=='h')mode1=2;
	else return 0.0f;

	char b=tolower(str[0]);//первый символ
	if(isdigit(b)||b=='.')mode2=0;
	else{
		if(str.size()<3)return 0.0f;
		if(b=='w')mode2=1;
		else if(b=='h')mode2=2;
		else return 0.0f;
		if(str[1]!='-')return 0.0f; //второй символ должен быть минусом
	}
	if(mode1!=0)str.pop_back();//удаляю символы после числа
	if(mode2!=0)str.erase(str.begin(),str.begin()+2);//удаляю символы перед числом

	if(!is_valid_float(str))return 0.0f;//это нормальное число?

	float f=stof(str);

	if(mode1==1)f*=SW;
	else if(mode1==2)f*=SH;
	if(mode2==1)f=SW-f;
	else if(mode2==2)f=SH-f;

	return f;
}

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
	return format("[ %.3f ]",SDL_GetTicks()/1000.0f);
}
void info_log(string text) {
	string str=get_time()+" [I] "+text;
	interface.console.AddLog(str);
	cout<<str<<endl;
#ifdef ANDROID
	__android_log_print(ANDROID_LOG_INFO, TAG, "%s\n",str.c_str());
#endif
}

void warning_log(string text) {
	string str=get_time()+" [W] "+text;
	interface.console.AddLog(str);
	cout<<str<<endl;
#ifdef ANDROID
	__android_log_print(ANDROID_LOG_WARN, TAG, "%s\n",str.c_str());
#endif
}
void error_log(string text) {
	string str=get_time()+" [E] "+text;
	interface.console.AddLog(str);
	cout<<str<<endl;
#ifdef ANDROID
	__android_log_print(ANDROID_LOG_ERROR, TAG, "%s\n",str.c_str());
#endif

}

string format(const string fmt, ...) {
	int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
	string str;
	va_list ap;
	while (1) {     // Maximum two passes on a POSIX system...
		str.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size) {  // Everything worked
			str.resize(n);
			return str;
		}
		if (n > -1)  // Needed size returned
			size = n + 1;   // For null char
		else
			size *= 2;      // Guess at a larger size (OS specific)
	}
	return str;
}
float _abs(float val) {
	if(val>0)return val;
	return -val;
}
string RWget(const char* filename) {
	SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
	if (rw == NULL) {
		warning_log(format("Cannot open %s",filename));
		return "";
	}
	string str;

	Sint64 res_size = SDL_RWsize(rw);
	char* res = new char[res_size+1];

	Sint64 nb_read_total = 0, nb_read = 1;
	char* buf = res;
	while (nb_read_total < res_size && nb_read != 0) {
		nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
		nb_read_total += nb_read;
		buf += nb_read;
	}
	SDL_RWclose(rw);
	if (nb_read_total != res_size) {
		delete res;
		return "";
	}
	res[nb_read_total] = '\0';
	str=res;
	delete res;
	int u[3]= {0xEF,0xBB,0xBF};
	for(int q=0; q<3; q++) {
		if(str[0]==(char)u[q])
			str.erase(str.begin());
		else
			break;
	}
	return str;
}
XMLNode open_xml(const char *path, const char *tag) {
	XMLResults res;
	string str=RWget(path);
	if(str=="")
		return XMLNode::emptyNode();
	XMLNode node;
#ifdef ANDROID
	node=XMLNode::parseString(str.c_str(),((string)tag+"_android").c_str(),&res);
	if(res.error==XMLError::eXMLErrorNone)
		return node;
#endif
	node=XMLNode::parseString(str.c_str(),tag,&res);
	if(res.error!=XMLError::eXMLErrorNone)
		panic(format("%s: %s, in line %d, column %d",XMLNode::getError(res.error),path,res.nLine,res.nColumn));
	return node;
}

bool in_rect(float x,float y,GPU_Rect r) {
	return r.x<x && r.y<y && r.x+r.w>x && r.y+r.h>y;
}
float deg(float v){
	return v/M_PI*180.0f;
}

float rad(float v){
	return v*M_PI/180.0f;
}
