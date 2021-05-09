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

static inline void trim(std::string &s) {
	while(s[0]==' ')
		s.erase(s.begin());
	while(s[s.size()-1]==' ')
		s.erase(s.end());
}

Pause::Pause(){
	get_text("pause/continue");
	get_text("pause/save_game");
	get_text("pause/load_game");
	get_text("pause/settings");
	get_text("pause/main_menu");
	get_text("pause/exit_game");
}

void Pause::Draw(){
	if(!shown)return;
	ImVec2 g=ImGui::GetItemRectSize();
	ImGuiIO &io=ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
										ImGuiCond_Always, ImVec2(0.5f,0.5f));
	if (!ImGui::Begin("", &shown,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
		ImGui::End();
		return;
	}
	static float w=0;
	ImVec2 align=ImVec2(w, 0);
	if(ImGui::Button(get_text("pause/continue").c_str(),align)){
		shown=0;
		interface.console.shown=0;
	}
	ImGui::Button(get_text("pause/save_game").c_str(),align);
	ImGui::Button(get_text("pause/load_game").c_str(),align);
	ImGui::Button(get_text("pause/settings").c_str(),align);
	ImGui::Button(get_text("pause/main_menu").c_str(),align);
	ImGui::Button(get_text("pause/exit_game").c_str(),align);
	w=ImGui::GetWindowContentRegionWidth();
	ImGui::End();
}

Console::Console() {
	ClearLog();
	memset(InputBuf, 0, sizeof(InputBuf));
	AddLog("LuaJIT 2.0.5 -- Copyright (C) 2005-2017 Mike Pall. http://luajit.org/");
	get_text("console/title");
	get_text("console/input");
	get_text("console/matches");
	get_text("console/clear");
}
Console::~Console() {
	ClearLog();
}
void  Console::ClearLog() {
	Items.clear();
}
void Console::AddLog(string str) {
	Items.push_back(str+"\n");
	ScrollToBottom=true;
}
void Console::Draw() {
	if(!shown)return;
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(get_text("console/title").c_str(), &shown)) {
		ImGui::End();
		return;
	}
	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow()) {
		if (ImGui::Selectable(get_text("console/clear").c_str()))
			ClearLog();
		ImGui::EndPopup();
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
	for (string item : Items) {
		if (!Filter.PassFilter(item.c_str()))
			continue;
		ImVec4 color;
		if (item.find("\"]:")!=item.npos)
			color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		else if (item.find("> ") == 0)
			color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		else
			color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::TextUnformatted(item.c_str());
		ImGui::PopStyleColor();
	}
	if(ImGui::GetScrollY() >= ImGui::GetScrollMaxY() && ScrollToBottom)
		ImGui::SetScrollHere(1.0f);

	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();
	bool reclaim_focus = false;
	ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
	if (ImGui::InputText(get_text("console/input").c_str(), InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &Console::TextEditCallbackStub, (void*)this)) {
		string s = InputBuf;
		trim(s);
		if (s[0]) ExecCommand(s);
		strcpy(InputBuf,"");
		reclaim_focus = true;
	}
	ImGui::SetItemDefaultFocus();
	if (reclaim_focus)
		ImGui::SetKeyboardFocusHere(-1);

	ImGui::End();
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
		const char* word_end = data->Buf + data->CursorPos;
		const char* word_start = word_end;
		while (word_start > data->Buf) {
			const char c = word_start[-1];
			if (c == ' ' || c == '\t' || c == ',' || c == ';')
				break;
			word_start--;
		}

		vector<string> candidates;

		if (candidates.size()==1) {
			data->DeleteChars(0, data->BufTextLen);
			data->InsertChars(0, candidates[0].c_str());
		} else if(candidates.size()) {
			int match_len = (int)(word_end - word_start);
			for (;;) {
				int c = 0;
				bool all_candidates_matches = true;
				for (int i = 0; i < candidates.size() && all_candidates_matches; i++)
					if (i == 0)
						c = toupper(candidates[i][match_len]);
					else if (c == 0 || c != toupper(candidates[i][match_len]))
						all_candidates_matches = false;
				if (!all_candidates_matches)
					break;
				match_len++;
			}
			AddLog(get_text("console/matches"));
			for (int i = 0; i < candidates.size(); i++)
				AddLog("- "+candidates[i]);
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

void Rect4::stabilize(float f) {
	if(top<1)top*=f;
	if(left<1)left*=f;
	if(right<1)right*=f;
	if(bottom<1)bottom*=f;
}
void Rect4::load(XMLNode l,float f) {
	top=stof(l.getAttribute("top"));
	left=stof(l.getAttribute("left"));
	right=stof(l.getAttribute("right"));
	bottom=stof(l.getAttribute("bottom"));
}
void Interface::update() {
	if(e.type==SDL_KEYDOWN) {
		if(e.key.keysym.sym==SDLK_BACKQUOTE)
			console.shown=!console.shown;
		else if(e.key.keysym.sym==SDLK_ESCAPE){
			if(console.shown)
				console.shown=0;
			else
				pause.shown=!pause.shown;
		}

	}
	game_interface.update();
	ImGui_ImplSDL2_ProcessEvent(&e);
}
void Interface::init_imgui() {
	SDL_GLContext& gl_context = ren->context->context;
	SDL_Window* window = SDL_GetWindowFromID(ren->context->windowID);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.IniFilename=nullptr;
	const char* glsl_version = "#version 120";
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui::StyleColorsClassic();
	load_imgui_font();
}
void Interface::load_imgui_font(){
	ImFontConfig font_config;
    font_config.OversampleH = 1;
    font_config.OversampleV = 1;
    font_config.PixelSnapH = 1;

    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF,
        0x0400, 0x044F,
        0,
    };
	ImGui::GetIO().Fonts->AddFontFromFileTTF((prefix+"fonts/interface.ttf").c_str(), 20.0f, &font_config, ranges);
}
void Interface::load_config() {
	game_interface.load_config();
}
void Interface::show() {
	game_interface.show();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(SDL_GetWindowFromID(ren->context->windowID));
	ImGui::NewFrame();
	console.Draw();
	pause.Draw();
	ImGui::Render();
	SDL_GL_MakeCurrent(SDL_GetWindowFromID(ren->context->windowID), ren->context->context);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game_interface::load_config() {
	XMLNode node=XMLNode::openFileHelper((prefix+"config/game_interface.xml").c_str(),"game_interface");
	{
		XMLNode text=node.getChildNode("text");
		load_font(font,text,"color",SH);
	}
	borders.load(node.getChildNode("border"),SH);
	borders.stabilize(SH);
}

void Game_interface::update() {}
void Game_interface::show() {
	short h=FC_GetLineHeight(font);
	{
		int health=0;
		if(get_entity("player"))
			health=get_entity("player")->health;
		FC_Draw(font,ren,borders.left,SH-borders.bottom-h,"%s %d %s",
				get_text("game_interface/health_prev").c_str(),
				health,
				get_text("game_interface/health").c_str()).w;
	}
	if(get_entity("player")) {
		auto draw_bullets=[=](string id,string str,uint8_t layer) {
			FC_DrawAlign(font,ren,SW-borders.left,SH-borders.top-layer*h,
						 FC_ALIGN_RIGHT,"%s %d/%d %s",get_text(str+"_prev").c_str(),
						 bullets[id].count,bullets[id].max,
						 get_text(str).c_str());
		};
		uint8_t layer=0;
		if(bullets[weapons[get_entity("player")->weapon].bullet2].max>0 &&
				weapons[get_entity("player")->weapon].bullet2 != weapons[get_entity("player")->weapon].bullet1)
			draw_bullets(weapons[get_entity("player")->weapon].bullet2,"game_interface/bullet2",++layer);
		if(bullets[weapons[get_entity("player")->weapon].bullet1].max>0)
			draw_bullets(weapons[get_entity("player")->weapon].bullet1,"game_interface/bullet1",++layer);
	}
}

void Interface::update_cursor() {
	set_cursor("aim.png");
}
Interface interface;
