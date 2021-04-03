#include "console.hpp"
#include "sdl.hpp"
#include "xmlParser.h"
#include "main.hpp"
#include "lua.hpp"
Console::String::String(){}
Console::String::String(string _text,uint8_t _type){
	text=_text;
	type=_type;
}
void Console::String::add(short pos,char c){
	text.insert(text.begin()+pos,c);
}
void Console::String::del(short pos){
	if(pos>2 && pos<=text.size() && text.size())
		text.erase(text.begin()+pos-1);
}
Console::Console(){
	strings.emplace_back((string)LUA_VERSION+" Copyright (C) 1994-2015 Lua.org, PUC-Rio",2);
	strings.emplace_back("> ",0);
}
void Console::show(){
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
		if(e.text.text[0]<128 && e.text.text[0]!='`')
			strings[strings.size()-1].add(cursor++,e.text.text[0]);
	}
	if(cursor<2)cursor=2;
	else if(cursor>strings[strings.size()-1].text.size())
		cursor=strings[strings.size()-1].text.size();
}
void Console::open(){
	if(strings[strings.size()-1].type!=0)
		strings.emplace_back("> ",0);
	shown=1;
}
void Console::close(){
	shown=0;
}
void Console::out(string str){
	if(strings[strings.size()-1].type==0 && strings[strings.size()-1].text=="> "){
		strings[strings.size()-1].text=str;
		strings[strings.size()-1].type=2;
	}else
		strings.emplace_back(str,2);
}
Console console;
