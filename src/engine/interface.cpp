#include "interface.hpp"
#include "sdl.hpp"
#include "xmlParser.h"
#include "main.hpp"
#include "level.hpp"
#include "lua.hpp"
#include "text.hpp"
#include "weapon.hpp"
#include "camera.hpp"
#include "utility.hpp"
void Interface::draw_rect1(GPU_Rect pos){
	pos=drawr(pos);
	GPU_RectangleFilled2(ren,pos,widget_color);
	draw_frame1(pos);
}
void Interface::draw_rect2(GPU_Rect pos){
	pos=drawr(pos);
	GPU_RectangleFilled2(ren,pos,widget_color);
	draw_frame2(pos);
}
void Interface::draw_frame1(GPU_Rect pos) {
	float x1=pos.x, x2=pos.x+pos.w;
	float y1=pos.y, y2=pos.y+pos.h;
	GPU_RectangleFilled(ren,x1,y1,x2,y1+frame_size,frame_color1);
	GPU_RectangleFilled(ren,x1,y1,x1+frame_size,y2,frame_color1);
	GPU_RectangleFilled(ren,x2,y2,x1,y2-frame_size,frame_color2);
	GPU_RectangleFilled(ren,x2,y2,x2-frame_size,y1,frame_color2);
}
void Interface::draw_frame2(GPU_Rect pos) {
	float x1=pos.x, x2=pos.x+pos.w;
	float y1=pos.y, y2=pos.y+pos.h;
	GPU_RectangleFilled(ren,x2,y2,x1,y2-frame_size,frame_color1);
	GPU_RectangleFilled(ren,x2,y2,x2-frame_size,y1,frame_color1);
	GPU_RectangleFilled(ren,x1,y1,x2,y1+frame_size,frame_color2);
	GPU_RectangleFilled(ren,x1,y1,x1+frame_size,y2,frame_color2);
}
bool Interface::draw_button(GPU_Rect pos,string text) {
	pos=drawr(pos);
	GPU_RectangleFilled2(ren,pos,widget_color);
	FC_DrawCenter(font,ren,pos,text.c_str());
	if(mouse.in_rect(pos) && mouse.state==2){
		draw_frame2(pos);
		return 1;
	}
	draw_frame1(pos);
	return 0;
}
float Interface::drawx(float x) {
	return viewport.x+x*viewport.w;
}
float Interface::drawy(float y) {
	return viewport.y+y*viewport.h;
}
GPU_Rect Interface::drawr(GPU_Rect r) {
	return GPU_MakeRect(drawx(r.x),drawy(r.y),r.w*viewport.w,r.h*viewport.h);
}
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
	game_interface.load_config();
	{
		XMLNode node=XMLNode::openFileHelper((prefix+"config/interface.xml").c_str(),"interface");
		XMLNode text=node.getChildNode("text");
		load_font(font,text,"color",SH);
	}
	viewport={0,0,SW,SH};
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
	XMLNode node=XMLNode::openFileHelper((prefix+"config/game_interface.xml").c_str(),"game_interface");
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
		if(get_entity("player"))
			health=get_entity("player")->health;
		FC_Draw(font,ren,borders.left,SH-borders.bottom-h,"%s %d %s",
				get_text("game_interface/health_prev").c_str(),
				health,
				get_text("game_interface/health").c_str()).w;
	}
	if(get_entity("player")) {
		auto draw_bullets=[=](string id,string str,uint8_t layer) {
			FC_DrawAlign(font,ren,SW-borders.left,SH-borders.top-layer*h,
						 FC_ALIGN_RIGHT,"%s %d/%d %s",get_text(str+"_prev").c_str(),
						 bullets[id].count,bullets[id].max,
						 get_text(str).c_str());
		};
		uint8_t layer=0;
		if(bullets[weapons[get_entity("player")->weapon].bullet2].max>0 &&
				weapons[get_entity("player")->weapon].bullet2 != weapons[get_entity("player")->weapon].bullet1)
			draw_bullets(weapons[get_entity("player")->weapon].bullet2,"game_interface/bullet2",++layer);
		if(bullets[weapons[get_entity("player")->weapon].bullet1].max>0)
			draw_bullets(weapons[get_entity("player")->weapon].bullet1,"game_interface/bullet1",++layer);
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
	interface.viewport={312,84,400,600};
	interface.draw_rect1({0,0,1,1});
	for(int q=0;q<6;q++)
		interface.draw_button({0.1,0.1+q*0.133 ,0.8,0.1},"text");
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
