#pragma once
#include "object.hpp"
#include <string>
#include <vector>
#include <cmath>
#include "xmlParser.h"
struct Joint : Object {
	Joint() {}
	std::string id1;
	std::string id2;
	bool collide=0;
	static GtkWidget *p1, *p2, *t1, *t2, *c_collide;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (Joint *p);
	static void update1();
	void vupdate();
};
struct PointJoint : Joint {
	PointJoint() {}
	float x;
	float y;
	bool create(float xp,float yp,int dr);
	std::vector<float*> get_xpoints();
	std::vector<float*> get_ypoints();
	static GtkWidget *px, *py, *tx, *ty;
	static GtkObject *ax, *ay;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (PointJoint *p);
	static void update1();
	void vupdate();
	void draw_lines(cairo_t *cr);
};
struct GearJoint : Joint {
	GearJoint() {}
	float ratio=1.0f;
	bool drag(float xp,float yp,int dr);
	bool create(float xp,float yp,int dr);
	void draw(cairo_t *cr);
	static GtkWidget *text,*rat, *info;
	static GtkObject *ar;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (GearJoint *p);
	static void update1();
	void vupdate();
	std::string name();
};
struct WeldJoint : PointJoint {
	WeldJoint() {}
	float stiffness=1000;
	float damping=0.5;
	void draw(cairo_t *cr);
	bool drag(float xp,float yp,int dr);
	static GtkWidget *ts, *td, *ps, *pd;
	static GtkObject *as, *ad;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (WeldJoint *p);
	static void update1();
	void vupdate();
	std::string name();
};
struct RevoluteJoint : PointJoint {
	RevoluteJoint() {}
	bool limit=0;
	float lower=-M_PI/4;
	float upper=M_PI/4;
	bool motor=0;
	float speed=10;
	float torque=1000;
	void draw(cairo_t *cr);
	bool drag(float xp,float yp,int dr);
	static GtkWidget *sl, *pl, *tl, *pu, *tu, *sm, *ts, *ps, *tt, *pt;
	static GtkObject *al, *au, *as, *at;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (RevoluteJoint *p);
	static void update1();
	void vupdate();
	std::string name();
};
struct DistanceJoint : Joint {
	DistanceJoint() {}
	float x1,y1,x2,y2;
	float min=0;
	float max=0;
	float stiffness=1000;
	float damping=0.5;
	void draw(cairo_t *cr);
	bool drag(float xp,float yp,int dr);
	bool create(float xp,float yp,int dr);
	static GtkWidget *px1, *px2, *py1, *py2, *tx1, *tx2, *ty1, *ty2, *pmin, *pmax, *ps, *pd, *tmin, *tmax, *ts, *td;
	static GtkObject *ax1, *ax2, *ay1, *ay2, *amin, *amax, *as, *ad;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (DistanceJoint *p);
	static void update1();
	void vupdate();
	std::string name();
	void draw_lines(cairo_t *cr);
};
struct PulleyJoint : Joint {
	PulleyJoint() {}
	float	x1=-1000,y1=-1000,x2=-1000,y2=-1000,x3=-1000,y3=-1000,x4=-1000,y4=-1000;
	float ratio=1.0f;
	static GtkWidget	*px1, *px2, *py1, *py2, *tx1, *tx2, *ty1, *ty2,
						*px3, *px4, *py3, *py4, *tx3, *tx4, *ty3, *ty4,
						*pr, *tr, *info;
	static GtkObject *ax1, *ax2, *ax3, *ax4, *ay1, *ay2, *ay3, *ay4, *ar;
	void draw(cairo_t *cr);
	bool drag(float xp,float yp,int dr);
	bool create(float xp,float yp,int dr);
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (PulleyJoint *p);
	static void update1();
	void vupdate();
	std::string name();
	void draw_lines(cairo_t *cr);
};
struct PrismaticJoint : PointJoint {
	PrismaticJoint() {}
	bool limit=0;
	float lower=-3;
	float upper=3;
	bool motor=0;
	float speed=10;
	float force=1000;
	float angle=0;
	void draw(cairo_t *cr);
	bool drag(float xp,float yp,int dr);
	static GtkWidget *sl, *pl, *tl, *pu, *tu, *sm, *ts, *ps, *tt, *pt, *pa, *ta;
	static GtkObject *al, *au, *as, *at, *aa;
	static void init(GtkWidget*);
	static void show();
	static void hide();
	static void update (PrismaticJoint *p);
	static void update1();
	void vupdate();
	std::string name();
};
