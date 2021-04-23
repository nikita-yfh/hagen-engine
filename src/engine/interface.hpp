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
	void stabilize();
};
struct Interface {
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
		Color pause_color;
		bool shown;
		uint8_t selected;
		FC_Font *font=0;
		void load_config();
		void show();
		void update();
		Rect4 borders;
	} pause;
	void show();
	void update();
	void load_config();
};
extern Interface interface;
