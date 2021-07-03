#include "dialog.hpp"
#include "sdl.hpp"
namespace dialog {
#ifdef _WIN32
#include <windows.h>

namespace {

UINT getIcon(Style style) {
	switch (style) {
	case Style::Info:
		return MB_ICONINFORMATION;
	case Style::Warning:
		return MB_ICONWARNING;
	case Style::Error:
		return MB_ICONERROR;
	case Style::Question:
		return MB_ICONQUESTION;
	default:
		return MB_ICONINFORMATION;
	}
}

UINT getButtons(Buttons buttons) {
	switch (buttons) {
	case Buttons::OK:
		return MB_OK;
	case Buttons::OKCancel:
		return MB_OKCANCEL;
	case Buttons::YesNo:
		return MB_YESNO;
	default:
		return MB_OK;
	}
}

Selection getSelection(int response) {
	switch (response) {
	case IDOK:
		return Selection::OK;
	case IDCANCEL:
		return Selection::Cancel;
	case IDYES:
		return Selection::Yes;
	case IDNO:
		return Selection::No;
	default:
		return Selection::None;
	}
}

}

Selection show(string title, string message, Style style, Buttons buttons) {
	UINT flags = MB_TASKMODAL;

	flags |= getIcon(style);
	flags |= getButtons(buttons);

	return getSelection(MessageBox(NULL, message.c_str(), title.c_str(), flags));
}

#elif defined (__linux__) && !defined(ANDROID)
#include <gtk/gtk.h>
namespace {

GtkMessageType getMessageType(Style style) {
	switch (style) {
	case Style::Info:
		return GTK_MESSAGE_INFO;
	case Style::Warning:
		return GTK_MESSAGE_WARNING;
	case Style::Error:
		return GTK_MESSAGE_ERROR;
	case Style::Question:
		return GTK_MESSAGE_QUESTION;
	default:
		return GTK_MESSAGE_INFO;
	}
}

GtkButtonsType getButtonsType(Buttons buttons) {
	switch (buttons) {
	case Buttons::OK:
		return GTK_BUTTONS_OK;
	case Buttons::OKCancel:
		return GTK_BUTTONS_OK_CANCEL;
	case Buttons::YesNo:
		return GTK_BUTTONS_YES_NO;
	default:
		return GTK_BUTTONS_OK;
	}
}

Selection getSelection(gint response) {
	switch (response) {
	case GTK_RESPONSE_OK:
		return Selection::OK;
	case GTK_RESPONSE_CANCEL:
		return Selection::Cancel;
	case GTK_RESPONSE_YES:
		return Selection::Yes;
	case GTK_RESPONSE_NO:
		return Selection::No;
	default:
		return Selection::None;
	}
}

} // namespace

Selection show(string title, string message, Style style, Buttons buttons) {
	if (!gtk_init_check(0, NULL)) {
		return Selection::None;
	}

	GtkWidget *dialog = gtk_message_dialog_new(NULL,
						GTK_DIALOG_MODAL,
						getMessageType(style),
						getButtonsType(buttons),
						"%s",
						message.c_str());
	gtk_window_set_title(GTK_WINDOW(dialog), title.c_str());
	Selection selection = getSelection(gtk_dialog_run(GTK_DIALOG(dialog)));

	gtk_widget_destroy(GTK_WIDGET(dialog));
	while (g_main_context_iteration(NULL, false));

	return selection;
}
#else
namespace {

int getMessageType(Style style) {
	switch (style) {
	case Style::Info:
		return SDL_MESSAGEBOX_INFORMATION;
	case Style::Warning:
		return SDL_MESSAGEBOX_WARNING;
	case Style::Error:
		return SDL_MESSAGEBOX_ERROR;
	default:
		return SDL_MESSAGEBOX_INFORMATION;
	}
}

SDL_MessageBoxButtonData getButton(int flags,Selection sel,const char *text){
	SDL_MessageBoxButtonData d;
	d.buttonid=(int)sel;
	d.flags=flags;
	d.text=text;
	return d;
}

} // namespace

Selection show(string title, string message, Style style, Buttons buttons) {
	SDL_Window *w=SDL_GetWindowFromID(ren->context->windowID);
	SDL_MessageBoxData data;
	data.flags=getMessageType(style)|SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT;
	data.window=w;
	data.title=title.c_str();
	data.message=message.c_str();
	data.colorScheme=nullptr;
	switch(buttons){
	case Buttons::OKCancel:{
		data.buttons=new SDL_MessageBoxButtonData[2]{
			getButton(SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,Selection::OK,"OK"),
			getButton(SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,Selection::Cancel,"Cancel")};
		data.numbuttons=2;
		}break;
	case Buttons::YesNo:{
		data.buttons=new SDL_MessageBoxButtonData[2]{
			getButton(SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,Selection::Yes,"Yes"),
			getButton(SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,Selection::No,"No")};
		data.numbuttons=2;
		}break;
	default:{
		data.buttons=new SDL_MessageBoxButtonData[1]{
			getButton(SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,Selection::OK,"OK")};
		data.numbuttons=1;
		}break;
	}
	int button;
	if(SDL_ShowMessageBox(&data,&button))
		button=(int)Selection::OK;
	delete[]data.buttons;
	return (Selection)button;
}
#endif
}
