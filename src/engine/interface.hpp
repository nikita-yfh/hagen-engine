#pragma once
#include <vector>
#include <string>
#include "sdl.hpp"
#include "SDL_FontCache.h"
#include "main.hpp"
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
struct Console {
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
	void draw();
	void update();
	Rect4 borders;
};
struct Pause {
	bool shown=false;
	void Draw();
	void close();
	int width=0;
};
struct SaverLoader {
	bool shown=false;
	bool mode=1; //save
	char selected[64];
	void Draw();
	void update_cache();
	void close();
	vector<string>list;
};
struct SettingManager {
	bool shown=false;
	uint8_t selected=0;
	void Draw();
	void update();
	Settings set;
	vector<string>languages;
};
struct Interface {
	void init_imgui();
	void load_imgui_config();
	void load_imgui_font(string name,float size);
	Game_interface game_interface;
	Console console;
	SaverLoader saver;
	Pause pause;
	SettingManager settingmanager;
	void draw();
	void update();
	void load_config();
	void update_cursor();
	bool shown();
	void hide();
	int prev_time;
	void quicksave();
	void quickload();
};
extern Interface interface;
