#include "sdl.hpp"
#include "SDL.h"
#include "level.hpp"
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
map<string,GPU_Image*>textures;
map<string,Mix_Chunk*>sounds;
SDL_Cursor *cursor=0;
Mix_Music *music=nullptr;
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
Color::Color(XMLNode node) {
	load(node);
}
Color::Color(int _r,int _g,int _b) {
	set(_r,_g,_b,255);
}
SDL_Color Color::color() {
	return SDL_Color({r,g,b,a});
}
void Color::load(XMLNode node) {
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
void Rect::load(XMLNode node,float f) {
	x=stoi(node.getAttribute("x"));
	y=stoi(node.getAttribute("y"));
	w=stoi(node.getAttribute("w"));
	h=stoi(node.getAttribute("h"));
	if(x<1.0f)x*=f;
	if(y<1.0f)y*=f;
	if(w<1.0f)w*=f;
	if(h<1.0f)h*=f;
}
void init(const char* title,int w,int h) {
	SDL_Init(SDL_INIT_EVERYTHING);
	GPU_SetPreInitFlags(GPU_INIT_DISABLE_VSYNC);
	ren=GPU_Init(w,h,0);
	Mix_Init(MIX_INIT_MP3|MIX_INIT_MOD);
	Mix_OpenAudio(22050,AUDIO_S16SYS,2,640);
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

void unset_cursor(){
	if(cursor)SDL_FreeCursor(cursor);
	cursor=SDL_GetDefaultCursor();
	SDL_SetCursor(cursor);
}
void set_cursor(string name) {
	if(cursor)SDL_FreeCursor(cursor);
	SDL_Surface *sur=GPU_LoadSurface(string(prefix+"cursors/"+name).c_str());
	cursor=SDL_CreateColorCursor(sur,0,0);
	SDL_FreeSurface(sur);
	SDL_SetCursor(cursor);
}
void set_center_cursor(string name) {
	if(cursor)SDL_FreeCursor(cursor);
	SDL_Surface *sur=GPU_LoadSurface(string(prefix+"cursors/"+name).c_str());
	cursor=SDL_CreateColorCursor(sur,sur->w/2,sur->h/2);
	if(!sur || !cursor)
		unset_cursor();
	SDL_FreeSurface(sur);
	SDL_SetCursor(cursor);
}
GPU_Image *load_texture(string str) {
	if(str.size() && !find_texture(str)) {
		textures[str]=GPU_LoadImage((prefix+"textures/"+str).c_str());
		if(!textures[str])
			throw string(SDL_GetError());
	}
	return textures[str];
}
void load_body_textures(b2Body *body) {
	for(b2Fixture *fix=body->GetFixtureList(); fix; fix=fix->GetNext()) {
		load_texture(F_DATA(fix,texture));
	}
}
void load_entity_textures(Entity *ent) {
	for(auto body : ent->bodies) {
		load_body_textures(body.second);
	}
}
void load_textures() {
	for(auto body : bodies)
		load_body_textures(body.second);
	for(auto ent : entities)
		load_entity_textures(ent.second);
	load_texture(background);
}
void configure_textures() {
}
void destroy_all() {
	for(auto &t : textures)
		GPU_FreeImage(t.second);
	textures.clear();
}

void load_font(FC_Font *&font, XMLNode node,string color,float h) {
	string font_path=node.getAttribute("name");
	float size=stof(node.getAttribute("size"));
	if(size<1)size*=h;
	font_path=prefix+"fonts/"+font_path;
	if(font!=0)
		FC_ClearFont(font);
	font=FC_CreateFont();
	FC_LoadFont(font,font_path.c_str(),size,Color(node.getChildNode(color.c_str())).color(),TTF_STYLE_NORMAL);
}
void play_music(string name){
	stop_music();
	music=Mix_LoadMUS((prefix+"music/"+name).c_str());
	if(music==0) throw string(SDL_GetError());
	Mix_PlayMusic(music,-1);
	Mix_ResumeMusic();
}
void stop_music(){
	Mix_PauseMusic();
	if(music)
		Mix_FreeMusic(music);
	music=0;
}
void pause_music(){
	Mix_PauseMusic();
}
void resume_music(){
	Mix_ResumeMusic();
}

void play_sound(string name){
	if(sounds.find(name)==sounds.end()){
		sounds[name]=Mix_LoadWAV((prefix+"sound/"+name).c_str());
		if(!sounds[name]) throw string(SDL_GetError());
	}
	Mix_PlayChannel(-1,sounds[name],0);
}
void play_ws_sound(string name){
	Mix_Chunk *sound=Mix_LoadWAV((prefix+"sound/"+name).c_str());
	if(!sound) throw string(SDL_GetError());
	Mix_PlayChannel(-1,sounds[name],0);
}
void play_distance_sound(string name,float x,float y){

}
