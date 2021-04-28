#include "effect.hpp"
#include "main.hpp"
#include "xmlParser.h"
#include "lua.hpp"
Effect::Effect(string name){
	load(name);
}
void Effect::load(string name){
	XMLNode node=XMLNode::openFileHelper((prefix+"effects/"+name+".xml").c_str(),"effect");
	period=stoi(node.getAttribute("period"));
	anim.resize(stoi(node.getAttribute("count")));
	for(int q=0;q<anim.size();q++){
		XMLNode tex_n=node.getChildNode("texture",q);
		anim[q]=load_texture(tex_n.getAttribute("name"));
	}
}

ShowEffect::ShowEffect(Effect *e,float _x,float _y){
	x=_x;
	y=_y;
	effect=e;
	begin_time=lua::get_time();
}

bool ShowEffect::del(){
	return lua::get_time()>begin_time+effect->anim.size()*effect->period;
}

void load_effect(string name){
	if(loaded_effects.find(name)!=loaded_effects.end())
		return;
	Effect e(name);
	loaded_effects[name]=e;
}

void create_effect(string name,float x,float y){
	load_effect(name);
	effects.emplace_back(&loaded_effects[name],x,y);
}

void update_effects(){
	for(int q=0;q<effects.size();q++){
		if(effects[q].del())
			effects.erase(effects.begin()+q);
	}
}

map<string,Effect>loaded_effects;
vector<ShowEffect>effects;
