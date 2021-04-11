#include "interface.hpp"
#include "sdl.hpp"
#include "xmlParser.h"
#include "main.hpp"
#include "player.hpp"
#include "level.hpp"
#include "lua.hpp"
#include "text.hpp"
void Rect4::stabilize(){
	if(top<1)top*=SH;
	if(left<1)left*=SH;
	if(right<1)right*=SH;
	if(bottom<1)bottom*=SH;
}
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
		XMLNode l=node.getChildNode("border");
		borders.top=stof(l.getAttribute("top"));
		borders.left=stof(l.getAttribute("left"));
		borders.right=stof(l.getAttribute("right"));
		borders.bottom=stof(l.getAttribute("bottom"));
		borders.stabilize();
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
	short h=FC_GetLineHeight(font);
	{
		string str;
		for(int q=0;q<player.max_lives;q++){
			/*GPU_Image *image=textures[((player.lives>q)?"interface/live2.png":"interface/live1.png")];
			GPU_BlitScale(image,0,ren,borders.left+lives_counter_size*(q+0.5f),borders.top+lives_counter_size/2,lives_counter_size/image->h,lives_counter_size/image->h);*/
			str+=(player.lives>q)?get_text("__live2"):get_text("__live1");
		}
		FC_Draw(font,ren,borders.left,borders.bottom,str.c_str());
	}
	{
		string str=to_string(entities["player"]->health);
		short w=FC_Draw(font,ren,borders.left,SH-borders.bottom-h,"%s %d %s",get_text("__health_prev").c_str(),(int)entities["player"]->health,get_text("__health").c_str()).w;
	}
	{
		auto draw_bullets=[=](string id,string str,uint8_t layer){
			FC_DrawAlign(font,ren,SW-borders.left,SH-borders.top-layer*h,
				FC_ALIGN_RIGHT,"%s %d/%d %s",get_text(str+"_prev").c_str(),
				player.bullets[id].count,player.bullets[id].max,
				get_text(str).c_str());
		};
		uint8_t layer=0;
		if(player.bullets[weapons[entities["player"]->weapon].bullet2].max>0)
			draw_bullets(weapons[entities["player"]->weapon].bullet2,"__bullet2",++layer);
		if(player.bullets[weapons[entities["player"]->weapon].bullet2].max>0)
			draw_bullets(weapons[entities["player"]->weapon].bullet1,"__bullet1",++layer);
	}
}
Interface interface;
