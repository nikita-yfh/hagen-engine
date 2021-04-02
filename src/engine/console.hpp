#pragma once
#include <vector>
#include <string>
#include "sdl.hpp"
#include "SDL_FontCache.h"
using namespace std;
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
	struct String{
		String();
		String(short _position);
		String(string _text,uint8_t _type,short _position);
		string text;
		uint8_t type=0;
		short position;
		void add(short pos,char c);
		void del(short pos);
	};
	vector<String>strings;
};
