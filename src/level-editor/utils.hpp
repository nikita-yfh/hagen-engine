#pragma once
#include <memory>
#include <string>
#include <stdexcept>
#include <gtk/gtk.h>
#include <cairo.h>
#include <algorithm>
#include <ctype.h>
#include <cmath>
#include "b2_math.h"
#include <sstream>
float drawx(float a);
float drawy(float a);
float dragx(float a);
float dragy(float a);
float to_grid(float a);
float to_angle_grid(float a);
bool touch(b2Vec2 v1,b2Vec2 v2);
void set_shape_color(cairo_t *cr, uint8_t layer);
float get_grid();
void ins_text(GtkWidget *table,GtkWidget *child, int pos);
void ins_widget(GtkWidget *table,GtkWidget *child, int pos);
void ins_widget2(GtkWidget *table,GtkWidget *child, int pos);
std::string lower(std::string);
void set_status(int context,const char* text);
void remove_status(int context);
float vec_angle1(b2Vec2 v);
float vec_angle2(b2Vec2 v);
template<typename ... Args>
std::string ssprintf(const std::string& format, Args ... args) {
	int size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	std::unique_ptr<char[]> buf(new char[ size ]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1);
}
std::string get_free_name(std::string name);
template<typename T>
std::string to_str(T val) {
	std::stringstream str;
	str<<val;
	return str.str();
}
b2Vec2 bis(b2Vec2 v1,b2Vec2 v2,float length);
b2Vec2 rotate(b2Vec2 o,b2Vec2 a,float angle);
