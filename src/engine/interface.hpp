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
struct WindowConfig {
	int width;
	int heigth;
	bool resize;
	bool collapse;
	bool move;
	bool title;
	bool focus;
	bool center;
	void load(XMLNode);
	bool apply(const char* name,bool *shown);
};
struct Rect4 {
	float top;
	float bottom;
	float left;
	float right;
	void stabilize(float f);
	void load(XMLNode l);
};
struct Vec {
	float x;
	float y;
	void stabilize(float f);
	void load(XMLNode l);
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
	void draw();
	void ExecCommand(string);
	int TextEditCallback(ImGuiInputTextCallbackData* data);
	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
	WindowConfig config;
};
struct GameInterface {
	bool shown=true;
	FC_Font *font=0;
	void load_config();
	void draw();
	void update();
	Rect4 borders;
};
struct Pause {
	bool shown=false;
	void draw();
	void close();
	int width=0;
	WindowConfig config;
};
struct SaverLoader {
	bool shown=false;
	bool mode=1; //1-save,0-load
	char selected[64];
	void draw();
	void update_cache();
	void close();
	vector<string>list;
	WindowConfig config;
};
struct SettingManager {
	bool shown=false;
	uint8_t selected=0;
	void draw();
	void update();
	Settings set;
	vector<string>languages;
	WindowConfig config;
	bool restart=false;
	void apply();
};
struct LevelChooser {
	vector<bool>opened;
	WindowConfig config;
	void load();
	void save();
	void def();
};
struct MainMenu {
	~MainMenu();
	bool shown=true;
	void draw();
	void load_config();
	FC_Font *font;
	Color active;
	Color inactive;
	GPU_Image *title=nullptr;
	Rect4 borders;
	float image_h;
};
struct Interface {
	void init_imgui();
	void load_imgui_config();
	void load_imgui_font(string name,float size);
	GameInterface game_interface;
	Console console;
	SaverLoader saver;
	Pause pause;
	SettingManager settingmanager;
	MainMenu mainmenu;
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
