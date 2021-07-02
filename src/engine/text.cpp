#include "text.hpp"
#include "main.hpp"
#include "xmlParser.h"
#include "utility.hpp"
#include "camera.hpp"
#include <iostream>
namespace text {
FC_Font *font;
Color tips_color;
Rect4 tips_borders;
Color subtitles_color;
Rect4 subtitles_borders;
Rect4 subtitles_position;
Tip::Tip(float x,float y,string text,Color color,float time) {
	set(x,y,text,color,time);
}
void Tip::set(float _x,float _y,string _text,Color _color,float _time) {
	pos= {_x,_y};
	text=_text;
	color=_color;
	timer=lua::get_time()+_time;
}
void Tip::draw(){
	b2Vec2 c=drawv(b2Vec2(pos));
	b2Vec2 size=b2Vec2(FC_GetWidth(font,text.c_str()),FC_GetLineHeight(font))/2.0f;
	GPU_RectangleFilled(ren,
						c.x-size.x-tips_borders.left,
						c.y-size.y-tips_borders.top,
						c.x+size.x+tips_borders.right,
						c.y+size.y+tips_borders.bottom,
						tips_color.color());
	FC_DrawColor(font,ren,c.x-size.x,c.y-size.y,color.color(),text.c_str());
}
Subtitles::Subtitles(string text,Color color,float time) {
	set(text,color,time);
}
void Subtitles::set(string _text,Color _color,float time){
	text=_text;
	color=_color;
	timer=lua::get_time()+time;
}
vector<Tip>tips;
vector<Subtitles>subtitles;
map <string,string>texts;
string get(string id) {
	if(texts.empty() || texts.find(id)==texts.end()) {
		XMLNode xml=open_xml((prefix+"locales/"+settings.language+".xml").c_str(),"text");
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
	XMLNode xml=open_xml((prefix+"locales/"+settings.language+".xml").c_str(),"text");
	for(int q=0;; q++) {
		XMLNode node=xml.getChildNode(q);
		if(node.isEmpty())return;
		texts[node.getName()]=xml.getAttribute("value");
	}
}
void load_config() {
	XMLNode node=open_xml((prefix+"config/text.xml").c_str(),"text");
	{
		XMLNode text=node.getChildNode("text");
		load_font(font,text,"default",SH);
	}
	{
		XMLNode tips_n=node.getChildNode("tips");
		load_value(tips_n,"color",tips_color);
		load_value(tips_n,"border",tips_borders);
	}

	{
		XMLNode tips_n=node.getChildNode("subtitles");
		load_value(tips_n,"color",subtitles_color);
		load_value(tips_n,"border",subtitles_borders);
		load_value(tips_n,"position",subtitles_position);
	}

	info_log("Loaded text config");
}
Color defcolor(){
	return Color(FC_GetDefaultColor(font));
}
int add_tip(lua_State *L){
	float x=luabridge::Stack<float>::get(L,1);
	float y=luabridge::Stack<float>::get(L,2);
	string text=luabridge::Stack<string>::get(L,3);
	if(lua_gettop(L)>3){
		Color color=luabridge::Stack<Color>::get(L,4);
		float time=luaL_optnumber(L, 5, 0.0f);
		tips.emplace_back(x,y,text,color,time);
	}else
		tips.emplace_back(x,y,text,FC_GetDefaultColor(font),0);
	return 0;
}
int add_subtitles(lua_State *L){
	string text=luabridge::Stack<string>::get(L,1);
	float time=luabridge::Stack<float>::get(L,2);
	if(lua_gettop(L)>2){
		Color color=luabridge::Stack<Color>::get(L,3);
		subtitles.emplace_back(text,color,time);
	}else
		subtitles.emplace_back(text,FC_GetDefaultColor(font),time);
	return 0;
}
void update() {
	for(int q=0; q<tips.size(); q++)
		if(tips[q].timer<=lua::get_time())
			tips.erase(tips.begin()+q);
	for(int q=0; q<subtitles.size(); q++)
		if(subtitles[q].timer<=lua::get_time())
			subtitles.erase(subtitles.begin(),subtitles.begin()+q);

}
void clear_text() {
	tips.clear();
}
void clear_locale() {
	texts.clear();
}
void draw() {
	for(Tip &tip : tips)
		tip.draw();
	/*float h=subtitles_borders.bottom;
	for(int q=tips.size()-1;q>=0;q--){

	}*/
}
}
