#include "interface.hpp"
#include "sdl.hpp"
#include "xmlParser.h"
#include "main.hpp"
#include "level.hpp"
#include "lua.hpp"
#include "text.hpp"
#include "weapon.hpp"
void Rect4::stabilize(float f) {
	if(top<1)top*=f;
	if(left<1)left*=f;
	if(right<1)right*=f;
	if(bottom<1)bottom*=f;
}
void Rect4::load(XMLNode l,float f) {
	top=stof(l.getAttribute("top"));
	left=stof(l.getAttribute("left"));
	right=stof(l.getAttribute("right"));
	bottom=stof(l.getAttribute("bottom"));
}
void Interface::update() {
	if(!console.shown)
		pause.update();
	console.update();
	game_interface.update();
}
void Interface::load_config() {
	pause.load_config();
	game_interface.load_config();
}
void Interface::show() {
	if(pause.shown)
		pause.show();
	console.show();
	if(!console.shown)
		game_interface.show();
}
Interface::Console::String::String() {}
Interface::Console::String::String(string _text,uint8_t _type) {
	text=_text;
	type=_type;
}
void Interface::Console::String::add(short pos,char c) {
	text.insert(text.begin()+pos,c);
}
void Interface::Console::String::del(short pos) {
	if(pos>2 && pos<=text.size() && text.size())
		text.erase(text.begin()+pos-1);
}
Interface::Console::Console() {
	strings.emplace_back("LuaJIT 2.0.5 -- Copyright (C) 2005-2017 Mike Pall. http://luajit.org/",2);
	strings.emplace_back("> ",0);
}
void Interface::Console::show() {
	if(!shown)return;
	GPU_RectangleFilled(ren,0,0,SW,SH,background.color());
	int size=FC_GetLineHeight(font);
	int pos=10;
	for(String s : strings) {
		Color *color;
		if(s.type==0)color=&input_color;
		else if(s.type==1)color=&error_color;
		else color=&output_color;
		pos+=FC_DrawColumnColor(font,ren,10,pos,SW-20,color->color(),s.text.c_str()).h;
	}
	if(pos>SH)
		strings.erase(strings.begin());
}
void Interface::Console::load_config() {
	XMLNode node=XMLNode::openFileHelper((prefix+"config/console.xml").c_str(),"console");
	background.load(node.getChildNode("background").getChildNode("color"));
	{
		XMLNode text=node.getChildNode("text");
		input_color.load(text.getChildNode("input_color"));
		error_color.load(text.getChildNode("error_color"));
		output_color.load(text.getChildNode("output_color"));
		load_font(font,text,"input_color",SH);
	}
}
void Interface::Game_interface::load_config() {
	XMLNode node=XMLNode::openFileHelper((prefix+"config/interface.xml").c_str(),"interface");
	{
		XMLNode text=node.getChildNode("text");
		load_font(font,text,"color",SH);
	}
	borders.load(node.getChildNode("border"),SH);
	borders.stabilize(SH);
}
void Interface::Console::update() {
	if(e.type==SDL_KEYDOWN) {
		if(e.key.keysym.sym==SDLK_BACKQUOTE)
			if(shown)close();
			else open();
		else if(shown) {
			switch(e.key.keysym.sym) {
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
			case SDLK_RETURN: {
				string str=strings[strings.size()-1].text;
				str.erase(str.begin(),str.begin()+2);
				if(luaL_dostring(lua::L,str.c_str()))
					strings.emplace_back(lua_tostring(lua::L,-1),1);
				strings.emplace_back("> ",0);
			}
			break;
			case SDLK_ESCAPE:
				close();
				break;
			}
		}
	} else if(e.type==SDL_TEXTINPUT && shown) {
		if(strlen(e.text.text)==1 && e.text.text[0]!='`')
			strings[strings.size()-1].add(cursor++,e.text.text[0]);
	}
	if(cursor<2)cursor=2;
	else if(cursor>strings[strings.size()-1].text.size())
		cursor=strings[strings.size()-1].text.size();
}
void Interface::Console::open() {
	if(strings[strings.size()-1].type!=0)
		strings.emplace_back("> ",0);
	shown=1;
	interface.update_cursor();
	load_config();
}
void Interface::Console::close() {
	shown=0;
	interface.update_cursor();
}
void Interface::Console::out(string str) {
	if(strings[strings.size()-1].type==0 && strings[strings.size()-1].text=="> ") {
		strings[strings.size()-1].text=str;
		strings[strings.size()-1].type=2;
	} else
		strings.emplace_back(str,2);
	if(strings.size()>100)
		strings.erase(strings.begin());
}
void Interface::Game_interface::update() {}
void Interface::Game_interface::show() {
	short h=FC_GetLineHeight(font);
	{
		int health=0;
		if(entities["player"])
			health=entities["player"]->health;
		FC_Draw(font,ren,borders.left,SH-borders.bottom-h,"%s %d %s",
				get_text("game_interface/health_prev").c_str(),
				health,
				get_text("game_interface/health").c_str()).w;
	}
	if(entities["player"]){
		auto draw_bullets=[=](string id,string str,uint8_t layer) {
			FC_DrawAlign(font,ren,SW-borders.left,SH-borders.top-layer*h,
						 FC_ALIGN_RIGHT,"%s %d/%d %s",get_text(str+"_prev").c_str(),
						 bullets[id].count,bullets[id].max,
						 get_text(str).c_str());
		};
		uint8_t layer=0;
		if(bullets[weapons[entities["player"]->weapon].bullet2].max>0 &&
				weapons[entities["player"]->weapon].bullet2 != weapons[entities["player"]->weapon].bullet1)
			draw_bullets(weapons[entities["player"]->weapon].bullet2,"game_interface/bullet2",++layer);
		if(bullets[weapons[entities["player"]->weapon].bullet1].max>0)
			draw_bullets(weapons[entities["player"]->weapon].bullet1,"game_interface/bullet1",++layer);
	}
}
void Interface::Pause::open() {
	shown=1;
	Mix_PauseMusic();
	interface.update_cursor();
}
void Interface::Pause::close() {
	shown=0;
	Mix_ResumeMusic();
	interface.update_cursor();
}
void Interface::Pause::show() {
	//GPU_Image *
	//GPU_BlitScale(textures["interface/pause_background.png"],0,ren,SW/2,SH/2,
	//	)
}
void Interface::Pause::load_config() {
	XMLNode node=XMLNode::openFileHelper((prefix+"config/pause.xml").c_str(),"pause");
	{
		XMLNode text=node.getChildNode("text");
		active_color.load	(text.getChildNode("active_color"));
		passive_color.load	(text.getChildNode("passive_color"));
	}
	{
		XMLNode size=node.getChildNode("size");
		w=stof(size.getAttribute("x"));
		h=stof(size.getAttribute("y"));
	}
	borders.load(node.getChildNode("border"),h);
	load_texture("interface/pause_background.png");
	load_texture("interface/pause_active_button.png");
	load_texture("interface/pause_passive_button.png");
}
void Interface::Pause::update() {
	if(e.type==SDL_KEYDOWN) {
		if(e.key.keysym.sym==SDLK_ESCAPE) {
			if(shown)close();
			else open();
		}
	}
}

void Interface::update_cursor() {
	if(pause.shown || console.shown)
		set_cursor("default.png");
	else
		set_cursor("aim.png");
}
Interface interface;
