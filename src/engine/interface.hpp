#pragma once
#include <vector>
#include <string>
#include "sdl.hpp"
#include "SDL_FontCache.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
using namespace std;
struct Rect4 {
	float top;
	float bottom;
	float left;
	float right;
	void stabilize(float f);
	void load(XMLNode l,float f);
};
struct Console{
	bool shown=false;
	char                  InputBuf[256];
	vector<string>     	  Items;
	vector<string>        History;
	int                   HistoryPos = -1;    // -1: new line, 0..History.Size-1 browsing history.
	ImGuiTextFilter       Filter;
	bool ScrollToBottom=false;

	Console();
	~Console();
	void ClearLog();
	void AddLog(string);
	void Draw();
	void ExecCommand(string);
	int TextEditCallback(ImGuiInputTextCallbackData* data);
	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
};
struct Game_interface {
	bool shown;
	FC_Font *font=0;
	void load_config();
	void show();
	void update();
	Rect4 borders;
};
struct Interface {
	void init_imgui();
	void load_imgui_font();
	Game_interface game_interface;
	Console console;
	void show();
	void update();
	void load_config();
	void update_cursor();
};
extern Interface interface;
