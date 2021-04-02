#include "console.hpp"
#include "sdl.hpp"
#include "xmlParser.h"
#include "main.hpp"
Console::String::String(){}
Console::String::String(short _position){
	position=_position;
}
Console::String::String(string _text,uint8_t _type,short _position){
	text=_text;
	type=_type;
	position=_position;
}
void Console::String::add(short pos,char c){
	text.insert(text.begin()+pos,c);
}
void Console::String::del(short pos){
	do {
		if (pos==0) {
			break;
		}
		if ((text[pos-1] & 0x80) == 0x00) {
			text[pos-1]=0x00;
			text.pop_back();
			break;
		}
		if ((text[pos-1] & 0xC0) == 0x80) {
			text[pos-1]=0x00;
			pos--;
			text.pop_back();
		}
		if ((text[pos-1] & 0xC0) == 0xC0) {
			text[pos-1]=0x00;
			text.pop_back();
			break;
		}
	} while(1);
}
Console::Console(){
	strings.emplace_back((string)LUA_VERSION+" Copyright (C) 1994-2015 Lua.org, PUC-Rio",2,0);
}
void Console::show(){
	if(!shown)return;
	GPU_RectangleFilled(ren,0,0,SW,SH,background.color());
	int size=FC_GetLineHeight(font);
	for(String s : strings){
		Color *color;
		if(s.type==0)color=&input_color;
		else if(s.type==1)color=&error_color;
		else color=&output_color;
		FC_DrawColumnColor(font,ren,10,10+size*s.position,SW-20,color->color(),s.text.c_str());
	}
}
void Console::load_config(){
	XMLNode node=XMLNode::openFileHelper((prefix+"config/console.xml").c_str(),"console");
	background.load(node.getChildNode("background").getChildNode("color"));
	{
		XMLNode text=node.getChildNode("text");
		input_color.load(text.getChildNode("input_color"));
		error_color.load(text.getChildNode("error_color"));
		output_color.load(text.getChildNode("output_color"));
		string font_path=text.getAttribute("name");
		int size=stoi(text.getAttribute("size"));
		font_path=prefix+"fonts/"+font_path;
		if(font!=0)
			FC_ClearFont(font);
		font=FC_CreateFont();
		FC_LoadFont(font,font_path.c_str(),size,input_color.color(),TTF_STYLE_NORMAL);
	}
}
void Console::update(){
	if(e.type==SDL_KEYDOWN){
		if(e.key.keysym.sym==SDLK_BACKQUOTE)
			shown=!shown;
		else if(shown){

		}
	}
}
