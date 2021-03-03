#include "utils.hpp"
#include "editor.hpp"
//#include "string.h"
#include "object.hpp"
#include <cmath>
using namespace std;
extern uint8_t key_state;
float drawx(float a) {
	return a*zoom+cx;
}
float drawy(float a) {
	return a*zoom+cy;
}
float dragx(float a) {
	return (a-cx)/zoom;
}
float dragy(float a) {
	return (a-cy)/zoom;
}
float get_grid() {
	if(key_state==0)		return grid;
	else if(key_state==1)	return grid/2;
	else return 0.001;
}
float get_angle_grid() {
	if(key_state==0)		return M_PI/12;
	else if(key_state==1)	return M_PI/24;
	else return M_PI/180;
}
float to_grid(float a) {
	float g=get_grid();
	return std::round(a/g)*g;
}
float to_angle_grid(float a) {
	float g=get_angle_grid();
	return std::round(a/g)*g;
}

bool touch(float x,float y,float xp,float yp) {
	return	x*zoom-POINT_SIZE<xp*zoom &&
			y*zoom-POINT_SIZE<yp*zoom &&
			x*zoom+POINT_SIZE>xp*zoom &&
			y*zoom+POINT_SIZE>yp*zoom;
}
void set_shape_color(cairo_t *cr, uint8_t type) {
	switch(type) {
	case 0:
		cairo_set_source_rgba(cr,STATIC_LAYER);
		break;
	case 1:
		cairo_set_source_rgba(cr,DYNAMIC_LAYER);
		break;
	case 2:
		cairo_set_source_rgba(cr,KINEMATIC_LAYER);
		break;
	}
}
void ins_text(GtkWidget *table,GtkWidget *child, int pos) {
	gtk_table_attach(GTK_TABLE(table),child,0,1,pos,pos+1,GTK_F,GTK_F,0,0);
}

void ins_widget(GtkWidget *table,GtkWidget *child, int pos) {
	gtk_table_attach(GTK_TABLE(table),child,1,2,pos,pos+1,GTK_EF,GTK_F,0,0);
}
void ins_widget2(GtkWidget *table,GtkWidget *child, int pos) {
	gtk_table_attach(GTK_TABLE(table),child,0,2,pos,pos+1,GTK_EF,GTK_F,0,0);
}
string lower(string source) {
	for ( size_t i = 0; i < source.length(); ++i ) {
		source[i] = tolower(source[i]);
	}
	return source;
}
void set_status(int context,const char*text) {
	gtk_statusbar_pop(GTK_STATUSBAR(status),context);
	gtk_statusbar_push(GTK_STATUSBAR(status),context,text);
}
void remove_status(int context) {
	gtk_statusbar_pop(GTK_STATUSBAR(status),context);
}
string get_free_name(string name) {
	vector<Object*> vec=get_all();
	for(int q=1;; q++) {
		bool ok=1;
		for(Object *obj : vec) {
			if(obj->id==ssprintf("_%s%d",name.c_str(),q)) {
				ok=0;
				break;
			}
		}
		if(ok)return ssprintf("_%s%d",name.c_str(),q);
	}
}
float to_fl(const char* str) {
	std::stringstream ss;
	ss<<str;
	float res;
	ss>>res;
	return res;
}
float vec_angle1(float x,float y) {
	if(x>=0)
		return M_PI+atan(y/x);
	else if(y>0)
		return 2*M_PI+atan(y/x);
	else
		return atan(y/x);
}
float vec_angle2(float x,float y) {
	return fmod(M_PI+vec_angle1(x,y),M_PI*2);
}
