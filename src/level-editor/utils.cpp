#include "utils.hpp"
#include "editor.hpp"
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

bool touch(b2Vec2 p1,b2Vec2 p2) {
	return	p1.x*zoom-POINT_SIZE<p2.x*zoom &&
			p1.y*zoom-POINT_SIZE<p2.y*zoom &&
			p1.x*zoom+POINT_SIZE>p2.x*zoom &&
			p1.y*zoom+POINT_SIZE>p2.y*zoom;
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
float vec_angle1(b2Vec2 p) {
	if(p.x>=0)
		return M_PI+atan(p.y/p.x);
	else if(p.y>0)
		return 2*M_PI+atan(p.y/p.x);
	else
		return atan(p.y/p.x);
}
float vec_angle2(b2Vec2 p) {
	return fmod(M_PI+vec_angle1(p),M_PI*2);
}
b2Vec2 bis(b2Vec2 v1,b2Vec2 v2,float length) {
	v1.Normalize();
	if(v1.Length()==0){
		v2=v2.Skew();
		v2.Normalize();
		return length*v2;
	}
	v2.Normalize();
	if(v1==-v2)
		v1+=0.001*v1.Skew();
	b2Vec2 sum=v1+v2;
	sum.Normalize();
	if(v1.x*v2.y-v1.y*v2.x>0)
		sum=-sum;
	float angle=vec_angle1(v1)-vec_angle2(v2);
	sum*=abs(length/cos(angle/2));

	return sum;
}
