#include "object.hpp"
#include "level.hpp"
#include "utils.hpp"
#include "events.hpp"
#include "editor.hpp"
#include "tree_view.hpp"
#include <cmath>
using namespace std;
int cur_table_string=0;
GtkObject	*Point::ax, *Point::ay,
			*Line::ax1, *Line::ax2, *Line::ay1, *Line::ay2,
			*Rect::ax,  *Rect::ay,  *Rect::aw,  *Rect::ah,
			*BiSymmetrical::ax, *BiSymmetrical::ay, *BiSymmetrical::ar,
			*Physic::a1, *Physic::a2, *Physic::a3, *Physic::a4, *Rotatable::aa,
			*Body::ax, *Body::ay, *Body::as, *Body::aad, *Body::ald,
			*Polygon::ax, *Polygon::ay,*Cover::ax, *Cover::ay, *Cover::ad,
			*PointJoint::ax, *PointJoint::ay,
			*WeldJoint::as, *WeldJoint::ad,
			*GearJoint::ar,
			*RevoluteJoint::al, *RevoluteJoint::au, *RevoluteJoint::as, *RevoluteJoint::at,
			*PrismaticJoint::al, *PrismaticJoint::au, *PrismaticJoint::as, *PrismaticJoint::at,
			*PrismaticJoint::aa,
			*DistanceJoint::ax1, *DistanceJoint::ax2, *DistanceJoint::ay1, *DistanceJoint::ay2,
			*DistanceJoint::amin, *DistanceJoint::amax, *DistanceJoint::as, *DistanceJoint::ad,
			*PulleyJoint::ax1, *PulleyJoint::ax2, *PulleyJoint::ay1, *PulleyJoint::ay2,
			*PulleyJoint::ax3, *PulleyJoint::ax4, *PulleyJoint::ay3, *PulleyJoint::ay4,
			*PulleyJoint::ar;

GtkWidget	*Line::px1, *Line::px2, *Line::py1, *Line::py2,
			*Line::tx1, *Line::tx2, *Line::ty1, *Line::ty2,
			*Rect::px, *Rect::py, *Rect::pw, *Rect::ph,
			*Rect::tx, *Rect::ty, *Rect::tw, *Rect::th,
			*Physic::combo, *Physic::text, *Physic::entry, *Physic::expand, *Physic::p4, *Physic::t5,
			*BiSymmetrical::pr, *BiSymmetrical::tr,
			*BiSymmetrical::py, *BiSymmetrical::px, *BiSymmetrical::tx, *BiSymmetrical::ty,
			*Point::py, *Point::px, *Point::tx, *Point::ty, *Body::tt, *Body::en_script,
			*Physic::p1, *Physic::p2, *Physic::p3, *Physic::t1, *Physic::t2, *Physic::t3, *Rotatable::ta, *Rotatable::pa,
			*Physic::set_but, *Physic::c_category, *Physic::t4, *Object::text, *Object::entry,
			*Body::combo, *Body::px, *Body::py, *Body::tx, *Body::ty, *Body::ps, *Body::cb, *Body::cr, *Body::ts,
			*Body::pad,*Body::pld,*Body::tad,*Body::tld,
			*Polygon::px, *Polygon::py, *Polygon::tx, *Polygon::ty,
			*Cover::px, *Cover::py, *Cover::tx, *Cover::ty,*Cover::td,*Cover::pd,
			*Joint::c_collide, *Joint::p1, *Joint::p2, *Joint::t1, *Joint::t2,
			*PointJoint::py, *PointJoint::px, *PointJoint::tx, *PointJoint::ty,
			*WeldJoint::ps, *WeldJoint::pd, *WeldJoint::ts, *WeldJoint::td,
			*GearJoint::text, *GearJoint::rat, *GearJoint::info,
			*RevoluteJoint::sl, *RevoluteJoint::pl, *RevoluteJoint::tl, *RevoluteJoint::pu, *RevoluteJoint::tu,
			*RevoluteJoint::sm, *RevoluteJoint::ts, *RevoluteJoint::ps, *RevoluteJoint::tt, *RevoluteJoint::pt,
			*PrismaticJoint::sl, *PrismaticJoint::pl, *PrismaticJoint::tl,
			*PrismaticJoint::pu, *PrismaticJoint::tu, *PrismaticJoint::ta,
			*PrismaticJoint::sm, *PrismaticJoint::ts, *PrismaticJoint::ps,
			*PrismaticJoint::tt, *PrismaticJoint::pt, *PrismaticJoint::pa,
			*DistanceJoint::px1, *DistanceJoint::px2, *DistanceJoint::py1, *DistanceJoint::py2,
			*DistanceJoint::tx1, *DistanceJoint::tx2, *DistanceJoint::ty1, *DistanceJoint::ty2,
			*DistanceJoint::pmin, *DistanceJoint::pmax, *DistanceJoint::ps, *DistanceJoint::pd,
			*DistanceJoint::tmax, *DistanceJoint::tmin, *DistanceJoint::ts, *DistanceJoint::td,
			*PulleyJoint::px1, *PulleyJoint::px2, *PulleyJoint::py1, *PulleyJoint::py2,
			*PulleyJoint::tx1, *PulleyJoint::tx2, *PulleyJoint::ty1, *PulleyJoint::ty2,
			*PulleyJoint::px3, *PulleyJoint::px4, *PulleyJoint::py3, *PulleyJoint::py4,
			*PulleyJoint::tx3, *PulleyJoint::tx4, *PulleyJoint::ty3, *PulleyJoint::ty4,
			*PulleyJoint::pr, *PulleyJoint::tr, *PulleyJoint::info,
			*Entity::text, *Entity::entry;

void hide_all() {
	Object::hide();
	Point::hide();
	Line::hide();
	Rect::hide();
	Physic::hide();
	Rotatable::hide();
	BiSymmetrical::hide();
	Polygon::hide();
	Cover::hide();
	Body::hide();
	Joint::hide();
	PointJoint::hide();
	WeldJoint::hide();
	GearJoint::hide();
	RevoluteJoint::hide();
	PrismaticJoint::hide();
	DistanceJoint::hide();
	PulleyJoint::hide();
	Entity::hide();
}
void create_all(GtkWidget *prop_table) {
	Object::init(prop_table);
	Point::init(prop_table);
	Line::init(prop_table);
	Rect::init(prop_table);
	Physic::init(prop_table);
	Rotatable::init(prop_table);
	BiSymmetrical::init(prop_table);
	Polygon::init(prop_table);
	Cover::init(prop_table);
	Body::init(prop_table);
	Joint::init(prop_table);
	PointJoint::init(prop_table);
	WeldJoint::init(prop_table);
	GearJoint::init(prop_table);
	RevoluteJoint::init(prop_table);
	PrismaticJoint::init(prop_table);
	DistanceJoint::init(prop_table);
	PulleyJoint::init(prop_table);
	Entity::init(prop_table);
}

vector <Object*> get_all() {
	vector <Object*> all;
	for(int q=0; q<level.bodies.size(); q++) {
		all.push_back(level.bodies[q]);
		for(int w=0; w<level.bodies[q]->shapes.size(); w++) {
			all.push_back(level.bodies[q]->shapes[w]);
		}

	}
	for(int q=0; q<level.joints.size(); q++) {
		all.push_back(level.joints[q]);
	}
	for(int q=0; q<level.entities.size(); q++) {
		all.push_back(level.entities[q]);
	}
	return all;
}

Object *get_selected_object() {
	vector <Object*> all=get_all();
	for(auto *shape : all) {
		if(shape->selected)return shape;
	}
	return 0;
}
Object *get_id(string id) {
	vector <Object*> all=get_all();
	for(auto *o : all) {
		if(o->id==id)return o;
	}
	return 0;
}

Body *get_selected_body() {
	for(Body *body : level.bodies) {
		if(body->selected)return body;
		for(Object *shape : body->shapes) {
			if(shape->selected)return body;
		}
	}
	return 0;
}
Body *parent(string id) {
	for(Body *b : level.bodies) {
		for(Object *o : b->shapes) {
			if(o->id==id)return b;
		}
		if(b->id==id)return b;
	}
	return 0;
}

bool delete_id(string id) {
	for(int q=0; q<level.bodies.size(); q++) {
		if(level.bodies[q]->id==id) {
			level.bodies.erase(level.bodies.begin()+q);
			return 1;
		}
		for(int e=0; e<level.bodies[q]->shapes.size(); e++) {
			if(level.bodies[q]->shapes[e]->id==id) {
				level.bodies[q]->shapes.erase(level.bodies[q]->shapes.begin()+e);
				return 1;
			}
		}
	}
	for(int q=0; q<level.joints.size(); q++) {
		if(level.joints[q]->id==id) {
			level.joints.erase(level.joints.begin()+q);
			return 1;
		}
	}
	for(int q=0; q<level.entities.size(); q++) {
		if(level.entities[q]->id==id) {
			level.entities.erase(level.entities.begin()+q);
			return 1;
		}
	}
	return 0;
}
void Object::init(GtkWidget* table) {
	text=gtk_label_new("ID");
	entry=gtk_entry_new();
	ins_text(table,text,cur_table_string);
	ins_widget(table,entry,cur_table_string++);
	g_signal_connect(G_OBJECT(entry),"notify::text",update1,0);
}
void Object::update(Object *p) {
	gtk_entry_set_text(GTK_ENTRY(entry),p->id.c_str());
}
void Object::update1() {
	Object *p=TYPE(Object*,get_selected_object());
	if(!p || point_ch || block)return;
	string t=gtk_entry_get_text(GTK_ENTRY(entry));
	vector<Object*> vec=get_all();
	if(t=="") {
		set_status(CONTEXT_ID_ERROR,"ID can't be empty");
		return;
	}
	for(int q=0; q<vec.size(); q++) {
		if(t==vec[q]->id && vec[q]!=p) {
			set_status(CONTEXT_ID_ERROR,"ID must be unique");
			return;
		}
	}
	remove_status(CONTEXT_ID_ERROR);
	change_id(p->id,t);
	p->id=t;
	gtk_widget_queue_draw(drawable);
}
void Object::hide() {
	gtk_widget_hide(text);
	gtk_widget_hide(entry);
}
void Object::show() {
	gtk_widget_show(text);
	gtk_widget_show(entry);
}
bool Object::drag(float xp,float yp,int dr) {
	return 0;
}
bool Object::create(float xp,float yp,int dr) {
	return 0;
}
bool Object::del(float xp,float yp,int dr) {
	return 0;
}
vector<b2Vec2*> Object::get_points() {
	return {};
}
string Object::name() const {
	return "Unknown";
}

void Object::draw(cairo_t *cr) {}
void Object::vupdate() {
	show();
	update(this);
};
b2Vec2 Object::mean()  {
	vector<b2Vec2*>p=get_points();
	b2Vec2 sum(0,0);
	for(int q=0; q<p.size(); q++)
		sum+=*p[q];
	return sum/(float)p.size();
}
void draw_drag_rect(cairo_t *cr, b2Vec2 pos,bool tc) {
	if(zoom<20)return;
	cairo_set_source_rgb(cr, 1.0,  0.0,(double)tc);
	cairo_rectangle(cr, drawx(pos.x)-POINT_SIZE,drawy(pos.y)-POINT_SIZE,POINT_SIZE*2,POINT_SIZE*2);
	cairo_stroke(cr);
}
void draw_drag_joint_rect(cairo_t *cr, b2Vec2 pos,bool tc) {
	if(zoom<20)return;
	cairo_set_source_rgb(cr, 0.0,  0.5,(double)tc);
	cairo_rectangle(cr, drawx(pos.x)-POINT_SIZE,drawy(pos.y)-POINT_SIZE,POINT_SIZE*2,POINT_SIZE*2);
	cairo_stroke(cr);
}
Point::Point(b2Vec2 _pos,string text) {
	pos=_pos;
	id=text;
}
void Point::draw(cairo_t *cr) {
	if(!shows[4])return;
	draw_drag_rect(cr,pos,selected&&point_ch);
	cairo_set_source_rgb(cr,BLACK);
	cairo_rectangle(cr,drawx(pos.x)-1,drawy(pos.y)-1,3,3);
	cairo_text_extents_t extents;
	cairo_text_extents(cr, id.c_str(), &extents);
	cairo_move_to(cr,drawx(pos.x)-extents.width/2,drawy(pos.y)-20);
	cairo_show_text(cr,id.c_str());
	cairo_fill(cr);
}
bool Point::drag(float xp,float yp,int dr) {
	if(dr==0 && touch(pos, {xp,yp})) {
		hide_all();

		point_ch=1;
		return 1;
	} else if(dr==1&&point_ch==1) {
		pos.x=to_grid(xp);
		pos.y=to_grid(yp);
		return 1;
	} else if(dr==2)point_ch=0;
	else if(dr==3)vupdate();
	return 0;
}
bool Point::create(float xp,float yp,int dr) {
	if(dr==0) {
		pos.x=to_grid(xp);
		pos.y=to_grid(yp);
		return 1;
	}
	return 0;
}
void Point::init(GtkWidget *table) {
	ax=gtk_adjustment_new(0,0,level.w,grid,grid,0);
	ay=gtk_adjustment_new(0,0,level.h,grid,grid,0);
	px=gtk_spin_button_new(GTK_ADJUSTMENT(ax),0,4);
	py=gtk_spin_button_new(GTK_ADJUSTMENT(ay),0,4);
	tx=gtk_label_new("X");
	ty=gtk_label_new("Y");

	ins_text	(table,tx,cur_table_string);
	ins_widget	(table,px,cur_table_string++);
	ins_text	(table,ty,cur_table_string);
	ins_widget	(table,py,cur_table_string++);
	g_signal_connect(G_OBJECT(px),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(py),"value_changed",update1,0);
}
void Point::show() {
	Object::show();
	gtk_widget_show(px);
	gtk_widget_show(py);
	gtk_widget_show(tx);
	gtk_widget_show(ty);
}
void Point::hide() {
	Object::hide();
	gtk_widget_hide(px);
	gtk_widget_hide(py);
	gtk_widget_hide(tx);
	gtk_widget_hide(ty);
}
void Point::update(Point *p) {
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax),p->pos.x,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay),p->pos.y,0,level.h,grid,0,0);
}
void Point::update1() {
	Point *p=TYPE(Point*,get_selected_object());
	if(!p || point_ch || block)return;
	p->pos.x=gtk_adjustment_get_value(GTK_ADJUSTMENT(ax));
	p->pos.y=gtk_adjustment_get_value(GTK_ADJUSTMENT(ay));
	gtk_widget_queue_draw(drawable);
}
vector<b2Vec2*> Point::get_points() {
	vector<b2Vec2*>vec;
	vec.push_back(&pos);
	return vec;
}
void Point::vupdate() {
	show();
	update(this);
	Object::update(this);
}
string Point::name() const {
	return "Point";
}
void Entity::init(GtkWidget *table) {
	text=gtk_label_new("Type");
	entry=gtk_entry_new();
	ins_text	(table,text,cur_table_string);
	ins_widget	(table,entry,cur_table_string++);
	g_signal_connect(G_OBJECT(entry),"notify::text",update1,0);
}
void Entity::show() {
	Point::show();
	gtk_widget_show(text);
	gtk_widget_show(entry);
}
void Entity::hide() {
	Point::hide();
	gtk_widget_hide(text);
	gtk_widget_hide(entry);
}
void Entity::update(Entity *p) {
	gtk_entry_set_text(GTK_ENTRY(entry),p->type.c_str());
}
void Entity::update1() {
	Entity *p=TYPE(Entity*,get_selected_object());
	if(!p || point_ch || block)return;
	p->type=gtk_entry_get_text(GTK_ENTRY(entry));
}
void Entity::vupdate() {
	show();
	update(this);
	Point::update(this);
	Object::update(this);
}
string Entity::name() const {
	return "Entity";
}
