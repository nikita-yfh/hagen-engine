#include "effect.hpp"
#include "main.hpp"
#include "xmlParser.h"
#include "lua.hpp"
namespace effect {
Effect::Effect(string name) {
	load(name);
}
void Effect::load(string name) {
	XMLNode node=XMLNode::openFileHelper((prefix+"effects/"+name+".xml").c_str(),"effect");
	period=stoi(node.getAttribute("period"));
	anim.resize(stoi(node.getAttribute("count")));
	for(int q=0; q<anim.size(); q++) {
		XMLNode tex_n=node.getChildNode("texture",q);
		anim[q]=load_texture(tex_n.getAttribute("name"));
	}
}

ShowEffect::ShowEffect(Effect *e,float _x,float _y) {
	x=_x;
	y=_y;
	effect=e;
	begin_time=lua::get_time();
}

bool ShowEffect::del() {
	return lua::get_time()>begin_time+effect->anim.size()*effect->period;
}

void load(string name) {
	if(loaded.find(name)!=loaded.end())
		return;
	Effect e(name);
	loaded[name]=e;
}

void create(string name,float x,float y) {
	load(name);
	effects.emplace_back(&loaded[name],x,y);
}

void update() {
	for(int q=0; q<effects.size(); q++) {
		if(effects[q].del())
			effects.erase(effects.begin()+q);
	}
}

map<string,Effect>loaded;
vector<ShowEffect>effects;
};
