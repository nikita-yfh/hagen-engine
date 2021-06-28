#include "interface.hpp"
#include "sdl.hpp"
#include "xmlParser.h"
#include "main.hpp"
#include "level.hpp"
#include "lua.hpp"
#include "text.hpp"
#include "weapon.hpp"
#include "camera.hpp"
#include "utility.hpp"
#include "input.hpp"
#include "imgui_impl_sdl.h"
#include "Vector.h"
#ifdef ANDROID
#include <GLES2/gl2.h>
#include "imgui_impl_sdl_es2.h"
#else
#include "imgui_impl_sdl_gl3.h"
#endif
typedef bool(initImgui_t)(SDL_Window*);
typedef void(newFrame_t)(SDL_Window*);
typedef void(shutdown_t)();

static initImgui_t *initImgui;
static newFrame_t *newFrame;
static shutdown_t *shutdown;
using namespace ImGui;

static void trim(std::string &s) {
	while(s[0]==' ')
		s.erase(s.begin());
	while(s[s.size()-1]==' ')
		s.erase(s.end());
}


void Interface::update() {
	if(!mainmenu.shown) {
		if(pkey(SDL_SCANCODE_GRAVE)) {
			console.shown=!console.shown;
			if(!shown())hide();
			update_cursor();
		} else if(pkey(SDL_SCANCODE_ESCAPE)||pkey(SDL_SCANCODE_AC_BACK)) {
			if(console.shown)
				console.hide();
			else if(pause.shown)
				pause.hide();
			else
				pause.shown=true;
			if(!shown())hide();
			update_cursor();
		} else if(pkey(SDL_SCANCODE_F5))
			quicksave();
		else if(pkey(SDL_SCANCODE_F9))
			quickload();
	}
	console.update();
}
void Interface::init_imgui() {
	SDL_Window* window = SDL_GetWindowFromID(ren->context->windowID);
	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& io = GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NoMouseCursorChange;
	io.IniFilename=nullptr;
#ifdef ANDROID
	initImgui = ImGui_ImplSdlGLES2_Init;
	newFrame = ImGui_ImplSdlGLES2_NewFrame;
	shutdown = ImGui_ImplSdlGLES2_Shutdown;
#else
	initImgui = ImGui_ImplSdlGL3_Init;
	newFrame = ImGui_ImplSdlGL3_NewFrame;
	shutdown = ImGui_ImplSdlGL3_Shutdown;
#endif
	initImgui(window);
	StyleColorsDark();
	load_imgui_config();
}
void Interface::update_imgui() {
	ImGui_ImplSDL2_ProcessEvent(&e);
}
void Interface::new_frame() {
	newFrame(SDL_GetWindowFromID(ren->context->windowID));
}
void Interface::load_imgui_config() {
	XMLNode node=open_xml((prefix+"config/imgui.xml").c_str(),"imgui");
	XMLNode text=node.getChildNode("text");
	if(!text.isEmpty())
		load_imgui_font(text.getAttribute("name"),text.getAttributef("size"));
	vector<const char*>colors= {
		"Text",
		"TextDisabled",
		"WindowBg",
		"ChildBg",
		"PopupBg",
		"Border",
		"BorderShadow",
		"FrameBg",
		"FrameBgHovered",
		"FrameBgActive",
		"TitleBg",
		"TitleBgActive",
		"TitleBgCollapsed",
		"MenuBarBg",
		"ScrollbarBg",
		"ScrollbarGrab",
		"ScrollbarGrabHovered",
		"ScrollbarGrabActive",
		"CheckMark",
		"SliderGrab",
		"SliderGrabActive",
		"Button",
		"ButtonHovered",
		"ButtonActive",
		"Header",
		"HeaderHovered",
		"HeaderActive",
		"Separator",
		"SeparatorHovered",
		"SeparatorActive",
		"ResizeGrip",
		"ResizeGripHovered",
		"ResizeGripActive",
		"PlotLines",
		"PlotLinesHovered",
		"PlotHistogram",
		"PlotHistogramHovered",
		"TextSelectedBg",
		"DragDropTarget",
		"NavHighlight",
		"NavWindowingHighlight",
		"NavWindowingDimBg",
		"ModalWindowDimBg"
	};
	ImGuiStyle &style=GetStyle();
	XMLNode colors_n=node.getChildNode("colors");
	if(!colors_n.isEmpty()) {
		for(int q=0; q<colors.size(); q++) {
			XMLNode color_n=colors_n.getChildNode(colors[q]);
			if(color_n.isEmpty())
				continue;
			ImVec4 color(
				color_n.getAttributei("r")/255.0f,
				color_n.getAttributei("g")/255.0f,
				color_n.getAttributei("b")/255.0f,
				color_n.getAttributei("a")/255.0f
			);
			style.Colors[q]=color;
		}
	}
	if(load_value(node,"Alpha",					style.Alpha))
		style.Alpha/=255.0f;
	load_value(node,"WindowPadding",			style.WindowPadding);
	load_value(node,"WindowRounding",			style.WindowRounding);
	load_value(node,"WindowBorderSize",			style.WindowBorderSize);
	load_value(node,"WindowMinSize",			style.WindowMinSize);
	load_value(node,"WindowTitleAlign",			style.WindowTitleAlign);
	load_value(node,"ChildRounding",			style.ChildRounding);
	load_value(node,"FramePadding",				style.FramePadding);
	load_value(node,"PopupBorderSize",			style.PopupBorderSize);
	load_value(node,"FrameRounding",			style.FrameRounding);
	load_value(node,"FrameBorderSize",			style.FrameBorderSize);
	load_value(node,"ItemSpacing",				style.ItemSpacing);
	load_value(node,"ItemInnerSpacing",			style.ItemInnerSpacing);
	load_value(node,"TouchExtraPadding",		style.TouchExtraPadding);
	load_value(node,"IndentSpacing",			style.IndentSpacing);
	load_value(node,"ColumnsMinSpacing",		style.ColumnsMinSpacing);
	load_value(node,"ScrollbarSize",			style.ScrollbarSize);
	load_value(node,"ScrollbarRounding",		style.ScrollbarRounding);
	load_value(node,"GrabMinSize",				style.GrabMinSize);
	load_value(node,"GrabRounding",				style.GrabRounding);
	load_value(node,"ButtonTextAlign",			style.ButtonTextAlign);
	load_value(node,"DisplayWindowPadding",		style.DisplayWindowPadding);
	load_value(node,"DisplaySafeAreaPadding",	style.ChildBorderSize);
	load_value(node,"MouseCursorScale",			style.MouseCursorScale);
	load_value(node,"AntiAliasedLines",			style.AntiAliasedLines);
	load_value(node,"AntiAliasedFill",			style.AntiAliasedFill);
	load_value(node,"CurveTessellationTol",		style.CurveTessellationTol);
	settingmanager.config.load(node.getChildNode("settings"));
	console.config.load(node.getChildNode("console"));
	pause.config.load(node.getChildNode("pause"));
	saver.config.load(node.getChildNode("saverloader"));
	levelchooser.config.load(node.getChildNode("levelchooser"));
}
ImFont *Interface::load_imgui_font(string name,float size) {
	name = (prefix+"fonts/"+name);
	ImFontConfig font_config;
	font_config.OversampleH = 1;
	font_config.OversampleV = 1;
	font_config.PixelSnapH = 1;

	static const ImWchar ranges[] = {
		0x0020, 0x00FF,
		0x0400, 0x044F,
		0,
	};
	ImFont *font=GetIO().Fonts->AddFontFromFileTTF(name.c_str(), size, &font_config, ranges);
	info_log("Loaded font "+name);
	return font;
}
void Interface::load_config() {
	game_interface.load_config();
	mainmenu.load_config();
	info_log("Loaded interface config");
}
void Interface::draw() {
	mainmenu.draw();
	game_interface.draw();
	GPU_Rectangle(ren,-20,-20,100000,100000,SDL_Color{0,0,0,0});
	console.draw();
	pause.draw();
	saver.draw();
	settingmanager.draw();
	levelchooser.draw();
	Render();
}
void Interface::update_cursor() {
	if(shown() || mainmenu.shown) {
		GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
	} else {
		GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		set_center_cursor("cursor.png");
	}
}
bool Interface::shown() {
	return console.shown || pause.shown;
}
void Interface::hide() {
	lua::prev_time=SDL_GetTicks();
}
void Interface::quicksave() {
	save_world_state("quicksave");
}
void Interface::quickload() {
	if(exist_file(saves+"quicksave.xml"))
		load_world_state("quicksave");
}

void Pause::draw() {
	if(!shown)return;
	if (!config.apply(text::getc("pause/title"), &shown)) {
		End();
		return;
	}
	ImVec2 align=ImVec2(width, 0);
	if(Button(text::getc("pause/continue"),align)) {
		hide();
	}
	if(Button(text::getc("pause/save_game"),align))
		interface.saver.show(1);
	if(Button(text::getc("pause/load_game"),align))
		interface.saver.show(0);
	if(Button(text::getc("pause/settings"),align))
		interface.settingmanager.show();
	if(Button(text::getc("pause/main_menu"),align))
		OpenPopup(text::getc("pause/main_menu_title"));
	if(Button(text::getc("pause/exit_game"),align))
		OpenPopup(text::getc("pause/exit_game_title"));
	width=GetWindowContentRegionWidth();
	if (BeginPopupModal(text::get("pause/main_menu_title").c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		Text("%s",text::getc("pause/main_menu_text"));
		Separator();

		if(Button(text::getc("common/ok")))
			interface.mainmenu.show();
		SameLine();
		if (Button(text::getc("common/cancel"))) {
			CloseCurrentPopup();
		}
		EndPopup();
	}
	if (BeginPopupModal(text::get("pause/exit_game_title").c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		Text("%s",text::getc("pause/exit_game_text"));
		Separator();

		if(Button(text::getc("common/ok")))
			quit();
		SameLine();
		if (Button(text::getc("common/cancel"))) {
			CloseCurrentPopup();
		}
		EndPopup();
	}
	End();
}
void Pause::hide() {
	shown=false;
	interface.saver.hide();
	interface.console.hide();
	interface.settingmanager.hide();
}


Console::Console() {
	ClearLog();
	memset(InputBuf, 0, sizeof(InputBuf));
}
Console::~Console() {
	ClearLog();
}
void  Console::ClearLog() {
	items.clear();
}
void Console::AddLog(string str) {
	items.push_back(str+"\n");
	ScrollToBottom=true;
}
void Console::draw() {
	if(!shown)return;
	if (!config.apply(text::getc("console/title"), &shown)) {
		End();
		return;
	}
	const float footer_height_to_reserve = GetStyle().ItemSpacing.y + GetFrameHeightWithSpacing();
	BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (BeginPopupContextWindow()) {
		if (Selectable(text::getc("console/clear")))
			ClearLog();
		EndPopup();
	}
	PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
	for (string item : items) {
		if (!Filter.PassFilter(item.c_str()))
			continue;
		ImVec4 color;
		if (item.find("\"]:")!=item.npos || item.find("[E]")!=item.npos)
			color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		else if (item.find("[I]")!=item.npos)
			color = ImVec4(0.7, 0.7f, 0.7f, 1.0f);
		else if (item.find("> ") == 0)
			color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		else if (item.find("\t\t- ") == 0 || item.find(text::get("console/matches"))!=item.npos)
			color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
		else
			color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
		PushStyleColor(ImGuiCol_Text, color);
		TextWrapped("%s",item.c_str());
		PopStyleColor();
	}
	if(GetScrollY() >= GetScrollMaxY() && ScrollToBottom)
		SetScrollHere(1.0f);

	PopStyleVar();
	EndChild();
	Separator();
	bool reclaim_focus = false;
	ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
	if (InputText(text::getc("console/input"), InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &Console::TextEditCallbackStub, (void*)this)) {
		string s = InputBuf;
		trim(s);
		if (s[0]) ExecCommand(s);
		strcpy(InputBuf,"");
		reclaim_focus = true;
	}
	SetItemDefaultFocus();
	if (reclaim_focus)
		SetKeyboardFocusHere(-1);

	End();
}
void Console::ExecCommand(string command_line) {
	AddLog("> "+command_line);
	HistoryPos = -1;
	for (int i = History.size() - 1; i >= 0; i--)
		if (History[i] == command_line) {
			History.erase(History.begin() + i);
			break;
		}
	History.push_back(command_line);
	if(luaL_dostring(lua::L,command_line.c_str()))
		AddLog(lua_tostring(lua::L,-1));
}
int Console::TextEditCallbackStub(ImGuiInputTextCallbackData* data) {
	Console* console = (Console*)data->UserData;
	return console->TextEditCallback(data);
}
int Console::TextEditCallback(ImGuiInputTextCallbackData* data) {
	switch (data->EventFlag) {
	case ImGuiInputTextFlags_CallbackCompletion: {
		string command=data->Buf;
		command.erase(command.begin()+data->CursorPos,command.end());

		char break_chars[]=" \t,;*&><=-+^~()[]{}\"'?%$#@\\|";
		int pos=command.size()-1;
		while(pos-->0) {
			bool ok=0;
			for(char c : break_chars) {
				if(command[pos]==c) {
					command.erase(command.begin(),command.begin()+pos+1);
					pos=0;
					break; //exit
				}
			}
			if(ok)break;
		}
		vector<string> candidates=lua::get_table_keys(command);
		if (candidates.size()==1) {
			int s1=data->CursorPos;
			if(command.find('.')!=command.npos)
				command.erase(0,command.rfind('.')+1);
			int s2=command.size();
			data->DeleteChars(s1-s2,s2);
			data->InsertChars(s1-s2,candidates[0].c_str());
		} else if(candidates.size()) {
			AddLog("\t\t"+text::get("console/matches"));
			for (int i = 0; i < candidates.size(); i++)
				AddLog("\t\t- "+candidates[i]);
		}

		break;
	}
	case ImGuiInputTextFlags_CallbackHistory: {
		const int prev_history_pos = HistoryPos;
		if (data->EventKey == ImGuiKey_UpArrow) {
			if (HistoryPos == -1)
				HistoryPos = History.size() - 1;
			else if (HistoryPos > 0)
				HistoryPos--;
		} else if (data->EventKey == ImGuiKey_DownArrow) {
			if (HistoryPos != -1)
				if (++HistoryPos >= History.size())
					HistoryPos = -1;
		}
		if (prev_history_pos != HistoryPos) {
			string history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
			data->DeleteChars(0, data->BufTextLen);
			data->InsertChars(0, history_str.c_str());
		}
	}
	}
	return 0;
}
void Console::show() {
	shown=true;
}
void Console::hide() {
	shown=false;
}
void Console::update() {
	if(items.size()>LOG_MAX)
		items.erase(items.begin(),items.end()-LOG_MAX);
}

void Rect4::load(XMLNode l) {
	top=load_scaled_float(l,"top");
	left=load_scaled_float(l,"left");
	right=load_scaled_float(l,"right");
	bottom=load_scaled_float(l,"bottom");
}


void Vec::load(XMLNode l) {
	x=load_scaled_float(l,"x");
	y=load_scaled_float(l,"y");
}

void SaverLoader::draw() {
	if(!shown)return;
	SetNextWindowSize(ImVec2(520, 300), ImGuiCond_FirstUseEver);
	if (!config.apply(text::getc(mode?"saveload/save_title":"saveload/load_title"), &shown)) {
		End();
		return;
	}
	const float footer_height_to_reserve = GetStyle().ItemSpacing.y + GetFrameHeightWithSpacing();
	BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_HorizontalScrollbar);
	bool ok=1;
	for (auto &l : list) {
		if (Selectable(l.c_str(), l.find(selected)==0 && ok && strlen(selected))) {
			strcpy(selected,l.c_str());
			ok=0;
		}
	}
	EndChild();
	bool press=0;
	if((mode && Button(text::getc("saveload/save")))||
			(!mode &&Button(text::getc("saveload/load"))))press=1;
	SameLine();
	if(Button(text::getc("saveload/delete"))) {
		remove((saves+selected+".xml").c_str());
		update_cache();
	}
	SameLine();
	if(Button(text::getc("common/cancel")))hide();
	SameLine();
	if(InputText("",selected,64,ImGuiInputTextFlags_EnterReturnsTrue) || press) {
		if(strlen(selected) && string(selected)!=string("settings")) {
			if(mode)
				save_world_state(selected);
			else if(exist_file(saves+selected+".xml"))
				load_world_state(selected);
			interface.pause.hide();
			interface.mainmenu.hide();
			hide();
		}
	}

	End();
}
void SaverLoader::show(bool _mode) {
	update_cache();
	mode=_mode;
	shown=true;
}
void SaverLoader::update_cache() {
	list=list_files(saves);
	for(int q=0; q<list.size(); q++) {
		string &s=list[q];
		if(s.find(".")!=string::npos) {
			s.erase(s.begin()+s.find("."),s.end());
		}
		if(s=="settings" || s=="levels")
			list.erase(list.begin()+q--);
	}
}
void SaverLoader::hide() {
	list.clear();
	shown=false;
}

void SettingManager::draw() {
	if(!shown)return;
	if(!config.apply(text::getc("settings/title"), &shown)) {
		End();
		return;
	}
	BeginChild(text::getc("settings/title"), ImVec2(0, -GetFrameHeightWithSpacing()),true);
	Text("%s",text::getc("settings/graphics"));
	int size_input[2]= {set.SW,set.SH};
	if(InputInt2(text::getc("settings/window_size"),size_input))
		restart_f=true;
	set.SW=size_input[0];
	set.SH=size_input[1];
	if(Checkbox(text::getc("settings/fullscreen"),&set.fullscreen))
		restart_f=true;
	Text("%s",text::getc("settings/sound"));
	SliderInt(text::getc("settings/sound_volume"),&set.sound_volume,0,100,"%d%%");
	SliderInt(text::getc("settings/music_volume"),&set.music_volume,0,100,"%d%%");
	Text("%s",text::getc("settings/game"));
	if (BeginCombo(text::getc("settings/language"),set.language.c_str())) {
		for (int q = 0; q<languages.size(); q++) {
			const bool is_selected = (set.language==languages[q]);
			if (Selectable(languages[q].c_str(),is_selected))
				set.language=languages[q];
			if (is_selected)
				SetItemDefaultFocus();
		}
		EndCombo();
	}

	EndChild();
	if(Button(text::getc("common/ok"))) {
		if(restart_f)
			OpenPopup(text::get("settings/restart_title").c_str());
		else
			apply();
	}
	SameLine();
	if(Button(text::getc("common/cancel")))
		hide();
	if (BeginPopupModal(text::get("settings/restart_title").c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		Text("%s",text::getc("settings/restart_text"));
		Separator();
		if(Button(text::getc("common/ok")))
			apply();
		SameLine();
		if (Button(text::getc("common/cancel")))
			CloseCurrentPopup();
		EndPopup();
	}
	End();
}
void SettingManager::apply() {
	set.save();
	if(restart_f)
		restart();
	else if(settings.language!=set.language)
		text::clear_locale();
	settings=set;
	hide();
}
void SettingManager::update() {
	set=settings;
	languages.clear();
	XMLNode node=open_xml((prefix+"config/languages.xml").c_str(),"languages");
	int count=node.getAttributei("count");
	for(int q=0; q<count; q++) {
		XMLNode lang=node.getChildNode("language",q);
		languages.push_back(lang.getAttribute("id"));
	}
	restart_f=false;
}
void SettingManager::show() {
	shown=true;
	update();
}
void SettingManager::hide() {
	shown=false;
	languages.clear();
}


void WindowConfig::load(XMLNode node) {
	collapse=node.getAttributei("collapse");
	resize=node.getAttributei("resize");
	center=node.getAttributei("center");
	heigth=node.getAttributei("heigth");
	width=node.getAttributei("width");
	title=node.getAttributei("title");
	focus=node.getAttributei("focus");
	move=node.getAttributei("move");
}
bool WindowConfig::apply(const char* name,bool *shown) {
	if(center)
		SetNextWindowPosCenter(ImGuiCond_FirstUseEver);
	SetNextWindowSize({width*1.0f,heigth*1.0f},ImGuiCond_FirstUseEver);
	int flags=0;
	if(!collapse)	flags|=ImGuiWindowFlags_NoCollapse;
	if(!move)		flags|=ImGuiWindowFlags_NoMove;
	if(!title)		flags|=ImGuiWindowFlags_NoTitleBar;
	if(!resize)		flags|=ImGuiWindowFlags_NoResize;
	if(!focus)		flags|=ImGuiWindowFlags_NoFocusOnAppearing;
	return Begin(name, shown, flags);
}


void GameInterface::load_config() {
	XMLNode node=open_xml((prefix+"config/game_interface.xml").c_str(),"game_interface");
	{
		XMLNode text=node.getChildNode("text");
		load_font(font,text,"color",SH);
	}
	load_value(node,"border",borders);
#ifdef TOUCH
	sensor::load();
#endif
}
void GameInterface::draw() {
	if(!shown)return;
	if(interface.mainmenu.shown)return;
	short h=FC_GetLineHeight(font);
	{
		int health=0;
		if(get_entity("player"))
			health=get_entity("player")->health;
		FC_Draw(font,ren,borders.left,SH-borders.bottom-h,"%s %d %s",
				text::getc("game_interface/health_prev"),
				health,
				text::getc("game_interface/health")).w;
	}
	if(get_entity("player")) {
		auto draw_bullets=[=](string id,string str,uint8_t layer) {
			FC_DrawAlign(font,ren,SW-borders.left,SH-borders.top-layer*h,
						 FC_ALIGN_RIGHT,"%s %d/%d %s",text::getc(str+"_prev"),
						 bullets[id].count,bullets[id].max,
						 text::getc(str));
		};
		uint8_t layer=0;
		if(bullets[get_entity("player")->weapon.bullet2].max>0 &&
				get_entity("player")->weapon.bullet2 != get_entity("player")->weapon.bullet1)
			draw_bullets(get_entity("player")->weapon.bullet2,"game_interface/bullet2",++layer);
		if(bullets[get_entity("player")->weapon.bullet1].max>0)
			draw_bullets(get_entity("player")->weapon.bullet1,"game_interface/bullet1",++layer);
	}
#ifdef TOUCH
	sensor::draw();
#endif
}


void MainMenu::draw() {
	if(!shown)return;
	static struct {
		string text;
		void(*func)();
		bool active;
	} buttons[]= {
		{
			text::get("main_menu/new_game"),[]() {
				load_level(interface.levelchooser.get_first_level(),true);
				interface.mainmenu.hide();
				interface.levelchooser.hide();
				interface.pause.hide();
			},0
		},
		{
			text::get("main_menu/choose_level"),[]() {
				interface.levelchooser.show();
			},0
		},
		{
			text::get("main_menu/load_game"),[]() {
				interface.saver.show(0);
			},0
		},
		{
			text::get("main_menu/settings"),[]() {
				interface.settingmanager.show();
			},0
		},
		{
			text::get("main_menu/exit_game"),[]() {
				quit();
			},0
		}
	};
	//uint16_t text_h=FC_GetLineHeight(font);
	//uint16_t pos=SH-borders.bottom-text_h*sizeof(buttons)/sizeof(*buttons);
	SetNextWindowBgAlpha(0.0f);
	float image_scale=image_h/title->h;

	float image_w=image_scale*title->w;
	GPU_BlitScale(title,0,ren,borders.left+image_w/2,borders.top+image_h/2,image_scale,image_scale);
	SetNextWindowPos({borders.left,SH-borders.bottom},ImGuiCond_FirstUseEver,{0,1});
	Begin("mainmenu",0,ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoBringToFrontOnFocus);
	PushFont(font);
	for(auto &button : buttons) {
		//GPU_Rect rect=GPU_MakeRect(borders.left,pos,FC_GetWidth(font,button.text.c_str()),text_h);
		//if(in_rect(mouse.x,mouse.y,rect)) {
			//FC_DrawColor(font,ren,borders.left,pos,active.color(),button.text.c_str());
			//if(mouse.state==1)
				//button.func();
		//} else {
			//FC_DrawColor(font,ren,borders.left,pos,inactive.color(),button.text.c_str());
		//}
		//pos+=text_h;
		if(button.active)
			TextDisabled("%s",button.text.c_str());
		else
			Text("%s",button.text.c_str());
		button.active=IsItemHovered();
		if(IsItemClicked())
			button.func();

	}
	PopFont();
	End();
}
void MainMenu::load_config() {
	XMLNode node=open_xml((prefix+"config/main_menu.xml").c_str(),"main_menu");
	{
		XMLNode text=node.getChildNode("text");
		float size=load_scaled_float(text,"size");
		font=interface.load_imgui_font(text.getAttribute("name"),size);
	}
	{
		XMLNode titlen=node.getChildNode("title");
		string str=titlen.getAttribute("image");
		image_h=load_scaled_float(titlen,"h");
		title=load_texture(str);
		textures.erase(str);
	}
	load_value(node,"border",borders);
}
MainMenu::~MainMenu() {
	if(title)
		GPU_FreeImage(title);
}
void MainMenu::hide() {
	shown=false;
	interface.update_cursor();
}
void MainMenu::show() {
	shown=true;
	interface.update_cursor();
	interface.pause.hide();
	lua::init_main_menu();
}


void LevelChooser::open_level(string id) {
	load();
	for(auto &level :levels) {
		if(level.id==id && level.show) {
			info_log("Opening "+id+" level");
			level.open=true;
			save();
			return;
		}
	}
	warning_log("Level "+id+" is not shown or not exist");
}
void LevelChooser::hide() {
	shown=false;
}
void LevelChooser::show() {
	load();
	shown=true;
}
void LevelChooser::load() {
	if(levels.size())return;
	XMLNode file;
	file=open_xml((saves+"levels.xml").c_str(),"levels");
	if(file.isEmpty()) {
		warning_log("Failed to load "+saves+"levels.xml, restoring default");
		file=open_xml((prefix+"config/levels.xml").c_str(),"levels");
	}
	int count=file.getAttributei("count");
	levels.clear();
	for(int q=0; q<count; q++) {
		XMLNode leveln=file.getChildNode("level",q);
		CLevel level;
		level.id=leveln.getAttribute("id");
		level.open=leveln.getAttributei("open");
		level.show=leveln.getAttributei("show");
		levels.push_back(level);
	}
	info_log("Loaded levels config");
	save();
}
void LevelChooser::save() {
	try {
		XMLNode file=XMLNode::createXMLTopNode("levels");
		file.addAttribute("count",levels.size());
		for(auto &level : levels) {
			XMLNode leveln=file.addChild("level");
			leveln.addAttribute("id",level.id);
			leveln.addAttribute("open",level.open);
			leveln.addAttribute("show",level.show);
		}
		file.writeToFile((saves+"levels.xml").c_str());
		info_log("Saved levels config");
	} catch(XMLError &er) {
		panic((string)"Cannot write to levels.xml: "+XMLNode::getError(er));
	} catch(...) {
		panic("Cannot write to levels.xml");
	}
}
void LevelChooser::draw() {
	if(!shown)return;
	SetNextWindowSize(ImVec2(520, 300), ImGuiCond_FirstUseEver);
	if (!config.apply(text::getc("levelchooser/title"), &shown)) {
		End();
		return;
	}
	const float footer_height_to_reserve = GetStyle().ItemSpacing.y + GetFrameHeightWithSpacing();
	BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_HorizontalScrollbar);
	for (int q=0; q<levels.size(); q++) {
		CLevel l=levels[q];
		if(!l.show)continue;
		if(l.open) {
			if(Selectable(l.id.c_str(), q==selected))
				selected=q;
		} else {
			PushStyleColor(ImGuiCol_Text, GetStyle().Colors[ImGuiCol_TextDisabled]);
			Selectable(l.id.c_str(), false);
			PopStyleColor();
		}
	}
	EndChild();
	if(Button(text::getc("levelchooser/load"))) {
		interface.mainmenu.hide();
		interface.levelchooser.hide();
		interface.pause.hide();
		load_level(levels[selected].id,true);
	}
	SameLine();
	if(Button(text::getc("common/cancel")))hide();
	SameLine();

	End();
}
string LevelChooser::get_first_level() {
	load();
	if(!levels.size())
		panic("Failed to get first level: level count = 0");
	return levels[0].id;
}
Interface interface;


///////////////////////////////////////////////////////////
using namespace luabridge;
static LuaRef *ImFlags;

static vector<const char*> luaval_to_vector_string(lua_State *L,int n) {
	vector<const char*>vec;
	int len = lua_objlen(L,n);
	for (int i=1; i <= len; i++) {
		lua_pushinteger(L,i);
		lua_gettable(L,n);
		const char *s = lua_tostring(L,-1);
		if (s)
			vec.push_back(s);

		lua_pop(L,1);
	}
	return vec;
}
static int ImBegin(lua_State *L) {
	int argi = 1;
	const char* name = lua_tostring(L, argi++);
	bool p_open = lua_isboolean(L, argi) ? lua_toboolean(L, argi++) : (bool)NULL;
	ImGuiWindowFlags flags = (ImGuiWindowFlags)luaL_optinteger(L, argi++, 0);
	bool ret = Begin(name, &p_open, flags);
	lua_pushboolean(L, ret);
	lua_pushboolean(L, p_open);
	return 2;
}
static int ImBeginChild(lua_State *L) {
	int argi = 1;
	ImGuiID id = (ImGuiID)lua_tointeger(L, argi++);
	ImVec2 size_def = ImVec2(0, 0);
	ImVec2 size;
	size.x = (float)luaL_optnumber(L, argi, size_def.x);
	size.y = (float)luaL_optnumber(L, argi + 1, size_def.y);
	if (size.x != size_def.x || size.y != size_def.y) argi += 2;
	bool border = lua_isboolean(L, argi) ? lua_toboolean(L, argi++) : (bool)false;
	ImGuiWindowFlags flags = (ImGuiWindowFlags)luaL_optinteger(L, argi++, 0);
	bool ret = BeginChild(id, size, border, flags);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImGetWindowPos(lua_State *L) {
	ImVec2 ret = GetWindowPos();
	lua_pushnumber(L, ret.x);
	lua_pushnumber(L, ret.y);
	return 2;
};
static int ImGetContentRegionMax(lua_State *L) {
	ImVec2 ret = GetContentRegionMax();
	lua_pushnumber(L, ret.x);
	lua_pushnumber(L, ret.y);
	return 2;
};
static int ImGetContentRegionAvail(lua_State *L) {
	ImVec2 ret = GetContentRegionAvail();
	lua_pushnumber(L, ret.x);
	lua_pushnumber(L, ret.y);
	return 2;
};
static int ImGetWindowContentRegionMin(lua_State *L) {
	ImVec2 ret = GetWindowContentRegionMin();
	lua_pushnumber(L, ret.x);
	lua_pushnumber(L, ret.y);
	return 2;
};
static int ImGetWindowContentRegionMax(lua_State *L) {
	ImVec2 ret = GetWindowContentRegionMax();
	lua_pushnumber(L, ret.x);
	lua_pushnumber(L, ret.y);
	return 2;
};
static int ImSetNextWindowPos(lua_State *L) {
	int argi = 1;
	ImVec2 pos;
	pos.x = (float)lua_tonumber(L, argi++);
	pos.y = (float)lua_tonumber(L, argi++);
	ImGuiCond cond = (ImGuiCond)luaL_optinteger(L, argi++, 0);
	ImVec2 pivot_def = ImVec2(0, 0);
	ImVec2 pivot;
	pivot.x = (float)luaL_optnumber(L, argi, pivot_def.x);
	pivot.y = (float)luaL_optnumber(L, argi + 1, pivot_def.y);
	if (pivot.x != pivot_def.x || pivot.y != pivot_def.y) argi += 2;
	SetNextWindowPos(pos, cond, pivot);
	return 0;
};
static int ImSetNextWindowPosCenter(lua_State *L) {
	ImGuiCond cond = (ImGuiCond)luaL_optinteger(L, 1, 0);
	SetNextWindowPosCenter(cond);
	return 0;
};
static int ImSetNextWindowSize(lua_State *L) {
	int argi = 1;
	ImVec2 size;
	size.x = (float)lua_tonumber(L, argi++);
	size.y = (float)lua_tonumber(L, argi++);
	ImGuiCond cond = (ImGuiCond)luaL_optinteger(L, argi++, 0);
	SetNextWindowSize(size, cond);
	return 0;
};
static int ImSetNextWindowCollapsed(lua_State *L) {
	int argi = 1;
	bool collapsed = lua_toboolean(L, argi++);
	ImGuiCond cond = (ImGuiCond)luaL_optinteger(L, argi++, 0);
	SetNextWindowCollapsed(collapsed, cond);
	return 0;
};
static inline void ImSetNextWindowSizeConstraints(float minx,float miny,float maxx,float maxy) {
	SetNextWindowSizeConstraints(ImVec2(minx,miny),ImVec2(maxx,maxy));
}
static inline void ImSetNextWindowContentSize(float x,float y) {
	SetNextWindowContentSize(ImVec2(x,y));
}
static int ImSetWindowPos(lua_State *L) {
	int argi = 1;
	ImVec2 pos;
	pos.x = (float)lua_tonumber(L, argi++);
	pos.y = (float)lua_tonumber(L, argi++);
	ImGuiCond cond = (ImGuiCond)luaL_optinteger(L, argi++, 0);
	SetWindowPos(pos, cond);
	return 0;
};
static int ImSetWindowSize(lua_State *L) {
	int argi = 1;
	ImVec2 size;
	size.x = (float)lua_tonumber(L, argi++);
	size.y = (float)lua_tonumber(L, argi++);
	ImGuiCond cond = (ImGuiCond)luaL_optinteger(L, argi++, 0);
	SetWindowSize(size, cond);
	return 0;
};
static int ImSetWindowCollapsed(lua_State *L) {
	int argi = 1;
	bool collapsed = lua_toboolean(L, argi++);
	ImGuiCond cond = (ImGuiCond)luaL_optinteger(L, argi++, 0);
	SetWindowCollapsed(collapsed, cond);
	return 0;
};
static inline void ImSetWindowFocus() {
	SetWindowFocus();
}
static inline void ImPushStyleColor(ImGuiCol idx, Color color) {
	ImVec4 v(color.r,color.g,color.b,color.a);
	PushStyleColor(idx,v);
}
static inline void ImPushStyleVarFloat(ImGuiStyleVar idx, float val) {
	PushStyleVar(idx,val);
}
static inline void ImPushStyleVarVec(ImGuiStyleVar idx, float x,float y) {
	PushStyleVar(idx,ImVec2(x,y));
}
static inline Color ImGetStyleColor(ImGuiCol idx) {
	ImVec4 v=GetStyleColorVec4(idx);
	return Color(v.x,v.y,v.z,v.w);
}
static inline void ImDummy(float x,float y) {
	Dummy(ImVec2(x,y));
}
static inline void ImSetCursorPos(float x,float y) {
	SetCursorPos(ImVec2(x,y));
}
static int ImGetCursorStartPos(lua_State* L) {
	ImVec2 ret = GetCursorStartPos();
	lua_pushnumber(L, ret.x);
	lua_pushnumber(L, ret.y);
	return 2;
};
static int ImGetCursorScreenPos(lua_State* L) {
	ImVec2 ret = GetCursorScreenPos();
	lua_pushnumber(L, ret.x);
	lua_pushnumber(L, ret.y);
	return 2;
};
static inline void ImSetCursorScreenPos(float x,float y) {
	SetCursorScreenPos(ImVec2(x,y));
}
static void ImPushID(const char* str_id){
	PushID(str_id);
}
static void ImPushID2(const char* str_id_begin, const char* str_id_end){
	PushID(str_id_begin,str_id_end);
}
static void ImPushID3(int int_id){
	PushID(int_id);
}
static ImGuiID ImGetID(const char* str_id){
	return GetID(str_id);
}
static ImGuiID ImGetID2(const char* str_id_begin, const char* str_id_end){
	return GetID(str_id_begin,str_id_end);
}
static inline void ImText(const char *text) {
	Text("%s",text);
}
static inline void ImTextColored(Color c,const char *text) {
	ImVec4 v(c.r,c.g,c.b,c.a);
	TextColored(v,"%s",text);
}
static inline void ImTextDisabled(const char *text) {
	TextDisabled("%s",text);
}
static inline void ImTextWrapped(const char *text) {
	TextWrapped("%s",text);
}
static inline void ImLabelText(const char *label,const char *text) {
	LabelText(label,"%s",text);
}
static inline void ImBulletText(const char *text) {
	BulletText("%s",text);
}
static int ImButton(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	ImVec2 size_def = ImVec2(0, 0);
	ImVec2 size;
	size.x = (float)luaL_optnumber(L, argi, size_def.x);
	size.y = (float)luaL_optnumber(L, argi + 1, size_def.y);
	if (size.x != size_def.x || size.y != size_def.y) argi += 2;
	bool ret = Button(label, size);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImInvisibleButton(lua_State *L) {
	int argi = 1;
	const char* str_id = lua_tostring(L, argi++);
	ImVec2 size;
	size.x = (float)lua_tonumber(L, argi++);
	size.y = (float)lua_tonumber(L, argi++);
	bool ret = InvisibleButton(str_id, size);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImImageButton(lua_State *L){
	int argi=1;
	const char* texture=lua_tostring(L,argi++);
	GPU_Image *tex=load_texture(texture);
	ImVec2 size(
		luaL_optnumber(L,argi++,tex->w),
		luaL_optnumber(L,argi++,tex->h)
	);
	ImVec2 uv0(
		luaL_optnumber(L,argi++,0.0f),
		luaL_optnumber(L,argi++,0.0f)
	);
	ImVec2 uv1(
		luaL_optinteger(L,argi++,1.0f),
		luaL_optinteger(L,argi++,1.0f)
	);
	int frame_padding=luaL_optinteger(L,argi++,-1);
	ImageButton((void*)GPU_GetTextureHandle(tex),size,uv0,uv1,frame_padding);
	return 0;
}
static int ImImage(lua_State *L){
	int argi=1;
	const char* texture=lua_tostring(L,argi++);
	GPU_Image *tex=load_texture(texture);
	ImVec2 size(
		luaL_optnumber(L,argi++,tex->w),
		luaL_optnumber(L,argi++,tex->h)
	);
	ImVec2 uv0(
		luaL_optnumber(L,argi++,0.0f),
		luaL_optnumber(L,argi++,0.0f)
	);
	ImVec2 uv1(
		luaL_optinteger(L,argi++,1.0f),
		luaL_optinteger(L,argi++,1.0f)
	);
	Image((void*)GPU_GetTextureHandle(tex),size,uv0,uv1);
	return 0;
}
static int ImCheckbox(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	bool v = lua_toboolean(L, argi++);
	bool ret = Checkbox(label, &v);
	lua_pushboolean(L, ret);
	lua_pushboolean(L, v);
	return 2;
};
static int ImCheckboxFlags(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	unsigned int flags = (unsigned int)lua_tointeger(L, argi++);
	unsigned int flags_value = (unsigned int)lua_tointeger(L, argi++);
	bool ret = CheckboxFlags(label, &flags, flags_value);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImProgressBar(lua_State *L) {
	int argi = 1;
	float fraction = (float)lua_tonumber(L, argi++);
	ImVec2 size_arg_def = ImVec2(-1, 0);
	ImVec2 size_arg;
	size_arg.x = (float)luaL_optnumber(L, argi, size_arg_def.x);
	size_arg.y = (float)luaL_optnumber(L, argi + 1, size_arg_def.y);
	if (size_arg.x != size_arg_def.x || size_arg.y != size_arg_def.y) argi += 2;
	const char* overlay = luaL_optstring(L, argi++, NULL);
	ProgressBar(fraction, size_arg, overlay);
	return 0;
};
static int ImBeginCombo(lua_State* L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	const char* preview_value = lua_tostring(L, argi++);
	ImGuiComboFlags flags = (ImGuiComboFlags)luaL_optinteger(L, argi++, 0);
	bool ret = BeginCombo(label, preview_value, flags);
	lua_pushboolean(L, ret);
	return 1;
};
static inline bool ImRadioButton(const char* label, bool active) {
	return RadioButton(label,active);
}
static int  ImRadioButton2(lua_State* L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v = (int)lua_tointeger(L, argi++);
	int v_button = (int)lua_tointeger(L, argi++);
	bool ret = RadioButton(label, &v, v_button);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v);
	return 2;
};
static int ImCombo(lua_State *L){
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int item = lua_tointeger(L, argi++);
	vector<const char*>items=luaval_to_vector_string(L,argi++);
	int popup_max_height_in_items=luaL_optinteger(L,argi++,-1);
	bool ret = Combo(label, &item, items.data(),items.size(),popup_max_height_in_items);
	lua_pushboolean(L, ret);
	lua_pushinteger(L,item);
	return 2;
}
static int ImDragFloat(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v = (float)lua_tonumber(L, argi++);
	float v_speed = (float)luaL_optnumber(L, argi++, 1.0f);
	float v_min = (float)luaL_optnumber(L, argi++, 0.0f);
	float v_max = (float)luaL_optnumber(L, argi++, 0.0f);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	float power = (float)luaL_optnumber(L, argi++, 1.0f);
	bool ret = DragFloat(label, &v, v_speed, v_min, v_max, format, power);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v);
	return 2;
};
static int ImDragFloat2(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v[2];
	v[0] = (float)lua_tonumber(L, argi++);
	v[1] = (float)lua_tonumber(L, argi++);
	float v_speed = (float)luaL_optnumber(L, argi++, 1.0f);
	float v_min = (float)luaL_optnumber(L, argi++, 0.0f);
	float v_max = (float)luaL_optnumber(L, argi++, 0.0f);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	float power = (float)luaL_optnumber(L, argi++, 1.0f);
	bool ret = DragFloat2(label, v, v_speed, v_min, v_max, format, power);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v[0]);
	lua_pushnumber(L, v[1]);
	return 3;
};
static int ImDragFloat3(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v[3];
	v[0] = (float)lua_tonumber(L, argi++);
	v[1] = (float)lua_tonumber(L, argi++);
	v[2] = (float)lua_tonumber(L, argi++);
	float v_speed = (float)luaL_optnumber(L, argi++, 1.0f);
	float v_min = (float)luaL_optnumber(L, argi++, 0.0f);
	float v_max = (float)luaL_optnumber(L, argi++, 0.0f);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	float power = (float)luaL_optnumber(L, argi++, 1.0f);
	bool ret = DragFloat3(label, v, v_speed, v_min, v_max, format, power);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v[0]);
	lua_pushnumber(L, v[1]);
	lua_pushnumber(L, v[2]);
	return 4;
};
static int ImDragFloat4(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v[4];
	v[0] = (float)lua_tonumber(L, argi++);
	v[1] = (float)lua_tonumber(L, argi++);
	v[2] = (float)lua_tonumber(L, argi++);
	v[3] = (float)lua_tonumber(L, argi++);
	float v_speed = (float)luaL_optnumber(L, argi++, 1.0f);
	float v_min = (float)luaL_optnumber(L, argi++, 0.0f);
	float v_max = (float)luaL_optnumber(L, argi++, 0.0f);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	float power = (float)luaL_optnumber(L, argi++, 1.0f);
	bool ret = DragFloat4(label, v, v_speed, v_min, v_max, format, power);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v[0]);
	lua_pushnumber(L, v[1]);
	lua_pushnumber(L, v[2]);
	lua_pushnumber(L, v[3]);
	return 5;
};
static int ImDragFloatRange2(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v_current_min = (float)lua_tonumber(L, argi++);
	float v_current_max = (float)lua_tonumber(L, argi++);
	float v_speed = (float)luaL_optnumber(L, argi++, 1.0f);
	float v_min = (float)luaL_optnumber(L, argi++, 0.0f);
	float v_max = (float)luaL_optnumber(L, argi++, 0.0f);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	const char* format_max = luaL_optstring(L, argi++, NULL);
	float power = (float)luaL_optnumber(L, argi++, 1.0f);
	bool ret = DragFloatRange2(label, &v_current_min, &v_current_max, v_speed, v_min, v_max, format, format_max, power);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v_current_min);
	lua_pushnumber(L, v_current_max);
	return 3;
};
static int ImDragInt(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v = (int)lua_tointeger(L, argi++);
	float v_speed = (float)luaL_optnumber(L, argi++, 1.0f);
	int v_min = (int)luaL_optinteger(L, argi++, 0);
	int v_max = (int)luaL_optinteger(L, argi++, 0);
	const char* format = luaL_optstring(L, argi++, "%d");
	bool ret = DragInt(label, &v, v_speed, v_min, v_max, format);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v);
	return 2;
};
static int ImDragInt2(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v[2];
	v[0] = (int)lua_tointeger(L, argi++);
	v[1] = (int)lua_tointeger(L, argi++);
	float v_speed = (float)luaL_optnumber(L, argi++, 1.0f);
	int v_min = (int)luaL_optinteger(L, argi++, 0);
	int v_max = (int)luaL_optinteger(L, argi++, 0);
	const char* format = luaL_optstring(L, argi++, "%d");
	bool ret = DragInt2(label, v, v_speed, v_min, v_max, format);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v[0]);
	lua_pushinteger(L, v[1]);
	return 3;
};
static int ImDragInt3(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v[3];
	v[0] = (int)lua_tointeger(L, argi++);
	v[1] = (int)lua_tointeger(L, argi++);
	v[2] = (int)lua_tointeger(L, argi++);
	float v_speed = (float)luaL_optnumber(L, argi++, 1.0f);
	int v_min = (int)luaL_optinteger(L, argi++, 0);
	int v_max = (int)luaL_optinteger(L, argi++, 0);
	const char* format = luaL_optstring(L, argi++, "%d");
	bool ret = DragInt3(label, v, v_speed, v_min, v_max, format);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v[0]);
	lua_pushinteger(L, v[1]);
	lua_pushinteger(L, v[2]);
	return 4;
};
static int ImDragInt4(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v[4];
	v[0] = (int)lua_tointeger(L, argi++);
	v[1] = (int)lua_tointeger(L, argi++);
	v[2] = (int)lua_tointeger(L, argi++);
	v[3] = (int)lua_tointeger(L, argi++);
	float v_speed = (float)luaL_optnumber(L, argi++, 1.0f);
	int v_min = (int)luaL_optinteger(L, argi++, 0);
	int v_max = (int)luaL_optinteger(L, argi++, 0);
	const char* format = luaL_optstring(L, argi++, "%d");
	bool ret = DragInt4(label, v, v_speed, v_min, v_max, format);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v[0]);
	lua_pushinteger(L, v[1]);
	lua_pushinteger(L, v[2]);
	lua_pushinteger(L, v[3]);
	return 5;
};
static int ImDragIntRange2(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v_current_min = (int)lua_tointeger(L, argi++);
	int v_current_max = (int)lua_tointeger(L, argi++);
	float v_speed = (float)luaL_optnumber(L, argi++, 1.0f);
	int v_min = (int)luaL_optinteger(L, argi++, 0);
	int v_max = (int)luaL_optinteger(L, argi++, 0);
	const char* format = luaL_optstring(L, argi++, "%d");
	const char* format_max = luaL_optstring(L, argi++, NULL);
	bool ret = DragIntRange2(label, &v_current_min, &v_current_max, v_speed, v_min, v_max, format, format_max);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v_current_min);
	lua_pushinteger(L, v_current_max);
	return 3;
};
static int ImSliderFloat(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v = (float)lua_tonumber(L, argi++);
	float v_min = (float)lua_tonumber(L, argi++);
	float v_max = (float)lua_tonumber(L, argi++);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	float power = (float)luaL_optnumber(L, argi++, 1.0f);
	bool ret = SliderFloat(label, &v, v_min, v_max, format, power);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v);
	return 2;
};
static int ImSliderFloat2(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v[2];
	v[0] = (float)lua_tonumber(L, argi++);
	v[1] = (float)lua_tonumber(L, argi++);
	float v_min = (float)lua_tonumber(L, argi++);
	float v_max = (float)lua_tonumber(L, argi++);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	float power = (float)luaL_optnumber(L, argi++, 1.0f);
	bool ret = SliderFloat2(label, v, v_min, v_max, format, power);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v[0]);
	lua_pushnumber(L, v[1]);
	return 3;
};
static int ImSliderFloat3(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v[3];
	v[0] = (float)lua_tonumber(L, argi++);
	v[1] = (float)lua_tonumber(L, argi++);
	v[2] = (float)lua_tonumber(L, argi++);
	float v_min = (float)lua_tonumber(L, argi++);
	float v_max = (float)lua_tonumber(L, argi++);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	float power = (float)luaL_optnumber(L, argi++, 1.0f);
	bool ret = SliderFloat3(label, v, v_min, v_max, format, power);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v[0]);
	lua_pushnumber(L, v[1]);
	lua_pushnumber(L, v[2]);
	return 4;
};
static int ImSliderFloat4(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v[4];
	v[0] = (float)lua_tonumber(L, argi++);
	v[1] = (float)lua_tonumber(L, argi++);
	v[2] = (float)lua_tonumber(L, argi++);
	v[3] = (float)lua_tonumber(L, argi++);
	float v_min = (float)lua_tonumber(L, argi++);
	float v_max = (float)lua_tonumber(L, argi++);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	float power = (float)luaL_optnumber(L, argi++, 1.0f);
	bool ret = SliderFloat4(label, v, v_min, v_max, format, power);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v[0]);
	lua_pushnumber(L, v[1]);
	lua_pushnumber(L, v[2]);
	lua_pushnumber(L, v[3]);
	return 5;
};
static int ImSliderAngle(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v_rad = (float)lua_tonumber(L, argi++);
	float v_degrees_min = (float)luaL_optnumber(L, argi++, -360.0f);
	float v_degrees_max = (float)luaL_optnumber(L, argi++, +360.0f);
	bool ret = SliderAngle(label, &v_rad, v_degrees_min, v_degrees_max);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v_rad);
	return 2;
};
static int ImSliderInt(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v = (int)lua_tointeger(L, argi++);
	int v_min = (int)lua_tointeger(L, argi++);
	int v_max = (int)lua_tointeger(L, argi++);
	const char* format = luaL_optstring(L, argi++, "%d");
	bool ret = SliderInt(label, &v, v_min, v_max, format);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v);
	return 2;
};
static int ImSliderInt2(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v[2];
	v[0] = (int)lua_tointeger(L, argi++);
	v[1] = (int)lua_tointeger(L, argi++);
	int v_min = (int)lua_tointeger(L, argi++);
	int v_max = (int)lua_tointeger(L, argi++);
	const char* format = luaL_optstring(L, argi++, "%d");
	bool ret = SliderInt2(label, v, v_min, v_max, format);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v[0]);
	lua_pushinteger(L, v[1]);
	return 3;
};
static int ImSliderInt3(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v[3];
	v[0] = (int)lua_tointeger(L, argi++);
	v[1] = (int)lua_tointeger(L, argi++);
	v[2] = (int)lua_tointeger(L, argi++);
	int v_min = (int)lua_tointeger(L, argi++);
	int v_max = (int)lua_tointeger(L, argi++);
	const char* format = luaL_optstring(L, argi++, "%d");
	bool ret = SliderInt3(label, v, v_min, v_max, format);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v[0]);
	lua_pushinteger(L, v[1]);
	lua_pushinteger(L, v[2]);
	return 4;
};
static int ImSliderInt4(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v[4];
	v[0] = (int)lua_tointeger(L, argi++);
	v[1] = (int)lua_tointeger(L, argi++);
	v[2] = (int)lua_tointeger(L, argi++);
	v[3] = (int)lua_tointeger(L, argi++);
	int v_min = (int)lua_tointeger(L, argi++);
	int v_max = (int)lua_tointeger(L, argi++);
	const char* format = luaL_optstring(L, argi++, "%d");
	bool ret = SliderInt4(label, v, v_min, v_max, format);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v[0]);
	lua_pushinteger(L, v[1]);
	lua_pushinteger(L, v[2]);
	lua_pushinteger(L, v[3]);
	return 5;
};
static int ImVSliderFloat(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	ImVec2 size;
	size.x = (float)lua_tonumber(L, argi++);
	size.y = (float)lua_tonumber(L, argi++);
	float v = (float)lua_tonumber(L, argi++);
	float v_min = (float)lua_tonumber(L, argi++);
	float v_max = (float)lua_tonumber(L, argi++);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	float power = (float)luaL_optnumber(L, argi++, 1.0f);
	bool ret = VSliderFloat(label, size, &v, v_min, v_max, format, power);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v);
	return 2;
};
static int ImVSliderInt(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	ImVec2 size;
	size.x = (float)lua_tonumber(L, argi++);
	size.y = (float)lua_tonumber(L, argi++);
	int v = (int)lua_tointeger(L, argi++);
	int v_min = (int)lua_tointeger(L, argi++);
	int v_max = (int)lua_tointeger(L, argi++);
	const char* format = luaL_optstring(L, argi++, "%d");
	bool ret = VSliderInt(label, size, &v, v_min, v_max, format);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v);
	return 2;
};
static int ImInputText(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	char buf[1024];
	strcpy(buf,lua_tostring(L, argi++));
	ImGuiInputTextFlags flags = (ImGuiInputTextFlags)luaL_optinteger(L, argi++, 0);
	bool ret = InputText(label,buf,1024,flags);
	lua_pushboolean(L, ret);
	lua_pushstring(L,buf);
	return 2;
};
static int ImInputTextMultiline(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	char buf[1024];
	strcpy(buf,lua_tostring(L, argi++));
	ImVec2 size;
	size.x = (float)luaL_optnumber(L, argi++,0.0f);
	size.y = (float)luaL_optnumber(L, argi++,0.0f);
	ImGuiInputTextFlags flags = (ImGuiInputTextFlags)luaL_optinteger(L, argi++, 0);
	bool ret = InputTextMultiline(label,buf,1024,size,flags);
	lua_pushboolean(L, ret);
	lua_pushstring(L,buf);
	return 2;
};
static int ImInputFloat(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v = (float)lua_tonumber(L, argi++);
	float step = (float)luaL_optnumber(L, argi++, 0.0f);
	float step_fast = (float)luaL_optnumber(L, argi++, 0.0f);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	ImGuiInputTextFlags flags = (ImGuiInputTextFlags)luaL_optinteger(L, argi++, 0);
	bool ret = InputFloat(label, &v, step, step_fast, format, flags);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v);
	return 2;
};
static int ImInputFloat2(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v[2];
	v[0] = (float)lua_tonumber(L, argi++);
	v[1] = (float)lua_tonumber(L, argi++);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	ImGuiInputTextFlags flags = (ImGuiInputTextFlags)luaL_optinteger(L, argi++, 0);
	bool ret = InputFloat2(label, v, format, flags);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v[0]);
	lua_pushnumber(L, v[1]);
	return 3;
};
static int ImInputFloat3(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v[3];
	v[0] = (float)lua_tonumber(L, argi++);
	v[1] = (float)lua_tonumber(L, argi++);
	v[2] = (float)lua_tonumber(L, argi++);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	ImGuiInputTextFlags flags = (ImGuiInputTextFlags)luaL_optinteger(L, argi++, 0);
	bool ret = InputFloat3(label, v, format, flags);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v[0]);
	lua_pushnumber(L, v[1]);
	lua_pushnumber(L, v[2]);
	return 4;
};
static int ImInputFloat4(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float v[4];
	v[0] = (float)lua_tonumber(L, argi++);
	v[1] = (float)lua_tonumber(L, argi++);
	v[2] = (float)lua_tonumber(L, argi++);
	v[3] = (float)lua_tonumber(L, argi++);
	const char* format = luaL_optstring(L, argi++, "%.3f");
	ImGuiInputTextFlags flags = (ImGuiInputTextFlags)luaL_optinteger(L, argi++, 0);
	bool ret = InputFloat4(label, v, format, flags);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, v[0]);
	lua_pushnumber(L, v[1]);
	lua_pushnumber(L, v[2]);
	lua_pushnumber(L, v[3]);
	return 5;
};
static int ImInputInt(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v = (int)lua_tointeger(L, argi++);
	int step = (int)luaL_optinteger(L, argi++, 1);
	int step_fast = (int)luaL_optinteger(L, argi++, 100);
	ImGuiInputTextFlags flags = (ImGuiInputTextFlags)luaL_optinteger(L, argi++, 0);
	bool ret = InputInt(label, &v, step, step_fast, flags);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v);
	return 2;
};
static int ImInputInt2(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v[2];
	v[0] = (int)lua_tointeger(L, argi++);
	v[1] = (int)lua_tointeger(L, argi++);
	ImGuiInputTextFlags flags = (ImGuiInputTextFlags)luaL_optinteger(L, argi++, 0);
	bool ret = InputInt2(label, v, flags);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v[0]);
	lua_pushinteger(L, v[1]);
	return 3;
};
static int ImInputInt3(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v[3];
	v[0] = (int)lua_tointeger(L, argi++);
	v[1] = (int)lua_tointeger(L, argi++);
	v[2] = (int)lua_tointeger(L, argi++);
	ImGuiInputTextFlags flags = (ImGuiInputTextFlags)luaL_optinteger(L, argi++, 0);
	bool ret = InputInt3(label, v, flags);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v[0]);
	lua_pushinteger(L, v[1]);
	lua_pushinteger(L, v[2]);
	return 4;
};
static int ImInputInt4(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int v[4];
	v[0] = (int)lua_tointeger(L, argi++);
	v[1] = (int)lua_tointeger(L, argi++);
	v[2] = (int)lua_tointeger(L, argi++);
	v[3] = (int)lua_tointeger(L, argi++);
	ImGuiInputTextFlags flags = (ImGuiInputTextFlags)luaL_optinteger(L, argi++, 0);
	bool ret = InputInt4(label, v, flags);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, v[0]);
	lua_pushinteger(L, v[1]);
	lua_pushinteger(L, v[2]);
	lua_pushinteger(L, v[3]);
	return 5;
};
static int ImColorEdit3(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float col[3];
	col[0] = (float)lua_tonumber(L, argi++);
	col[1] = (float)lua_tonumber(L, argi++);
	col[2] = (float)lua_tonumber(L, argi++);
	ImGuiColorEditFlags flags = (ImGuiColorEditFlags)luaL_optinteger(L, argi++, 0);
	bool ret = ColorEdit3(label, col, flags);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, col[0]);
	lua_pushnumber(L, col[1]);
	lua_pushnumber(L, col[2]);
	return 4;
};
static int ImColorEdit4(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float col[4];
	col[0] = (float)lua_tonumber(L, argi++);
	col[1] = (float)lua_tonumber(L, argi++);
	col[2] = (float)lua_tonumber(L, argi++);
	col[3] = (float)lua_tonumber(L, argi++);
	ImGuiColorEditFlags flags = (ImGuiColorEditFlags)luaL_optinteger(L, argi++, 0);
	bool ret = ColorEdit4(label, col, flags);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, col[0]);
	lua_pushnumber(L, col[1]);
	lua_pushnumber(L, col[2]);
	lua_pushnumber(L, col[3]);
	return 5;
};
static int ImColorPicker3(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float col[3];
	col[0] = (float)lua_tonumber(L, argi++);
	col[1] = (float)lua_tonumber(L, argi++);
	col[2] = (float)lua_tonumber(L, argi++);
	ImGuiColorEditFlags flags = (ImGuiColorEditFlags)luaL_optinteger(L, argi++, 0);
	bool ret = ColorPicker3(label, col, flags);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, col[0]);
	lua_pushnumber(L, col[1]);
	lua_pushnumber(L, col[2]);
	return 4;
};
static int ImColorPicker4(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	float col[4];
	col[0] = (float)lua_tonumber(L, argi++);
	col[1] = (float)lua_tonumber(L, argi++);
	col[2] = (float)lua_tonumber(L, argi++);
	col[3] = (float)lua_tonumber(L, argi++);
	ImGuiColorEditFlags flags = (ImGuiColorEditFlags)luaL_optinteger(L, argi++, 0);
	float ref_col = (float)luaL_optnumber(L, argi++, 0.0f);
	bool ret = ColorPicker4(label, col, flags, &ref_col);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, col[0]);
	lua_pushnumber(L, col[1]);
	lua_pushnumber(L, col[2]);
	lua_pushnumber(L, col[3]);
	return 5;
};
static int ImColorButton(lua_State *L) {
	int argi = 1;
	const char* desc_id = lua_tostring(L, argi++);
	ImVec4 col;
	col.x = (float)lua_tonumber(L, argi++);
	col.y = (float)lua_tonumber(L, argi++);
	col.z = (float)lua_tonumber(L, argi++);
	col.w = (float)lua_tonumber(L, argi++);
	ImGuiColorEditFlags flags = (ImGuiColorEditFlags)luaL_optinteger(L, argi++, 0);
	ImVec2 size_def = ImVec2(0, 0);
	ImVec2 size;
	size.x = (float)luaL_optnumber(L, argi, size_def.x);
	size.y = (float)luaL_optnumber(L, argi + 1, size_def.y);
	if (size.x != size_def.x || size.y != size_def.y) argi += 2;
	bool ret = ColorButton(desc_id, col, flags, size);
	lua_pushboolean(L, ret);
	return 1;
};
static bool ImTreeNode(const char *label) {
	return TreeNode(label);
}
static int ImTreeNodeEx(lua_State *L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	ImGuiTreeNodeFlags flags = (ImGuiTreeNodeFlags)luaL_optinteger(L, argi++, 0);
	bool ret = TreeNodeEx(label, flags);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImTreePush(lua_State *L) {
	int argi = 1;
	const char* id = luaL_optstring(L, argi++,0);
	TreePush(id);
	return 0;
};
static int ImSetNextTreeNodeOpen(lua_State *L) {
	int argi = 1;
	bool open = lua_toboolean(L, argi++);
	ImGuiCond cond = (ImGuiCond)luaL_optinteger(L, argi++, 0);
	SetNextTreeNodeOpen(open,cond);
	return 0;
};
static int ImSelectable(lua_State *L) {
	int argi = 1;
	const char *label=lua_tostring(L, argi++);
	bool selected = lua_toboolean(L, argi++);
	ImVec2 size;
	size.x = (float)luaL_optnumber(L, argi++,0.0f);
	size.y = (float)luaL_optnumber(L, argi++,0.0f);
	ImGuiSelectableFlags flags=(ImGuiSelectableFlags)luaL_optinteger(L,argi++,0);
	bool ret=Selectable(label,selected,flags,size);
	lua_pushboolean(L,ret);
	return 1;
};
static int ImListBox(lua_State *L) {
	auto label = luaL_checkstring(L, 1);
	int current_item = luaL_checkinteger(L, 2);
	vector<const char*> items=luaval_to_vector_string(L,3);
	lua_pushboolean(L,
					ListBox(label, &current_item, items.data(), items.size(),
								   luaL_optinteger(L, 4, -1)));
	lua_pushinteger(L, current_item);
	return 2;
}
static int ImListBoxHeader(lua_State* L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	ImVec2 size_def = ImVec2(0, 0);
	ImVec2 size;
	size.x = (float)luaL_optnumber(L, argi, size_def.x);
	size.y = (float)luaL_optnumber(L, argi + 1, size_def.y);
	if (size.x != size_def.x || size.y != size_def.y) argi += 2;
	bool ret = ListBoxHeader(label, size);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImListBoxHeader2(lua_State* L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	int items_count = (int)lua_tointeger(L, argi++);
	int height_in_items = (int)luaL_optinteger(L, argi++, -1);
	bool ret = ListBoxHeader(label, items_count, height_in_items);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImCollapsingHeader(lua_State* L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	if(lua_isboolean(L,argi)){
		bool p_open=lua_toboolean(L,argi++);
		ImGuiTreeNodeFlags flags=(ImGuiTreeNodeFlags)luaL_optinteger(L,argi++,0);
		bool ret = CollapsingHeader(label,&p_open,flags);
		lua_pushboolean(L, ret);
		lua_pushboolean(L, p_open);
		return 2;
	}else{
		ImGuiTreeNodeFlags flags=(ImGuiTreeNodeFlags)luaL_optinteger(L,argi++,0);
		bool ret = CollapsingHeader(label, flags);
		lua_pushboolean(L, ret);
		return 1;
	}
};
static int ImBeginMenu(lua_State* L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	bool enabled = lua_isboolean(L, argi) ? lua_toboolean(L, argi++) : (bool)true;
	bool ret = BeginMenu(label, enabled);
	lua_pushboolean(L, ret);
	return 1;
};
static void ImSetTooltip(const char *text){
	SetTooltip("%s",text);
}
static int ImBeginPopup(lua_State* L) {
	int argi = 1;
	const char* str_id = lua_tostring(L, argi++);
	ImGuiWindowFlags flags = (ImGuiWindowFlags)luaL_optinteger(L, argi++, 0);
	bool ret = BeginPopup(str_id, flags);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImBeginPopupContextItem(lua_State* L) {
	int argi = 1;
	const char* str_id = luaL_optstring(L, argi++, NULL);
	int mouse_button = luaL_optinteger(L, argi++, 1);
	bool ret = BeginPopupContextItem(str_id, mouse_button);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImBeginPopupContextWindow(lua_State* L) {
	int argi = 1;
	const char* str_id = luaL_optstring(L, argi++, NULL);
	int mouse_button = luaL_optinteger(L, argi++, 1);
	bool also_over_items = lua_isboolean(L, argi) ? lua_toboolean(L, argi++) : (bool)true;
	bool ret = BeginPopupContextWindow(str_id, mouse_button, also_over_items);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImBeginPopupContextVoid(lua_State* L) {
	int argi = 1;
	const char* str_id = luaL_optstring(L, argi++, NULL);
	int mouse_button = luaL_optinteger(L, argi++, 1);
	bool ret = BeginPopupContextVoid(str_id, mouse_button);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImBeginPopupModal(lua_State* L) {
	int argi = 1;
	const char* name = lua_tostring(L, argi++);
	bool p_open = lua_isboolean(L, argi) ? lua_toboolean(L, argi++) : (bool)NULL;
	ImGuiWindowFlags flags = (ImGuiWindowFlags)luaL_optinteger(L, argi++, 0);
	bool ret = BeginPopupModal(name, &p_open, flags);
	lua_pushboolean(L, ret);
	lua_pushboolean(L, p_open);
	return 2;
};
static int ImMenuItem(lua_State* L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	const char* shortcut = luaL_optstring(L, argi++, NULL);
	bool selected = lua_isboolean(L, argi) ? lua_toboolean(L, argi++) : (bool)false;
	bool enabled = lua_isboolean(L, argi) ? lua_toboolean(L, argi++) : (bool)true;
	bool ret = MenuItem(label, shortcut, selected, enabled);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImMenuItem2(lua_State* L) {
	int argi = 1;
	const char* label = lua_tostring(L, argi++);
	const char* shortcut = lua_tostring(L, argi++);
	bool p_selected = lua_toboolean(L, argi++);
	bool enabled = lua_isboolean(L, argi) ? lua_toboolean(L, argi++) : (bool)true;
	bool ret = MenuItem(label, shortcut, &p_selected, enabled);
	lua_pushboolean(L, ret);
	lua_pushboolean(L, p_selected);
	return 2;
};
static int ImOpenPopupOnItemClick(lua_State* L) {
	int argi = 1;
	const char* str_id = luaL_optstring(L, argi++, NULL);
	int mouse_button = luaL_optinteger(L, argi++, 1);
	bool ret = OpenPopupOnItemClick(str_id, mouse_button);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImColumns(lua_State* L) {
	int argi = 1;
	int count = (int)luaL_optinteger(L, argi++, 1);
	const char* id = luaL_optstring(L, argi++, NULL);
	bool border = lua_isboolean(L, argi) ? lua_toboolean(L, argi++) : (bool)true;
	Columns(count, id, border);
	return 0;
};
static int ImGetColumnWidth(lua_State* L) {
	int argi = 1;
	int column_index = (int)luaL_optinteger(L, argi++, -1);
	float ret = GetColumnWidth(column_index);
	lua_pushnumber(L, ret);
	return 1;
};
static int ImGetColumnOffset(lua_State* L) {
	int argi = 1;
	int column_index = (int)luaL_optinteger(L, argi++, -1);
	float ret = GetColumnOffset(column_index);
	lua_pushnumber(L, ret);
	return 1;
};
static int ImPushClipRect(lua_State* L) {
	int argi = 1;
	ImVec2 clip_rect_min;
	clip_rect_min.x = (float)lua_tonumber(L, argi++);
	clip_rect_min.y = (float)lua_tonumber(L, argi++);
	ImVec2 clip_rect_max;
	clip_rect_max.x = (float)lua_tonumber(L, argi++);
	clip_rect_max.y = (float)lua_tonumber(L, argi++);
	bool intersect_with_current_clip_rect = lua_isboolean(L, argi) ? lua_toboolean(L, argi++) : (bool)false;
	PushClipRect(clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
	return 0;
};
static int ImSetKeyboardFocusHere(lua_State* L) {
	int argi = 1;
	int offset = (int)luaL_optinteger(L, argi++, 0);
	SetKeyboardFocusHere(offset);
	return 0;
};
static int ImIsItemHovered(lua_State* L) {
	int argi = 1;
	ImGuiHoveredFlags flags = (ImGuiHoveredFlags)luaL_optinteger(L, argi++, 0);
	bool ret = IsItemHovered(flags);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImIsItemClicked(lua_State* L) {
	int argi = 1;
	int mouse_button = luaL_optinteger(L, argi++, 0);
	bool ret = IsItemClicked(mouse_button);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImGetItemRectMin(lua_State* L) {
	ImVec2 ret = GetItemRectMin();
	lua_pushnumber(L, ret.x);
	lua_pushnumber(L, ret.y);
	return 2;
};
static int ImGetItemRectMax(lua_State* L) {
	ImVec2 ret = GetItemRectMax();
	lua_pushnumber(L, ret.x);
	lua_pushnumber(L, ret.y);
	return 2;
};
static int ImGetItemRectSize(lua_State* L) {
	ImVec2 ret = GetItemRectSize();
	lua_pushnumber(L, ret.x);
	lua_pushnumber(L, ret.y);
	return 2;
};
static int ImIsRectVisible(lua_State* L) {
	int argi = 1;
	ImVec2 size;
	size.x = (float)lua_tonumber(L, argi++);
	size.y = (float)lua_tonumber(L, argi++);
	bool ret = IsRectVisible(size);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImIsRectVisible2(lua_State* L) {
	int argi = 1;
	ImVec2 rect_min;
	rect_min.x = (float)lua_tonumber(L, argi++);
	rect_min.y = (float)lua_tonumber(L, argi++);
	ImVec2 rect_max;
	rect_max.x = (float)lua_tonumber(L, argi++);
	rect_max.y = (float)lua_tonumber(L, argi++);
	bool ret = IsRectVisible(rect_min, rect_max);
	lua_pushboolean(L, ret);
	return 1;
};
static int ImCalcTextSize(lua_State* L) {
	int argi = 1;
	const char* text = lua_tostring(L, argi++);
	const char* text_end = luaL_optstring(L, argi++, NULL);
	bool hide_text_after_double_hash = lua_isboolean(L, argi) ? lua_toboolean(L, argi++) : (bool)false;
	float wrap_width = (float)luaL_optnumber(L, argi++, -1.0f);
	ImVec2 ret = CalcTextSize(text, text_end, hide_text_after_double_hash, wrap_width);
	lua_pushnumber(L, ret.x);
	lua_pushnumber(L, ret.y);
	return 2;
};
static int ImCalcListClipping(lua_State* L) {
	int argi = 1;
	int items_count = (int)lua_tointeger(L, argi++);
	float items_height = (float)lua_tonumber(L, argi++);
	int out_items_display_start = (int)lua_tointeger(L, argi++);
	int out_items_display_end = (int)lua_tointeger(L, argi++);
	CalcListClipping(items_count, items_height, &out_items_display_start, &out_items_display_end);
	lua_pushinteger(L, out_items_display_start);
	lua_pushinteger(L, out_items_display_end);
	return 2;
};
static int ImBeginChildFrame(lua_State* L) {
	int argi = 1;
	ImGuiID id = (ImGuiID)lua_tointeger(L, argi++);
	ImVec2 size;
	size.x = (float)lua_tonumber(L, argi++);
	size.y = (float)lua_tonumber(L, argi++);
	ImGuiWindowFlags flags = (ImGuiWindowFlags)luaL_optinteger(L, argi++, 0);
	bool ret = BeginChildFrame(id, size, flags);
	lua_pushboolean(L, ret);
	return 1;
};
static ImFont *ImLoadFont(string name,float size) {
	return interface.load_imgui_font(name,size);
}
void bind_imgui() {
	ImFlags=new LuaRef(newTable(lua::L));
#define BN .beginNamespace
#define EN .endNamespace()
#define F .addFunction
#define BC .beginClass
#define EC .endClass()
#define DC .deriveClass
#define P .addProperty
#define C .addConstructor
#define V .addVariable
#define VC(a,b) .addVariable(a,b,0)
	getGlobalNamespace(lua::L)
	BN("ImGui")
	BC<ImFont>("Font")
	EC
	F("GetVersion",GetVersion)
	F("Begin",ImBegin)
	F("End",End)
	F("BeginChild",ImBeginChild)
	F("EndChild",EndChild)
	F("IsWindowAppearing",IsWindowAppearing)
	F("IsWindowCollapsed",IsWindowCollapsed)
	F("IsWindowFocused",IsWindowFocused)
	F("IsWindowHovered",IsWindowHovered)
	F("GetWindowDrawList",GetWindowDrawList)
	F("GetWindowWidth",GetWindowWidth)
	F("GetWindowHeight",GetWindowHeight)
	F("GetContentRegionMax",ImGetContentRegionMax)
	F("GetContentRegionAvail",ImGetContentRegionAvail)
	F("GetWindowContentRegionMin",ImGetWindowContentRegionMin)
	F("GetWindowContentRegionMax",ImGetWindowContentRegionMax)
	F("GetWindowContentRegionWidth",GetWindowContentRegionWidth)
	F("SetNextWindowPos",ImSetNextWindowPos)
	F("SetNextWindowPosCenter",ImSetNextWindowPosCenter)
	F("SetNextWindowSize",ImSetNextWindowSize)
	F("SetNextWindowSizeConstraints",ImSetNextWindowSizeConstraints)
	F("SetNextWindowContentSize",ImSetNextWindowContentSize)
	F("SetNextWindowCollapsed",SetNextWindowCollapsed)
	F("SetNextWindowFocus",SetNextWindowFocus)
	F("SetNextWindowBgAlpha",SetNextWindowBgAlpha)
	F("SetWindowPos",ImSetWindowPos)
	F("SetWindowSize",ImSetWindowSize)
	F("SetWindowCollapsed",ImSetWindowCollapsed)
	F("SetWindowFocus",ImSetWindowFocus)
	F("SetWindowFontScale",SetWindowFontScale)
	F("GetScrollX",GetScrollX)
	F("GetScrollY",GetScrollY)
	F("GetScrollMaxX",GetScrollMaxX)
	F("GetScrollMaxY",GetScrollMaxY)
	F("SetScrollX",SetScrollX)
	F("SetScrollY",SetScrollY)
	F("SetScrollHere",SetScrollHere)
	F("SetScrollFromPosY",SetScrollFromPosY)
	F("PushFont",PushFont)
	F("PopFont",PopFont)
	F("PushStyleColor",ImPushStyleColor)
	F("PopStyleColor",PopStyleColor)
	F("PushStyleVarFloat",ImPushStyleVarFloat)
	F("PushStyleVarVec",ImPushStyleVarVec)
	F("PopStyleVar",PopStyleVar)
	F("GetStyleColor",ImGetStyleColor)
	F("GetFont",GetFont)
	F("GetFontSize",GetFontSize)
	F("PushItemWidth",PushItemWidth)
	F("PopItemWidth",PopItemWidth)
	F("CalcItemWidth",CalcItemWidth)
	F("PushTextWrapPos",PushTextWrapPos)
	F("PopTextWrapPos",PopTextWrapPos)
	F("PushAllowKeyboardFocus",PushAllowKeyboardFocus)
	F("PopAllowKeyboardFocus",PopAllowKeyboardFocus)
	F("PushButtonRepeat",PushButtonRepeat)
	F("PopButtonRepeat",PopButtonRepeat)
	F("Separator",Separator)
	F("SameLine",SameLine)
	F("NewLine",NewLine)
	F("Spacing",Spacing)
	F("Dummy",ImDummy)
	F("Indent",Indent)
	F("Unindent",Unindent)
	F("BeginGroup",BeginGroup)
	F("EndGroup",EndGroup)
	F("GetCursorPos",GetCursorPos)
	F("SetCursorPos",SetCursorPos)
	F("GetCursorStartPos",ImGetCursorStartPos)
	F("GetCursorScreenPos",ImGetCursorScreenPos)
	F("SetCursorScreenPos",ImGetCursorScreenPos)
	F("AlignTextToFramePadding",AlignTextToFramePadding)
	F("GetTextLineHeight",GetTextLineHeight)
	F("GetTextLineHeightWithSpacing",GetTextLineHeightWithSpacing)
	F("GetFrameHeight",GetFrameHeight)
	F("GetFrameHeightWithSpacing",GetFrameHeightWithSpacing)
	F("PushID",ImPushID)
	F("PushID2",ImPushID2)
	F("PushID3",ImPushID3)
	F("PopID",PopID)
	F("GetID",ImGetID)
	F("GetID2",ImGetID2)
	F("Text",ImText)
	F("TextColored",ImTextColored)
	F("TextDisabled",ImTextDisabled)
	F("TextWrapped",ImTextWrapped)
	F("LabelText",ImLabelText)
	F("BulletText",ImBulletText)
	F("Button",ImButton)
	F("SmallButton",SmallButton)
	F("InvisibleButton",InvisibleButton)
	F("ArrowButton",ArrowButton)
	F("Image",ImImage)
	F("ImageButton",ImImageButton)
	F("Checkbox",ImCheckbox)
	F("RadioButton",ImRadioButton)
	F("RadioButton2",ImRadioButton2)
	F("ProgressBar",ImProgressBar)
	F("Bullet",ImGui::Bullet)
	F("BeginCombo",ImBeginCombo)
	F("EndCombo",EndCombo)
	F("Combo",ImCombo)
	F("DragFloat",ImDragFloat)
	F("DragFloat2",ImDragFloat2)
	F("DragFloat3",ImDragFloat3)
	F("DragFloat4",ImDragFloat4)
	F("DragFloatRange2",ImDragFloatRange2)
	F("DragInt",ImDragInt)
	F("DragInt2",ImDragInt2)
	F("DragInt3",ImDragInt3)
	F("DragInt4",ImDragInt4)
	F("DragFloatRange2",ImDragIntRange2)
	F("SliderFloat",ImSliderFloat)
	F("SliderFloat2",ImSliderFloat2)
	F("SliderFloat3",ImSliderFloat3)
	F("SliderFloat4",ImSliderFloat4)
	F("SliderAngle",ImSliderAngle)
	F("SliderInt",ImSliderInt)
	F("SliderInt2",ImSliderInt2)
	F("SliderInt3",ImSliderInt3)
	F("SliderInt4",ImSliderInt4)
	F("InputText",ImInputText)
	F("InputTextMultiline",ImInputTextMultiline)
	F("InputFloat",ImInputFloat)
	F("InputFloat2",ImInputFloat2)
	F("InputFloat3",ImInputFloat3)
	F("InputFloat4",ImInputFloat4)
	F("InputInt",ImInputInt)
	F("InputInt2",ImInputInt2)
	F("InputInt3",ImInputInt3)
	F("InputInt4",ImInputInt4)
	F("ColorEdit3",ImColorEdit3)
	F("ColorEdit4",ImColorEdit4)
	F("ColorPicker3",ImColorPicker3)
	F("ColorPicker4",ImColorPicker4)
	F("ColorButton",ImColorButton)
	F("SetColorEditOptions",SetColorEditOptions)
	F("TreeNode",ImTreeNode)
	F("TreeNodeEx",ImTreeNodeEx)
	F("TreePush",ImTreePush)
	F("TreePop",TreePop)
	F("TreeAdvanceToLabelPos",TreeAdvanceToLabelPos)
	F("GetTreeNodeToLabelSpacing",GetTreeNodeToLabelSpacing)
	F("SetNextTreeNodeOpen",ImSetNextTreeNodeOpen)
	F("CollapsingHeader",ImCollapsingHeader)
	F("ListBox",ImListBox)
	F("Selectable",ImSelectable)
	F("ListBox",ImListBox)
	F("ListBoxHeader",ImListBoxHeader)
	F("ListBoxFooter",ListBoxFooter)
	F("BeginMainMenuBar",BeginMainMenuBar)
	F("EndMainMenuBar",EndMainMenuBar)
	F("EndMenuBar",EndMenuBar)
	F("BeginMenu",ImBeginMenu)
	F("EndMenu",EndMenu)
	F("MenuItem",ImMenuItem)
	F("MenuItem2",ImMenuItem2)
	F("BeginTooltip",BeginTooltip)
	F("EndTooltip",EndTooltip)
	F("SetTooltip",ImSetTooltip)
	F("OpenPopup",OpenPopup)
	F("BeginPopup",ImBeginPopup)
	F("BeginPopupContextItem",ImBeginPopupContextItem)
	F("BeginPopupContextWindow",ImBeginPopupContextWindow)
	F("BeginPopupContextVoid",ImBeginPopupContextVoid)
	F("BeginPopupModal",ImBeginPopupModal)
	F("EndPopup",EndPopup)
	F("OpenPopupOnItemClick",ImOpenPopupOnItemClick)
	F("IsPopupOpen",IsPopupOpen)
	F("CloseCurrentPopup",CloseCurrentPopup)
	F("Columns",ImColumns)
	F("NextColumn",NextColumn)
	F("GetColumnIndex",GetColumnIndex)
	F("GetColumnWidth",ImGetColumnWidth)
	F("SetColumnWidth",SetColumnWidth)
	F("GetColumnOffset",ImGetColumnOffset)
	F("SetColumnOffset",SetColumnOffset)
	F("GetColumnsCount",GetColumnsCount)
	F("PushClipRect",ImPushClipRect)
	F("PopClipRect",PopClipRect)
	F("SetItemDefaultFocus",SetItemDefaultFocus)
	F("SetKeyboardFocusHere",ImSetKeyboardFocusHere)
	F("IsItemHovered",ImIsItemHovered)
	F("IsItemActive",IsItemActive)
	F("IsItemFocused",IsItemFocused)
	F("IsItemClicked",ImIsItemClicked)
	F("IsItemVisible",IsItemVisible)
	F("IsItemEdited",IsItemEdited)
	F("IsItemDeactivated",IsItemDeactivated)
	F("IsItemDeactivatedAfterEdit",IsItemDeactivatedAfterEdit)
	F("IsAnyItemHovered",IsAnyItemHovered)
	F("IsAnyItemActive",IsAnyItemActive)
	F("IsAnyItemFocused",IsAnyItemFocused)
	F("GetItemRectMin",ImGetItemRectMin)
	F("GetItemRectMax",ImGetItemRectMax)
	F("GetItemRectSize",ImGetItemRectSize)
	F("SetItemAllowOverlap",SetItemAllowOverlap)
	F("IsRectVisible",ImIsRectVisible)
	F("IsRectVisible2",ImIsRectVisible2)
	F("GetTime",GetTime)
	F("GetFrameCount",GetFrameCount)
	F("GetStyleColorName",GetStyleColorName)
	F("CalcTextSize",ImCalcTextSize)
	F("CalcListClipping",ImCalcListClipping)
	F("BeginChildFrame",ImBeginChildFrame)
	F("EndChildFrame",EndChildFrame)
	F("GetClipboardText",GetClipboardText)
	F("SetClipboardText",SetClipboardText)
	F("LoadFont",ImLoadFont)
	P("Flags",&ImFlags)
	EN;
#define ENUM(name,...) "ImGui.Flags." name "={\n" __VA_ARGS__ "}\n"
	lua::dostring(
		ENUM("Window",
			 "None						= 0,"
			 "NoTitleBar				= 2 ^ 0,"
			 "NoResize					= 2 ^ 1,"
			 "NoMove					= 2 ^ 2,"
			 "NoScrollbar				= 2 ^ 3,"
			 "NoScrollWithMouse			= 2 ^ 4,"
			 "NoCollapse				= 2 ^ 5,"
			 "AlwaysAutoResize			= 2 ^ 6,"
			 "NoSavedSettings			= 2 ^ 8,"
			 "NoInputs					= 2 ^ 9,"
			 "MenuBar					= 2 ^ 10,"
			 "HorizontalScrollbar		= 2 ^ 11,"
			 "NoFocusOnAppearing		= 2 ^ 12,"
			 "NoBringToFrontOnFocus		= 2 ^ 13,"
			 "AlwaysVerticalScrollbar	= 2 ^ 14,"
			 "AlwaysHorizontalScrollbar	= 2 ^ 15,"
			 "AlwaysUseWindowPadding	= 2 ^ 16,"
			 "NoNavInputs				= 2 ^ 18,"
			 "NoNavFocus				= 2 ^ 19,"
			 "NoNav						= 786432,"
			 "NavFlattened				= 2 ^ 23,"
			)
		ENUM("InputText",
			 "None                = 0,"
			 "CharsDecimal        = 2 ^ 0,"
			 "CharsHexadecimal    = 2 ^ 1,"
			 "CharsUppercase      = 2 ^ 2,"
			 "CharsNoBlank        = 2 ^ 3,"
			 "AutoSelectAll       = 2 ^ 4,"
			 "EnterReturnsTrue    = 2 ^ 5,"
			 "CallbackCompletion  = 2 ^ 6,"
			 "CallbackHistory     = 2 ^ 7,"
			 "CallbackAlways      = 2 ^ 8,"
			 "CallbackCharFilter  = 2 ^ 9,"
			 "AllowTabInput       = 2 ^ 10,"
			 "CtrlEnterForNewLine = 2 ^ 11,"
			 "NoHorizontalScroll  = 2 ^ 12,"
			 "AlwaysInsertMode    = 2 ^ 13,"
			 "ReadOnly            = 2 ^ 14,"
			 "Password            = 2 ^ 15,"
			 "NoUndoRedo          = 2 ^ 16,"
			 "CharsScientific     = 2 ^ 17,"
			 "CallbackResize      = 2 ^ 18,"
			 "Multiline           = 2 ^ 20"
			)
		ENUM("TreeNode",
			 "None                 = 0,"
			 "Selected             = 2 ^ 0,"
			 "Framed               = 2 ^ 1,"
			 "AllowItemOverlap     = 2 ^ 2,"
			 "NoTreePushOnOpen     = 2 ^ 3,"
			 "NoAutoOpenOnLog      = 2 ^ 4,"
			 "DefaultOpen          = 2 ^ 5,"
			 "OpenOnDoubleClick    = 2 ^ 6,"
			 "OpenOnArrow          = 2 ^ 7,"
			 "Leaf                 = 2 ^ 8,"
			 "Bullet               = 2 ^ 9,"
			 "FramePadding         = 2 ^ 10,"
			 "NavLeftJumpsBackHere = 2 ^ 13,"
			 "CollapsingHeader     = 26"
			)
		ENUM("Selectable",
			 "None				= 0,"
			 "DontClosePopups	= 2 ^ 0,"
			 "SpanAllColumns	= 2 ^ 1,"
			 "AllowDoubleClick	= 2 ^ 2,"
			 "Disabled			= 2 ^ 3"
			)
		ENUM("Combo",
			 "None				= 0,"
			 "PopupAlignLeft	= 2 ^ 0,"
			 "HeightSmall		= 2 ^ 1,"
			 "HeightRegular		= 2 ^ 2,"
			 "HeightLarge		= 2 ^ 3,"
			 "HeightLargest		= 2 ^ 4,"
			 "NoArrowButton		= 2 ^ 5,"
			 "NoPreview			= 2 ^ 6,"
			 "HeightMask_		= 30"
			)
		ENUM("Focused",
			 "None				= 0,"
			 "ChildWindows		= 2 ^ 0,"
			 "RootWindow		= 2 ^ 1,"
			 "AnyWindow			= 2 ^ 2,"
			 "RootAndChildWindows = 6"
			)
		ENUM("Hovered",
			 "None							= 0,"
			 "ChildWindow					= 2 ^ 0,"
			 "RootWindow						= 2 ^ 1,"
			 "AnyWindow						= 2 ^ 2,"
			 "AllowWhenBlockedByPopup		= 2 ^ 3,"
			 "AllowWhenBlockedByActiveItem	= 2 ^ 5,"
			 "AllowWhenOverlapped			= 2 ^ 6,"
			 "AllowWhenDisabled				= 2 ^ 7,"
			 "RectOnly						= 104,"
			 "RootAndChildWindows			= 3"
			)
		ENUM("Directory",
			 "None	= -1,"
			 "Left	= 0,"
			 "Right	= 1,"
			 "Up		= 2,"
			 "Down	= 3"
			)
		ENUM("Color",
			 "Text					= 0,"
			 "TextDisabled			= 1,"
			 "WindowBg				= 2,"
			 "ChildBg				= 3,"
			 "PopupBg				= 4,"
			 "Border					= 5,"
			 "BorderShadow			= 6,"
			 "FrameBg				= 7,"
			 "FrameBgHovered			= 8,"
			 "FrameBgActive			= 9,"
			 "TitleBg				= 10,"
			 "TitleBgActive			= 11,"
			 "TitleBgCollapsed		= 12,"
			 "MenuBarBg				= 13,"
			 "ScrollbarBg			= 14,"
			 "ScrollbarGrab			= 15,"
			 "ScrollbarGrabHovered	= 16,"
			 "ScrollbarGrabActive	= 17,"
			 "CheckMark				= 18,"
			 "SliderGrab				= 19,"
			 "SliderGrabActive		= 20,"
			 "Button					= 21,"
			 "ButtonHovered			= 22,"
			 "ButtonActive			= 23,"
			 "Header					= 24,"
			 "HeaderHovered			= 25,"
			 "HeaderActive			= 26,"
			 "Separator				= 27,"
			 "SeparatorHovered		= 28,"
			 "SeparatorActive		= 29,"
			 "ResizeGrip				= 30,"
			 "ResizeGripHovered		= 31,"
			 "ResizeGripActive		= 32,"
			 "PlotLines				= 33,"
			 "PlotLinesHovered		= 34,"
			 "PlotHistogram			= 35,"
			 "PlotHistogramHovered	= 36,"
			 "TextSelectedBg			= 37,"
			 "DragDropTarget			= 38,"
			 "NavHighlight			= 39,"
			 "NavWindowingHighlight	= 40,"
			 "NavWindowingDimBg		= 41,"
			 "ModalWindowDimBg		= 42"
			)
		ENUM("Style",
			 "Alpha				= 0,"
			 "WindowPadding		= 1,"
			 "WindowRounding		= 2,"
			 "WindowBorderSize	= 3,"
			 "WindowMinSize		= 4,"
			 "WindowTitleAlign	= 5,"
			 "ChildRounding		= 6,"
			 "ChildBorderSize	= 7,"
			 "PopupRounding		= 8,"
			 "PopupBorderSize	= 9,"
			 "FramePadding		= 10,"
			 "FrameRounding		= 11,"
			 "FrameBorderSize	= 12,"
			 "ItemSpacing		= 13,"
			 "ItemInnerSpacing	= 14,"
			 "IndentSpacing		= 15,"
			 "ScrollbarSize		= 16,"
			 "ScrollbarRounding	= 17,"
			 "GrabMinSize		= 18,"
			 "GrabRounding		= 19,"
			 "ButtonTextAlign	= 20"
			)
		ENUM("Cond",
			 "Always			= 2 ^ 0,"
			 "Once			= 2 ^ 1,"
			 "FirstUseEver	= 2 ^ 2,"
			 "Appearing		= 2 ^ 3"
			)
	);
}
