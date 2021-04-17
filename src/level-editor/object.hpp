#pragma once
#include <vector>
#include <stdint.h>
#include <cairo.h>
#include <string>
#include <gtk/gtk.h>
#include "main.hpp"

struct Object {
	Object() {}
	virtual void draw(cairo_t *cr);
	virtual bool drag(float xp,float yp,int dr);
	virtual bool create(float xp,float yp,int dr);
	virtual std::vector<float*> get_xpoints();
	virtual std::vector<float*> get_ypoints();
	virtual void vupdate();
	bool selected=0;
	std::string id;
	static GtkWidget *text, *entry;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (Object *p);
	static void update1();
	virtual std::string name();
	virtual bool del(float xp,float yp,int dr);
	virtual float mean_x();
	virtual float mean_y();
};
std::vector <Object*> get_all();
void hide_all();
void create_all(GtkWidget*);
Object *get_selected_object();
bool delete_id(std::string id);
void draw_drag_rect(cairo_t *cr, float x,float y,bool tc);
void draw_drag_joint_rect(cairo_t *cr, float x,float y,bool tc);
Object *get_id(std::string);

extern int cur_table_string;

struct Point:Object {
	Point(float,float,std::string);
	Point() {};
	void draw(cairo_t *cr);
	bool drag(float xp,float yp,int dr);
	bool create(float xp,float yp,int dr);
	float x;
	float y;
	std::vector<float*> get_xpoints();
	std::vector<float*> get_ypoints();
	static GtkWidget *px, *py, *tx, *ty;
	static GtkObject *ax,*ay;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (Point *p);
	static void update1();
	void vupdate();
	std::string name();
};

struct Entity : Point {
	Entity() {};
	std::string type;
	bool player_on;
	static GtkWidget *entry,*text;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (Entity *p);
	static void update1();
	void vupdate();
	std::string name();
};
