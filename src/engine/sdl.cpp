#include "sdl.hpp"
#include "SDL.h"
#include "level.hpp"
#include "SDL_FontCache.h"
#include "utility.hpp"
#include "physic.hpp"
#include "camera.hpp"
#include <string>
FC_Font *font;
SDL_Event e;
int SW=0;
int SH=0;
GPU_Target *ren;
const uint8_t* key=SDL_GetKeyboardState(0);
using namespace std;
map<string,GPU_Image*>textures;
GPU_Image *background=0;
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
void panic(string name,string message) {
	printf("%s\n",message.c_str());
	quit();
	exit(1);
}

void load_cursor() {
	/*SDL_Surface *sur=IMG_Load("cursors/default.png");
	SDL_Cursor *cur=SDL_CreateColorCursor(sur,0,0);
	SDL_FreeSurface(sur);
	SDL_SetCursor(cur);*/
}
void load_texture(string str){
	if(str.size() && !find_texture(str)) {
		textures[str]=GPU_LoadImage(("textures/"+str).c_str());
		if(!textures[str])
			throw string(SDL_GetError());
	}
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
	string path="backgrounds/"+name;
	if(!exist_file(path))
		throw string("Loading \""+name+"\" background failed: file not found");
	if(background)
		GPU_FreeImage(background);
	background=GPU_LoadImage(path.c_str());
	if(!background)
		throw string("Loading \""+name+"\" background failed: "+SDL_GetError());
}
