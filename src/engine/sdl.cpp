#include "sdl.hpp"
#include "SDL.h"
#include "level.hpp"
#include "SDL_FontCache.h"
#include "utility.hpp"
#include "physic.hpp"
#include "camera.hpp"
#include "main.hpp"
#include "dialog.hpp"
#include <string>
SDL_Event e;
int SW=0;
int SH=0;
GPU_Target *ren;
const uint8_t* key=SDL_GetKeyboardState(0);
using namespace std;
unordered_map<string,GPU_Image*>textures;
GPU_Image *background=0;
void Color::set(int _r,int _g,int _b,int _a) {
	r=_r;
	g=_g;
	b=_b;
	a=_a;
}
Color::Color() {}
Color::Color(int _r,int _g,int _b,int _a) {
	set(_r,_g,_b,_a);
}
Color::Color(int _r,int _g,int _b) {
	set(_r,_g,_b,255);
}
SDL_Color Color::color() {
	return SDL_Color({r,g,b,a});
}
void Color::load(XMLNode node){
	r=stoi(node.getAttribute("r"));
	g=stoi(node.getAttribute("g"));
	b=stoi(node.getAttribute("b"));
	a=stoi(node.getAttribute("a"));
}
void Rect::set(float _x,float _y,float _w,float _h) {
	x=_x;
	y=_y;
	w=_w;
	h=_h;
}
Rect::Rect() {}
Rect::Rect(float _x,float _y,float _w,float _h) {
	set(_x,_y,_w,_h);
}
void Rect::load(XMLNode node){
	x=stoi(node.getAttribute("x"));
	y=stoi(node.getAttribute("y"));
	w=stoi(node.getAttribute("w"));
	h=stoi(node.getAttribute("h"));
}
void init(const char* title,int w,int h) {
	SDL_Init(SDL_INIT_EVERYTHING);
	GPU_SetPreInitFlags(GPU_INIT_DISABLE_VSYNC);
	ren=GPU_Init(w,h,0);
	SW=w;
	SH=h;
}
GPU_Image *find_texture(string id) {
	return textures[id];
}
void quit() {
	GPU_Quit();
	TTF_Quit();
	SDL_Quit();
}
void panic(string message) {
	dialog::show("Fatal error",message,dialog::Style::Error);
	quit();
	exit(1);
}

void load_cursor() {
	SDL_Surface *sur=GPU_LoadSurface(string(prefix+"cursors/default.png").c_str());
	SDL_Cursor *cur=SDL_CreateColorCursor(sur,0,0);
	SDL_FreeSurface(sur);
	SDL_SetCursor(cur);
}
GPU_Image *load_texture(string str){
	if(str.size() && !find_texture(str)) {
		textures[str]=GPU_LoadImage((prefix+"textures/"+str).c_str());
		if(!textures[str])
			throw string(SDL_GetError());
	}
	return textures[str];
}
void load_body_textures(b2Body *body){
	for(b2Fixture *fix=body->GetFixtureList(); fix; fix=fix->GetNext()) {
		load_texture(F_DATA(fix,texture));
	}
}
void load_entity_textures(Entity *ent){
	for(auto body : ent->bodies) {
		load_body_textures(body.second);
	}
}
void load_textures() {
	for(auto body : bodies) {
		load_body_textures(body.second);
	}
	for(auto ent : entities) {
		load_entity_textures(ent.second);
	}
}
void configure_textures() {
}
void destroy_textures() {
}
void load_background(string name) {
	string path=prefix+"backgrounds/"+name;
	if(!exist_file(path))
		throw string("Loading \""+name+"\" background failed: file not found");
	if(background)
		GPU_FreeImage(background);
	background=GPU_LoadImage(path.c_str());
	if(!background)
		throw string("Loading \""+name+"\" background failed: "+SDL_GetError());
}
