#include "sdl.hpp"
#include "SDL.h"
#include "level.hpp"
#include "SDL_image.h"
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
std::vector<Texture>textures;
GPU_Image *background=0;
void init(const char* title,int w,int h) {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	GPU_SetPreInitFlags(GPU_INIT_DISABLE_VSYNC);
	ren=GPU_Init(w,h,0);
	SW=w;
	SH=h;
}
GPU_Image *find_texture(std::string id) {
	for(int q=0; q<textures.size(); q++)
		if(textures[q].id==id)return textures[q].texture;
	return 0;
}
void quit() {
	GPU_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
void panic(string name,string message) {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,name.c_str(),message.c_str(),0);
	quit();
	exit(1);
}

void load_cursor() {
	SDL_Surface *sur=IMG_Load("cursors/default.png");
	SDL_Cursor *cur=SDL_CreateColorCursor(sur,0,0);
	SDL_FreeSurface(sur);
	SDL_SetCursor(cur);
}
void load_textures() {
	for(int q=0; q<bodies.size(); q++) {
		for(b2Fixture *fix=bodies[q]->GetFixtureList(); fix; fix=fix->GetNext()) {
			string str=F_DATA(fix,texture);
			if(str.size() && !find_texture(str)) {
				GPU_Image *texture=GPU_LoadImage(("textures/"+str).c_str());
				if(!texture)
					throw string(SDL_GetError());
				textures.push_back({texture,str});
			}
		}
	}
}
void configure_textures() {
}
void destroy_textures() {
}
void load_background(std::string name) {
	string path="backgrounds/"+name;
	if(!exist_file(path))
		throw string("Loading \""+name+"\" background failed: file not found");
	if(background)
		GPU_FreeImage(background);
	background=GPU_LoadImage(path.c_str());
	if(!background)
		throw string("Loading \""+name+"\" background failed: "+SDL_GetError());
}
