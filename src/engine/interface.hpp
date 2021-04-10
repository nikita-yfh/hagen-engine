#pragma once
#include <vector>
#include <string>
#include "sdl.hpp"
#include "SDL_FontCache.h"
using namespace std;
struct Interface{
	struct Console{
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
		struct String{
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
	}console;
	void show();
	void update();
	void load_config();
};
extern Interface interface;
