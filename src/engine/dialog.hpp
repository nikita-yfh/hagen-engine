#pragma once
#include <string>
using namespace std;
namespace dialog {
enum class Style {
	Info,
	Warning,
	Error,
	Question
};

enum class Buttons {
	OK,
	OKCancel,
	YesNo
};

enum class Selection {
	OK,
	Cancel,
	Yes,
	No,
	None
};

const Style DEFAULT_STYLE = Style::Info;
const Buttons DEFAULT_BUTTONS = Buttons::OK;

Selection show(string title, string message, Style style, Buttons buttons);

inline Selection show(string title, string message, Style style) {
	return show(title, message, style, DEFAULT_BUTTONS);
}

inline Selection show(string title, string message, Buttons buttons) {
	return show(title, message, DEFAULT_STYLE, buttons);
}

inline Selection show(string title,string message) {
	return show(title, message, DEFAULT_STYLE, DEFAULT_BUTTONS);
}
}

