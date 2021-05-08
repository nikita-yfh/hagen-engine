#pragma once
#include <vector>
#include <string>
#include "sdl.hpp"
#include "SDL_FontCache.h"
using namespace std;
struct Rect4 {
	float top;
	float bottom;
	float left;
	float right;
	void stabilize(float f);
	void load(XMLNode l,float f);
};
struct Interface {
	SDL_Color background{0,0,0,255};
	SDL_Color widget_color{255,0,0,255};
	SDL_Color frame_color1{255,255,255,255};
	SDL_Color frame_color2{0,0,0,255};
	float frame_size=4;
	GPU_Rect viewport;
	float drawx(float x);
	float drawy(float y);
	GPU_Rect drawr(GPU_Rect r);
	void draw_frame1(GPU_Rect pos);
	void draw_frame2(GPU_Rect pos);
	bool draw_button(GPU_Rect pos,string text);


	FC_Font *font=0;
	struct Console {
		Console();
		bool shown;
		int max_strings_count;
		void show();
		void update();
		void load_config();
		Color background;
		Color input_color;
		Color error_color;
		Color output_color;
		FC_Font *font=0;
		short cursor=10;
		struct String {
			String();
			String(string _text,uint8_t _type);
			string text;
			uint8_t type=0;
			void add(short pos,char c);
			void del(short pos);
		};
		vector<String>strings;
		void out(string str);
		void open();
		void close();
	} console;
	struct Game_interface {
		bool shown;
		FC_Font *font=0;
		void load_config();
		void show();
		void update();
		Rect4 borders;
	} game_interface;
	struct Pause {
		Color active_color;
		Color passive_color;
		bool shown;
		uint8_t selected;
		void load_config();
		void show();
		void update();
		void open();
		void close();
		Rect4 borders;
		float w;
		float h;
	} pause;

	void show();
	void update();
	void load_config();
	void update_cursor();
};
extern Interface interface;
