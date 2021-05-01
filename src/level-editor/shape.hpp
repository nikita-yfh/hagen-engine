#pragma once
#include "object.hpp"
#include "xmlParser.h"
std::string find_shape_1(float x,float y);
std::string find_shape_2(float x,float y);
struct Physic : Object {
	float density=1000.0f;
	float restitution=0.0f;
	float friction=0.3f;
	uint8_t layer=2;
	bool ex=1;
	std::string texture="";
	unsigned short category=0;
	unsigned short mask=0xFFFF;
	static GtkWidget	*p1, *p2, *p3, *t1, *t2, *t3, *t4, *combo, *entry,
			  *text, *expand, *set_but, *c_category;
	static GtkObject *a1, *a2, *a3;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (Physic *p);
	static void update1();
	void vupdate();
	void save(XMLNode &parent,float xp,float yp);
};
struct BiSymmetrical : Physic {
	BiSymmetrical() {}
	BiSymmetrical(float,float,float);
	bool drag(float xp,float yp,int dr);
	bool create(float xp,float yp,int dr);
	float x;
	float y;
	float r;
	std::vector<float*> get_xpoints();
	std::vector<float*> get_ypoints();
	static GtkWidget *px, *py, *pr, *tx, *ty, *tr;
	static GtkObject *ax, *ay, *ar;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (BiSymmetrical *p);
	static void update1();
	void vupdate();
};
struct Square : BiSymmetrical {
	Square() {}
	Square(float,float,float);
	void draw(cairo_t *cr);
	std::string name();
};
struct Circle : BiSymmetrical {
	Circle() {}
	Circle(float,float,float);
	void draw(cairo_t *cr);
	std::string name();
};
struct BiPoints : Physic {
	BiPoints() {}
	BiPoints(float,float,float,float);
	float x1;
	float y1;
	float x2;
	float y2;
	bool drag(float xp,float yp,int dr);
	bool create(float xp,float yp,int dr);
	std::vector<float*> get_xpoints();
	std::vector<float*> get_ypoints();
	static GtkWidget *px1, *px2, *py1, *py2, *tx1, *tx2, *ty1, *ty2;
	static GtkObject *ax1, *ax2, *ay1, *ay2;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (BiPoints *p);
	static void update1();
	void vupdate();
};
struct Rect : BiPoints {
	Rect() {}
	Rect(float,float,float,float);
	void draw(cairo_t *cr);
	std::string name();
};
struct Polygon : Physic {
	Polygon();
	Polygon(std::vector<float>,std::vector<float>);
	static GtkWidget *px, *py, *tx, *ty;
	static GtkObject *ax, *ay;
	void draw(cairo_t *cr);
	bool drag(float xp,float yp,int dr);
	bool create(float xp,float yp,int dr);
	void push(float x,float y);
	std::vector<float>x;
	std::vector<float>y;
	std::vector<float*> get_xpoints();
	std::vector<float*> get_ypoints();
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (Polygon *p);
	static void update1();
	void vupdate();
	std::string name();
	int size();
};
struct Cover : Physic {
	Cover();
	Cover(std::vector<float>,std::vector<float>);
	static GtkWidget *px, *py, *tx, *ty;
	static GtkObject *ax, *ay;
	void draw(cairo_t *cr);
	bool drag(float xp,float yp,int dr);
	bool create(float xp,float yp,int dr);
	void push(float x,float y);
	std::vector<float>x;
	std::vector<float>y;
	std::vector<float*> get_xpoints();
	std::vector<float*> get_ypoints();
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (Cover *p);
	static void update1();
	void vupdate();
	std::string name();
	int size();
};
struct Line : BiPoints {
	Line() {}
	Line(float,float,float,float);
	void draw(cairo_t *cr);
	std::string name();
};
