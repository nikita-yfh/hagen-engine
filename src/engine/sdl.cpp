#include "sdl.hpp"
#include "SDL.h"
#include "level.hpp"
#include "utility.hpp"
#include "physic.hpp"
#include "camera.hpp"
#include "main.hpp"
#include "dialog.hpp"
#include "effect.hpp"
#include <string>
#include "shader.hpp"
#include "render.hpp"
int &SW=settings.SW;
int &SH=settings.SH;
GPU_Target *ren=0;
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
Color::Color(SDL_Color c) {
	r=c.r;
	g=c.g;
	b=c.b;
	a=c.a;
}
Color::Color(int _r,int _g,int _b) {
	set(_r,_g,_b,255);
}
SDL_Color Color::color() {
	return SDL_Color({r,g,b,a});
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
	x=load_scaled_float(node,"x");
	y=load_scaled_float(node,"x");
	w=load_scaled_float(node,"w");
	h=load_scaled_float(node,"h");
}
static void print_renderer_info(GPU_Renderer *renderer) {
	GPU_RendererID id = renderer->id;

	info_log(format("Using renderer: %s (%d.%d)", id.name, id.major_version, id.minor_version));
	info_log(format("Shader versions supported: %d to %d", renderer->min_shader_version, renderer->max_shader_version));

	string languages[6]= {
		"None",
		"ARB_ASSEMBLY",
		"GLSL",
		"GLSLES",
		"HLSL",
		"CG"
	};

	info_log("Shader language: "+languages[renderer->shader_language]);
}
void init() {
	if(SDL_Init(SDL_INIT_EVERYTHING)) {
		error_log(SDL_GetError());
		throw string(SDL_GetError());
	}
	info_log("SDL inited succesfully");
	settings.load();
	GPU_SetPreInitFlags(GPU_INIT_DISABLE_VSYNC);
	ren=GPU_Init(settings.SW,settings.SH,settings.fullscreen?SDL_WINDOW_FULLSCREEN:0);
	if(!ren) {
		GPU_ErrorObject error=GPU_PopErrorCode();
		string str=(string)"In function "+error.function+": "+error.details;
		error_log(str);
		throw string(error.details);
	}
	init_target();
	info_log("Renderer created succesfully");

	print_renderer_info(GPU_GetCurrentRenderer());


	int mix_flags=MIX_INIT_MP3|MIX_INIT_MOD;
	if(Mix_Init(mix_flags)&mix_flags!=mix_flags) {
		error_log(SDL_GetError());
		throw string(SDL_GetError());
	}
	if(Mix_OpenAudio(22050,AUDIO_S16SYS,2,640)) {
		error_log(SDL_GetError());
		throw string(SDL_GetError());
	}
	info_log("Music inited succesfully");
	interface.load_config();
	interface.init_imgui();
	info_log("ImGui inited succesfully");
	interface.update_cursor();
}
GPU_Image *find_texture(string id) {
	return textures[id];
}
void quit() {
	try {
		delete_target();
		GPU_Quit();
		Mix_CloseAudio();
		Mix_Quit();
		SDL_Quit();
		textures.clear();
		effect::effects.clear();
		effect::loaded.clear();
		close_level();
	} catch(...) {
		exit(1);
	}
	exit(1);
}
void panic(string message) {
	error_log(message);
	dialog::show("Fatal error",message,dialog::Style::Error);
	quit();
}

void unset_cursor() {
	if(cursor)SDL_FreeCursor(cursor);
	cursor=SDL_GetDefaultCursor();
	SDL_SetCursor(cursor);
}
void set_cursor(string name) {
	if(cursor)SDL_FreeCursor(cursor);
	SDL_Surface *sur=GPU_LoadSurface(string(prefix+"textures/"+name).c_str());
	cursor=SDL_CreateColorCursor(sur,0,0);
	SDL_FreeSurface(sur);
	SDL_SetCursor(cursor);
}
void set_center_cursor(string name) {
	if(cursor)SDL_FreeCursor(cursor);
	SDL_Surface *sur=GPU_LoadSurface(string(prefix+"textures/"+name).c_str());
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
			error_log(SDL_GetError());
		else
			info_log("Loaded texture "+str);
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
void destroy_all() {
	for(auto &t : textures)
		GPU_FreeImage(t.second);
	textures.clear();
}

void load_font(FC_Font *&font, XMLNode node,string color,float h) {
	string font_path=node.getAttribute("name");
	float size=load_scaled_float(node,"size");
	font_path=prefix+"fonts/"+font_path;
	if(font!=0)
		FC_ClearFont(font);
	font=FC_CreateFont();
	Color col;
	load_value(node,color.c_str(),col);
	FC_LoadFont(font,font_path.c_str(),size,col.color(),TTF_STYLE_NORMAL);
}
void play_music(string name) {
	stop_music();
	music=Mix_LoadMUS((prefix+"music/"+name).c_str());
	if(music==0) throw string(SDL_GetError());
	Mix_VolumeMusic(settings.music_volume);
	Mix_PlayMusic(music,-1);
	Mix_ResumeMusic();
}
void stop_music() {
	Mix_PauseMusic();
	if(music)
		Mix_FreeMusic(music);
	music=0;
}
void pause_music() {
	Mix_PauseMusic();
}
void resume_music() {
	Mix_ResumeMusic();
}

void play_sound(string name) {
	if(sounds.find(name)==sounds.end()) {
		sounds[name]=Mix_LoadWAV((prefix+"sound/"+name).c_str());
		if(!sounds[name]) throw string(SDL_GetError());
	}
	Mix_PlayChannel(-1,sounds[name],0);
	Mix_Volume(-1,settings.sound_volume*1.28f);
}
void play_ws_sound(string name) {
	Mix_Chunk *sound=Mix_LoadWAV((prefix+"sound/"+name).c_str());
	if(!sound) throw string(SDL_GetError());
	int channel=Mix_PlayChannel(-1,sounds[name],0);
	Mix_Volume(channel,settings.sound_volume*1.28f);
}
void play_distance_sound(string name,float distance) {
	if(sounds.find(name)==sounds.end()) {
		sounds[name]=Mix_LoadWAV((prefix+"sound/"+name).c_str());
		if(!sounds[name]) throw string(SDL_GetError());
	}
	int channel=Mix_PlayChannel(-1,sounds[name],0);
	Mix_Volume(channel,min(settings.sound_volume*1.28f,1.92f/distance*settings.sound_volume));
}
void clear_sounds() {
	sounds.clear();
}
