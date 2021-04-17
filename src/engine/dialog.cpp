#include "dialog.hpp"
namespace dialog {
#ifdef WIN32
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

#else
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

#endif
}
