#pragma once
#include <string>
#include <map>
using namespace std;
extern string language;
string get_text(string id);
const char* get_ctext(string id);
void preload_locale();
void clear_locale();
