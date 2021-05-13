#pragma once
#include <string>
#include <map>
using namespace std;
extern string language;
string get_text(string id);
void preload_locale();
void clear_locale();
