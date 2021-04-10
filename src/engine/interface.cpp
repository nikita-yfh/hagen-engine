#include "interface.hpp"
#include "sdl.hpp"
#include "xmlParser.h"
#include "main.hpp"
#include "player.hpp"
#include "lua.hpp"
void Interface::update(){
	console.update();
	game_interface.update();
}
void Interface::load_config(){
	console.load_config();
	game_interface.load_config();
}
void Interface::show(){
	console.show();
	if(!console.shown)
		game_interface.show();
}
Interface::Console::String::String(){}
Interface::Console::String::String(string _text,uint8_t _type){
	text=_text;
	type=_type;
}
void Interface::Console::String::add(short pos,char c){
	text.insert(text.begin()+pos,c);
}
void Interface::Console::String::del(short pos){
	if(pos>2 && pos<=text.size() && text.size())
		text.erase(text.begin()+pos-1);
}
Interface::Console::Console(){
	strings.emplace_back((string)LUA_VERSION+" Copyright (C) 1994-2015 Lua.org, PUC-Rio",2);
	strings.emplace_back("> ",0);
}
void Interface::Console::show(){
	if(!shown)return;
	GPU_RectangleFilled(ren,0,0,SW,SH,background.color());
	int size=FC_GetLineHeight(font);
	int pos=10;
	for(String s : strings){
		Color *color;
		if(s.type==0)color=&input_color;
		else if(s.type==1)color=&error_color;
		else color=&output_color;
		pos+=FC_DrawColumnColor(font,ren,10,pos,SW-20,color->color(),s.text.c_str()).h;
	}
	if(pos>SH)
		strings.erase(strings.begin());
}
void Interface::Console::load_config(){
	XMLNode node=XMLNode::openFileHelper((prefix+"config/console.xml").c_str(),"console");
	background.load(node.getChildNode("background").getChildNode("color"));
	{
		XMLNode text=node.getChildNode("text");
		input_color.load(text.getChildNode("input_color"));
		error_color.load(text.getChildNode("error_color"));
		output_color.load(text.getChildNode("output_color"));
		load_font(font,text,"input_color");
	}
}
void Interface::Game_interface::load_config(){
	XMLNode node=XMLNode::openFileHelper((prefix+"config/interface.xml").c_str(),"interface");
	{
		XMLNode text=node.getChildNode("text");
		load_font(font,text,"color");
	}
	{
		XMLNode l=node.getChildNode("lives_counter");
		{
			XMLNode pos=l.getChildNode("position");
			lives_counter.x=stof(pos.getAttribute("dx"));
			lives_counter.y=stof(pos.getAttribute("dy"));
			if(lives_counter.x<1)lives_counter.x*=SH;
			if(lives_counter.y<1)lives_counter.y*=SH;
		}
		{
			XMLNode size=l.getChildNode("size");
			lives_counter.w=stof(size.getAttribute("x"));
			lives_counter.h=stof(size.getAttribute("y"));
			if(lives_counter.w<1)lives_counter.w*=SH;
			if(lives_counter.h<1)lives_counter.h*=SH;
		}
	}
	{
		XMLNode l=node.getChildNode("health_counter");
		{
			XMLNode pos=l.getChildNode("position");
			health_counter_dx=stof(pos.getAttribute("dx"));
			health_counter_dy=stof(pos.getAttribute("dy"));
			if(health_counter_dx<1)health_counter_dx*=SH;
			if(health_counter_dy<1)health_counter_dy*=SH;
		}
	}
}
void Interface::Console::update(){
	if(e.type==SDL_KEYDOWN){
		if(e.key.keysym.sym==SDLK_BACKQUOTE)
			if(shown)close();
			else open();
		else if(shown){
			switch(e.key.keysym.sym){
			case SDLK_BACKSPACE:
				strings[strings.size()-1].del(cursor--);
				break;
			case SDLK_DELETE:
				strings[strings.size()-1].del(cursor+1);
				break;
			case SDLK_LEFT:
				cursor--;
				break;
			case SDLK_RIGHT:
				cursor++;
				break;
			case SDLK_RETURN:{
				string str=strings[strings.size()-1].text;
				str.erase(str.begin(),str.begin()+2);
				if(luaL_dostring(lua::L,str.c_str()))
					strings.emplace_back(lua_tostring(lua::L,-1),1);
				strings.emplace_back("> ",0);
				}break;
			case SDLK_ESCAPE:
				close();
				break;
			}
		}
	}else if(e.type==SDL_TEXTINPUT && shown){
		if(strlen(e.text.text)==1 && e.text.text[0]!='`')
			strings[strings.size()-1].add(cursor++,e.text.text[0]);
	}
	if(cursor<2)cursor=2;
	else if(cursor>strings[strings.size()-1].text.size())
		cursor=strings[strings.size()-1].text.size();
}
void Interface::Console::open(){
	if(strings[strings.size()-1].type!=0)
		strings.emplace_back("> ",0);
	shown=1;
}
void Interface::Console::close(){
	shown=0;
}
void Interface::Console::out(string str){
	if(strings[strings.size()-1].type==0 && strings[strings.size()-1].text=="> "){
		strings[strings.size()-1].text=str;
		strings[strings.size()-1].type=2;
	}else
		strings.emplace_back(str,2);
}
void Interface::Game_interface::update(){
	
}
void Interface::Game_interface::show(){
	for(int q=0;q<player.max_lives;q++){
		GPU_Image *image=textures[((player.lives>q)?"interface/live2.png":"interface/live1.png")];
		GPU_BlitScale(image,0,ren,lives_counter.x+lives_counter.w*q+lives_counter.w/2,lives_counter.y+lives_counter.h/2,lives_counter.w/image->w,lives_counter.h/image->h);
	}
	string str=to_string(entity["player"].health);
	short w=FC_GetWidth(font,str.c_str());
	short h=FC_GetHeigth(font,str.c_str());
}
Interface interface;
