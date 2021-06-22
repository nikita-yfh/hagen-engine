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

static inline void trim(std::string &s) {
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
void Interface::load_imgui_font(string name,float size) {
	ImFontConfig font_config;
	font_config.OversampleH = 1;
	font_config.OversampleV = 1;
	font_config.PixelSnapH = 1;

	static const ImWchar ranges[] = {
		0x0020, 0x00FF,
		0x0400, 0x044F,
		0,
	};
	GetIO().Fonts->AddFontFromFileTTF((prefix+"fonts/"+name).c_str(), size, &font_config, ranges);
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
	ImGui::BeginChild(text::getc("settings/title"), ImVec2(0, -ImGui::GetFrameHeightWithSpacing()),true);
	Text("%s",text::getc("settings/graphics"));
	int size_input[2]= {set.SW,set.SH};
	if(InputInt2(text::getc("settings/window_size"),size_input))
		restart=true;
	set.SW=size_input[0];
	set.SH=size_input[1];
	if(Checkbox(text::getc("settings/fullscreen"),&set.fullscreen))
		restart=true;
	Text("%s",text::getc("settings/sound"));
	SliderInt(text::getc("settings/sound_volume"),&set.sound_volume,0,100,"%d%%");
	SliderInt(text::getc("settings/music_volume"),&set.music_volume,0,100,"%d%%");
	Text("%s",text::getc("settings/game"));
	if (ImGui::BeginCombo(text::getc("settings/language"),set.language.c_str())) {
		for (int q = 0; q<languages.size(); q++) {
			const bool is_selected = (set.language==languages[q]);
			if (ImGui::Selectable(languages[q].c_str(),is_selected))
				set.language=languages[q];
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	EndChild();
	if(Button(text::getc("common/ok"))) {
		if(restart)
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
	if(restart)
		quit();
	else if(settings.language==set.language)
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
	restart=false;
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
	struct {
		string text;
		void(*func)();
	} buttons[]= {
		{
			text::get("main_menu/new_game"),[]() {
				load_level(interface.levelchooser.get_first_level(),true);
				interface.mainmenu.hide();
				interface.levelchooser.hide();
				interface.pause.hide();
			}
		},
		{
			text::get("main_menu/choose_level"),[]() {
				interface.levelchooser.show();
			}
		},
		{
			text::get("main_menu/load_game"),[]() {
				interface.saver.show(0);
			}
		},
		{
			text::get("main_menu/settings"),[]() {
				interface.settingmanager.show();
			}
		},
		{
			text::get("main_menu/exit_game"),[]() {
				quit();
			}
		}
	};
	uint16_t text_h=FC_GetLineHeight(font);
	uint16_t pos=SH-borders.bottom-text_h*sizeof(buttons)/sizeof(*buttons);

	float image_scale=image_h/title->h;

	float image_w=image_scale*title->w;
	GPU_BlitScale(title,0,ren,borders.left+image_w/2,borders.top+image_h/2,image_scale,image_scale);
	for(auto button : buttons) {
		GPU_Rect rect=GPU_MakeRect(borders.left,pos,FC_GetWidth(font,button.text.c_str()),text_h);
		if(in_rect(mouse.x,mouse.y,rect)) {
			FC_DrawColor(font,ren,borders.left,pos,active.color(),button.text.c_str());
			if(mouse.state==1)
				button.func();
		} else {
			FC_DrawColor(font,ren,borders.left,pos,inactive.color(),button.text.c_str());
		}
		pos+=text_h;
	}
}
void MainMenu::load_config() {
	XMLNode node=open_xml((prefix+"config/main_menu.xml").c_str(),"main_menu");
	{
		XMLNode text=node.getChildNode("text");
		load_font(font,text,"inactive",SH);
		load_value(text,"active",active);
		load_value(text,"inactive",inactive);
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
static LuaRef *last_value;
static LuaRef *ImFlags;
static bool ImBegin(const char* name, bool o = 0, ImGuiWindowFlags flags = 0) {
	bool ret=Begin(name,&o,flags);
	(*last_value)=o;
	return ret;
}
static inline LuaRef ImGetLast() {
	return *last_value;
}
static inline bool ImBeginChild(const char* str_id, float sx=0, float sy=0, bool border = false, ImGuiWindowFlags flags = 0) {
	return BeginChild(str_id,ImVec2(sx,sy),border,flags);
}
static inline float ImGetWindowPosX() {
	return GetWindowPos().x;
}
static inline float ImGetWindowPosY() {
	return GetWindowPos().y;
}
static inline float ImGetContentRegionMaxX() {
	return GetContentRegionMax().x;
}
static inline float ImGetContentRegionMaxY() {
	return GetContentRegionMax().y;
}
static inline float ImGetContentRegionAvailX() {
	return GetContentRegionAvail().x;
}
static inline float ImGetContentRegionAvailY() {
	return GetContentRegionAvail().y;
}
static inline float ImGetWindowContentRegionMinX() {
	return GetWindowContentRegionMin().x;
}
static inline float ImGetWindowContentRegionMinY() {
	return GetWindowContentRegionMin().y;
}
static inline float ImGetWindowContentRegionMaxX() {
	return GetWindowContentRegionMax().x;
}
static inline float ImGetWindowContentRegionMaxY() {
	return GetWindowContentRegionMax().y;
}
static inline void ImSetNextWindowPos(float x,float y,ImGuiCond cond = 0,float px=0,float py=0) {
	SetNextWindowPos(ImVec2(x,y),cond,ImVec2(px,py));
}
static inline void ImSetNextWindowSize(float x,float y,ImGuiCond cond = 0) {
	SetNextWindowSize(ImVec2(x,y),cond);
}
static inline void ImSetNextWindowSizeConstraints(float minx,float miny,float maxx,float maxy) {
	SetNextWindowSizeConstraints(ImVec2(minx,miny),ImVec2(maxx,maxy));
}
static inline void ImSetNextWindowContentSize(float x,float y) {
	SetNextWindowContentSize(ImVec2(x,y));
}
static inline void ImSetWindowPos(float x,float y,ImGuiCond cond = 0) {
	SetWindowPos(ImVec2(x,y),cond);
}
static inline void ImSetWindowSize(float x,float y,ImGuiCond cond = 0) {
	SetWindowSize(ImVec2(x,y),cond);
}
static inline void ImSetWindowCollapsed(bool collapsed, ImGuiCond cond = 0) {
	SetWindowCollapsed(collapsed,cond);
}
static inline void ImSetWindowFocus() {
	SetWindowFocus();
}
static inline void ImSetWindowPosName(const char* name, float x, float y, ImGuiCond cond = 0) {
	SetWindowPos(name,ImVec2(x,y),cond);
}
static inline void ImSetWindowSizeName(const char* name, float x, float y, ImGuiCond cond = 0) {
	SetWindowSize(name,ImVec2(x,y),cond);
}
static inline void ImSetWindowCollapsedName(const char* name, bool collapsed, ImGuiCond cond = 0) {
	SetWindowCollapsed(name,collapsed,cond);
}
static inline void ImSetWindowFocusName(const char* name) {
	SetWindowFocus(name);
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
static inline float ImGetCursorStartPosX() {
	return GetCursorStartPos().x;
}
static inline float ImGetCursorStartPosY() {
	return GetCursorStartPos().y;
}
static inline float ImGetCursorScreenPosX() {
	return GetCursorScreenPos().x;
}
static inline float ImGetCursorScreenPosY() {
	return GetCursorScreenPos().y;
}
static inline void ImSetCursorScreenPos(float x,float y) {
	SetCursorScreenPos(ImVec2(x,y));
}
static inline void ImSetCursorScreenPosX(float x) {
	SetCursorScreenPos(ImVec2(x,ImGetCursorScreenPosY()));
}
static inline void ImSetCursorScreenPosY(float y) {
	SetCursorScreenPos(ImVec2(ImGetCursorScreenPosX(),y));
}
static inline void ImText(const char *text){
	Text("%s",text);
}
static inline void ImTextColored(Color c,const char *text){
	ImVec4 v(c.r,c.g,c.b,c.a);
	TextColored(v,"%s",text);
}
static inline void ImTextDisabled(const char *text){
	TextDisabled("%s",text);
}
static inline void ImTextWrapped(const char *text){
	TextWrapped("%s",text);
}
static inline void ImLabelText(const char *label,const char *text){
	LabelText(label,"%s",text);
}
static inline void ImBulletText(const char *text){
	BulletText("%s",text);
}
static inline bool ImButton(const char *label, float x,float y){
	return Button(label,ImVec2(x,y));
}
static inline bool ImInvisibleButton(const char *label, float x,float y){
	return InvisibleButton(label,ImVec2(x,y));
}
static bool ImImageButton(string texture,float x,float y){
	GPU_Image *tex=load_texture(texture);
	return ImageButton((void*)GPU_GetTextureHandle(tex),ImVec2(x,y));
}
static void ImImage(string texture,float x,float y){
	GPU_Image *tex=load_texture(texture);
	Image((void*)GPU_GetTextureHandle(tex),ImVec2(x,y));
}
static bool ImCheckbox(const char *label,bool v){
	bool ret=Checkbox(label,&v);
	(*last_value)=v;
	return ret;
}
static inline void ImProgressBar(float fraction,float x,float y,const char *overlay){
	ProgressBar(fraction,ImVec2(x,y),overlay);
}
static inline bool ImRadioButton(const char* label, bool active){
	return RadioButton(label,active);
}
static bool ImCombo(const char *label, int item, vector<const char*>items,int height=-1){
	const char **i=new const char*[items.size()];
	bool ret=Combo(label,&item,i,items.size(),height);
	(*last_value)=item;
	delete[]i;
	return ret;
}
static bool ImDragFloat(const char* label, float v, float v_speed, float v_min, float v_max, const char* format){
	bool ret=DragFloat(label,&v,v_speed,v_min,v_max,format);
	(*last_value)=v;
	return ret;
}
static bool ImDragFloatN(const char* label,vector<float>vi,float v_speed, float v_min, float v_max, const char* format){
	assert(vi.size()!=0);
	float v[4];
	for(int q=0;q<vi.size();q++)
		v[q]=vi[q];
	bool ret=DragScalarN(label, ImGuiDataType_Float, v, vi.size(), v_speed, &v_min, &v_max, format);
	for(int q=0;q<vi.size();q++)
		vi[q]=v[q];
	(*last_value)=vi;
	return ret;
}
static bool ImDragColor(const char* label,Color *color,float v_speed){
	assert(color!=0);
	int v[4]={color->r,color->g,color->b,color->a};
	return DragInt4(label,v,v_speed,0.0f,255.0f);
}
static bool ImSliderFloat(const char* label, float v, float v_min, float v_max, const char* format){
	bool ret=SliderFloat(label,&v,v_min,v_max,format);
	(*last_value)=v;
	return ret;
}
static bool ImSliderFloatN(const char* label,vector<float>vi, float v_min, float v_max, const char* format){
	assert(vi.size()!=0);
	float v[4];
	for(int q=0;q<vi.size();q++)
		v[q]=vi[q];
	bool ret=SliderScalarN(label, ImGuiDataType_Float, v, vi.size(), &v_min, &v_max, format);
	for(int q=0;q<vi.size();q++)
		vi[q]=v[q];
	(*last_value)=vi;
	return ret;
}
static bool ImSliderColor(const char* label,Color *color){
	assert(color!=0);
	int v[4]={color->r,color->g,color->b,color->a};
	return SliderInt4(label,v,0.0f,255.0f);
}
void bind_imgui() {
	ImFlags=new LuaRef(newTable(lua::L));
	last_value=new LuaRef(lua::L);
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
	F("GetContentRegionMaxX",ImGetContentRegionMaxX)
	F("GetContentRegionMaxY",ImGetContentRegionMaxY)
	F("GetContentRegionAvailX",ImGetContentRegionAvailX)
	F("GetContentRegionAvailY",ImGetContentRegionAvailY)
	F("GetWindowContentRegionMinX",ImGetWindowContentRegionMinX)
	F("GetWindowContentRegionMinY",ImGetWindowContentRegionMinY)
	F("GetWindowContentRegionMaxX",ImGetWindowContentRegionMaxX)
	F("GetWindowContentRegionMaxY",ImGetWindowContentRegionMaxY)
	F("GetWindowContentRegionWidth",GetWindowContentRegionWidth)
	F("SetNextWindowPos",ImSetNextWindowPos)
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
	F("GetCursorPosX",GetCursorPosX)
	F("GetCursorPosY",GetCursorPosY)
	F("SetCursorPosX",SetCursorPosX)
	F("SetCursorPosY",SetCursorPosY)
	F("SetCursorPos",SetCursorPos)
	F("GetCursorStartPosX",ImGetCursorStartPosX)
	F("GetCursorStartPosY",ImGetCursorStartPosY)
	F("GetCursorScreenPosX",ImGetCursorScreenPosX)
	F("GetCursorScreenPosY",ImGetCursorScreenPosY)
	F("SetCursorScreenPosX",ImGetCursorScreenPosX)
	F("SetCursorScreenPosY",ImGetCursorScreenPosY)
	F("SetCursorScreenPos",ImSetCursorScreenPos)
	F("AlignTextToFramePadding",AlignTextToFramePadding)
	F("GetTextLineHeight",GetTextLineHeight)
	F("GetTextLineHeightWithSpacing",GetTextLineHeightWithSpacing)
	F("GetFrameHeight",GetFrameHeight)
	F("GetFrameHeightWithSpacing",GetFrameHeightWithSpacing)
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
	F("ProgressBar",ImProgressBar)
	F("Bullet",ImGui::Bullet)
	F("BeginCombo",BeginCombo)
	F("EndCombo",EndCombo)
	F("Combo",ImCombo)
	F("DragValue",ImDragFloat)
	F("DragValueN",ImDragFloatN)
	F("DragColor",ImDragColor)
	F("SliderValue",ImSliderFloat)
	F("SliderValueN",ImSliderFloatN)
	F("SliderColor",ImSliderColor)
	P("last",ImGetLast)
	V("Flags",&ImFlags)
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
		ENUM("DragDrop",
			"None							= 0,"
			"SourceNoPreviewTooltip			= 2 ^ 0,"
			"SourceNoDisableHover			= 2 ^ 1,"
			"SourceNoHoldToOpenOthers		= 2 ^ 2,"
			"SourceAllowNullID				= 2 ^ 3,"
			"SourceExtern					= 2 ^ 4,"
			"SourceAutoExpirePayload		= 2 ^ 5,"
			"AcceptBeforeDelivery			= 2 ^ 10,"
			"AcceptNoDrawDefaultRect		= 2 ^ 11,"
			"AcceptNoPreviewTooltip			= 2 ^ 12,"
			"AcceptPeekOnly					= 3072"
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
