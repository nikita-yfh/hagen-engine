#include "text.hpp"
#include "main.hpp"
#include "xmlParser.h"
#include "utility.hpp"
#include "camera.hpp"
#include <iostream>
namespace text{
FC_Font *font;
Color tips_color;
Rect4 tips_borders;
Tip::Tip(float _x,float _y,string _text,Color _color){
	pos={_x,_y};
	text=_text;
	color=_color;
}
Tip::Tip(float _x,float _y,string _text){
	pos={_x,_y};
	text=_text;
	color=FC_GetDefaultColor(font);
}
vector<Tip>tips;
map <string,string>texts;
string get(string id) {
	if(texts.empty() || texts.find(id)==texts.end()) {
		XMLNode xml=XMLNode::openFileHelper((prefix+"locales/"+settings.language+".xml").c_str(),"text");
		string id1=id;
		while(id1.find("/")!=id1.npos) {
			string s(id,0,id1.find("/"));
			id1.erase(id1.begin(),id1.begin()+id1.find("/")+1);
			xml=xml.getChildNode(s.c_str());
		}
		xml=xml.getChildNode(id1.c_str());
		if(!(xml.isEmpty()))
			texts[id]=xml.getAttribute("value");
		else
			warning_log("String "+id+" not found in locale file "+settings.language+".xml");
	}
	return texts[id];
}
const char* getc(string id) {
	get(id);
	return texts[id].c_str();
}
void preload() {
	XMLNode xml=XMLNode::openFileHelper((prefix+"locales/"+settings.language+".xml").c_str(),"text");
	for(int q=0;; q++) {
		XMLNode node=xml.getChildNode(q);
		if(node.isEmpty())return;
		texts[node.getName()]=xml.getAttribute("value");
	}
}
void load_config(){
	XMLNode node=XMLNode::openFileHelper((prefix+"config/text.xml").c_str(),"text");
	{
		XMLNode text=node.getChildNode("text");
		load_font(font,text,"default",SH);
	}
	{
		XMLNode tips_n=node.getChildNode("tips");
		load_value(tips_n,"color",tips_color);
		load_value(tips_n,"border",tips_borders);
	}

	info_log("Loaded text config");
}
void add_tip_color(float x,float y,string text,Color color){
	tips.emplace_back(x,y,text,color);
}
void add_tip(float x,float y,string text){
	tips.emplace_back(x,y,text);
}
void clear_text(){
	tips.clear();
}
void clear_locale() {
	texts.clear();
}
void draw(){
	for(Tip &tip : tips){
		b2Vec2 c=drawv(b2Vec2(tip.pos));
		b2Vec2 size=b2Vec2(FC_GetWidth(font,tip.text.c_str()),FC_GetLineHeight(font))/2.0f;
		GPU_RectangleFilled(ren,
							c.x-size.x-tips_borders.left,
							c.y-size.y-tips_borders.top,
							c.x+size.x+tips_borders.right,
							c.y+size.y+tips_borders.bottom,
						tips_color.color());
		FC_DrawColor(font,ren,c.x-size.x,c.y-size.y,tip.color.color(),tip.text.c_str());
	}
}
}
