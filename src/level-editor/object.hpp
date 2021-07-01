#pragma once
#include <vector>
#include <stdint.h>
#include <cairo.h>
#include <string>
#include <gtk/gtk.h>
#include "b2_math.h"
#include "main.hpp"

struct Object {
	Object() {}
	virtual void draw(cairo_t *cr);
	virtual bool drag(float xp,float yp,int dr);
	virtual bool create(float xp,float yp,int dr);
	virtual std::vector<b2Vec2*> get_points() ;
	virtual void vupdate();
	bool selected=0;
	std::string id;
	static GtkWidget *text, *entry;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (Object *p);
	static void update1();
	virtual std::string name() const;
	virtual bool del(float xp,float yp,int dr);
	virtual b2Vec2 mean() ;
};
std::vector <Object*> get_all();
void hide_all();
void create_all(GtkWidget*);
Object *get_selected_object();
bool delete_id(std::string id);
void draw_drag_rect(cairo_t *cr, b2Vec2 pos,bool tc);
void draw_drag_joint_rect(cairo_t *cr, b2Vec2 pos,bool tc);
Object *get_id(std::string);

extern int cur_table_string;

struct Point:Object {
	Point(b2Vec2,std::string);
	Point() {};
	void draw(cairo_t *cr);
	bool drag(float xp,float yp,int dr);
	bool create(float xp,float yp,int dr);
	b2Vec2 pos;
	std::vector<b2Vec2*> get_points() ;
	static GtkWidget *px, *py, *tx, *ty;
	static GtkObject *ax,*ay;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (Point *p);
	static void update1();
	void vupdate();
	std::string name() const;
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
	std::string name() const;
};
