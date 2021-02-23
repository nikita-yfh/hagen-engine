#pragma once
#include "object.hpp"
#include "shape.hpp"
#include <string>
struct Body : Object {
	Body() {};
	~Body();
	std::vector<Physic*>shapes;
	uint8_t type=0;
	bool fixed_rot=0;
	bool bullet=0;
	float gravity_scale=1.0f;
	bool drag(float xp,float yp,int dr);
	std::string name();
	static GtkWidget *combo, *px, *py, *ps, *tx, *ty, *ts, *cb, *cr;
	static GtkObject *ax, *ay, *as;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (Body *p);
	static void update1();
	void vupdate();
	void save(XMLNode&,bool);
	void load(XMLNode&,bool);
	float mean_x();
	float mean_y();
	void setpos(float x,float y);
	bool template_open(std::string);
	bool template_save(std::string);
	void resolve_names();
};
Body *get_selected_body();
Body *parent(std::string id);
void template_save();
