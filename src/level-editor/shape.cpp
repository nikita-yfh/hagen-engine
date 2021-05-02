#include "shape.hpp"
#include "level.hpp"
#include "utils.hpp"
#include "events.hpp"
#include "editor.hpp"
#include "tree_view.hpp"
#include <cmath>
using namespace std;
string find_shape_1(b2Vec2 pos) {
	for(Body *bd : level.bodies) {
		for(Physic *sh : bd->shapes) {
			if(sh->drag(pos.x,pos.y,4))
				return bd->id;
		}
	}
	return "";
}
string find_shape_2(b2Vec2 pos) {
	Body *b1=0;
	for(Body *bd : level.bodies) {
		for(Physic *sh : bd->shapes) {
			if(sh->drag(pos.x,pos.y,4)) {
				if(b1!=bd && b1!=0)
					return bd->id;
				else
					b1=bd;
			}

		}
	}
	if(b1)return b1->id;
	return "";
}
BiSymmetrical::BiSymmetrical(b2Vec2 _pos,float _r) {
	pos=_pos;
	r=_r;
}
bool BiSymmetrical::drag(float xp,float yp,int dr) {
	if(!shows[layer])return 0;
	if(!shows[3+parent(id)->type])return 0;
	if(dr==0) {
		if(touch(pos+b2Vec2(r,0), {xp,yp}))	point_ch=2;
		else	if(touch(pos, {xp,yp}))	point_ch=1;
		else return 0;
		hide_all();
		vupdate();
		return 1;
	} else if(dr==1&&point_ch) {
		if(point_ch==1) {
			pos.x=to_grid(xp);
			pos.y=to_grid(yp);
		} else {
			r=to_grid((pos-b2Vec2(xp,yp)).Length());
		}
		return 1;
	} else if(dr==2)point_ch=0;
	else if(dr==3)vupdate();
	else if(dr==4) {
		if(name()=="Circle")
			return (pos-b2Vec2(xp,yp)).Length()<=r;
		else
			return abs(xp-pos.x)<=r && abs(yp-pos.y)<=r;
	}
	return 0;
}
bool BiSymmetrical::create(float xp,float yp,int dr) {
	if(dr==0) {
		pos.x=to_grid(xp);
		pos.y=to_grid(yp);
		r=0;
	} else if(dr==1) {
		r=to_grid((pos-b2Vec2(xp,yp)).Length());
	} else if(dr==3) {
		if(r==0)return 1;
	}
	return 0;
}
void BiSymmetrical::init(GtkWidget *table) {
	ax=gtk_adjustment_new(0,0,level.w,grid,grid,0);
	ay=gtk_adjustment_new(0,0,level.h,grid,grid,0);
	ar=gtk_adjustment_new(0,0,1000,grid,grid,0);
	px=gtk_spin_button_new(GTK_ADJUSTMENT(ax),0,4);
	py=gtk_spin_button_new(GTK_ADJUSTMENT(ay),0,4);
	pr=gtk_spin_button_new(GTK_ADJUSTMENT(ar),0,4);
	tx=gtk_label_new("X");
	ty=gtk_label_new("Y");
	tr=gtk_label_new("Radius");

	ins_text	(table,tx,cur_table_string);
	ins_widget	(table,px,cur_table_string++);
	ins_text	(table,ty,cur_table_string);
	ins_widget	(table,py,cur_table_string++);
	ins_text	(table,tr,cur_table_string);
	ins_widget	(table,pr,cur_table_string++);
	g_signal_connect(G_OBJECT(px),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(py),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(pr),"value_changed",update1,0);

}
void BiSymmetrical::show() {
	Physic::show();
	gtk_widget_show(px);
	gtk_widget_show(py);
	gtk_widget_show(pr);
	gtk_widget_show(tx);
	gtk_widget_show(ty);
	gtk_widget_show(tr);
}
void BiSymmetrical::hide() {
	Physic::hide();
	gtk_widget_hide(px);
	gtk_widget_hide(py);
	gtk_widget_hide(pr);
	gtk_widget_hide(tx);
	gtk_widget_hide(ty);
	gtk_widget_hide(tr);
}
void BiSymmetrical::update(BiSymmetrical *p) {
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax),p->pos.x,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay),p->pos.y,0,level.h,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ar),p->r,0,1000,grid,0,0);
}
void BiSymmetrical::update1() {
	BiSymmetrical *p=TYPE(BiSymmetrical*,get_selected_object());
	if(!p || point_ch)return;
	p->pos.x=gtk_adjustment_get_value(GTK_ADJUSTMENT(ax));
	p->pos.y=gtk_adjustment_get_value(GTK_ADJUSTMENT(ay));
	p->r=gtk_adjustment_get_value(GTK_ADJUSTMENT(ar));
	gtk_widget_queue_draw(drawable);
}
vector<b2Vec2*> BiSymmetrical::get_points() {
	return {&pos};
}
void BiSymmetrical::vupdate() {
	show();
	update(this);
	Physic::update(this);
	Object::update(this);
}
Square::Square(b2Vec2 _pos,float rp) {
	pos=_pos;
	r=rp;
}
void Square::draw(cairo_t *cr) {
	if(!shows[layer])return;
	if(!shows[3+parent(id)->type])return;
	draw_drag_rect(cr,pos,selected&&point_ch==1);
	draw_drag_rect(cr,pos+b2Vec2(r,0),selected&&point_ch==2);
	set_shape_color(cr,parent(id)->type);
	cairo_rectangle(cr,drawx(pos.x-r),drawy(pos.y-r),drawx(r*2)-cx,drawy(r*2)-cy);
	cairo_stroke_preserve(cr);
	cairo_fill(cr);
}
Circle::Circle(b2Vec2 _pos,float rp) {
	pos=_pos;
	r=rp;
}
string Square::name() {
	return "Square";
}
void Circle::draw(cairo_t *cr) {
	if(!shows[layer])return;
	if(!shows[3+parent(id)->type])return;
	draw_drag_rect(cr,pos,selected&&point_ch==1);
	draw_drag_rect(cr,pos+b2Vec2(r,0),selected&&point_ch==2);
	set_shape_color(cr,parent(id)->type);
	cairo_arc(cr,drawx(pos.x),drawy(pos.y),r*zoom,0,2*M_PI);
	cairo_stroke_preserve(cr);
	cairo_fill(cr);
}
string Circle::name() {
	return "Circle";
}

bool BiPoints::drag(float xp,float yp,int dr) {
	if(!shows[layer])return 0;
	if(!shows[3+parent(id)->type])return 0;
	if(dr==0) {
		if(touch(p1, {xp,yp}))	point_ch=1;
		else	if(touch(p2, {xp,yp}))	point_ch=2;
		else return 0;
		hide_all();
		vupdate();
		return 1;
	} else if(dr==1&&point_ch) {
		if(point_ch==1) {
			p1.x=to_grid(xp);
			p1.y=to_grid(yp);
		} else if(point_ch==2) {
			p2.x=to_grid(xp);
			p2.y=to_grid(yp);
		}
		return 1;
	} else if(dr==2)point_ch=0;
	else if(dr==3)vupdate();
	else if(dr==4) {
		if(name()=="Rect")
			return abs((p1.x+p2.x)/2-xp)<=abs(p1.y-p2.y)/2
				   && abs((p1.y+p2.y)/2-yp)<=abs(p1.y-p2.y)/2;
		else
			return 0;
	}
	return 0;
}
bool BiPoints::create(float xp,float yp,int dr) {
	if(dr==0) {
		p1.x=p2.x=to_grid(xp);
		p1.y=p2.y=to_grid(yp);
	} else if(dr==1) {
		p2.x=to_grid(xp);
		p2.x=to_grid(yp);
	} else if(dr==3) {
		if(p1==p2)return 1;
	}
	return 0;
}
vector<b2Vec2*> BiPoints::get_points() {
	return {&p1,&p2};
}
void BiPoints::init(GtkWidget *table) {
	ax1=gtk_adjustment_new(0,0,level.w,grid,grid,0);
	ay1=gtk_adjustment_new(0,0,level.h,grid,grid,0);
	ax2=gtk_adjustment_new(0,0,level.w,grid,grid,0);
	ay2=gtk_adjustment_new(0,0,level.h,grid,grid,0);
	px1=gtk_spin_button_new(GTK_ADJUSTMENT(ax1),0,4);
	py1=gtk_spin_button_new(GTK_ADJUSTMENT(ay1),0,4);
	px2=gtk_spin_button_new(GTK_ADJUSTMENT(ax2),0,4);
	py2=gtk_spin_button_new(GTK_ADJUSTMENT(ay2),0,4);
	tx1=gtk_label_new("X1");
	ty1=gtk_label_new("Y1");
	tx2=gtk_label_new("X2");
	ty2=gtk_label_new("Y2");
	ins_text(table,tx1,cur_table_string);
	ins_widget(table,px1,cur_table_string++);
	ins_text(table,ty1,cur_table_string);
	ins_widget(table,py1,cur_table_string++);
	ins_text(table,tx2,cur_table_string);
	ins_widget(table,px2,cur_table_string++);
	ins_text(table,ty2,cur_table_string);
	ins_widget(table,py2,cur_table_string++);


	g_signal_connect(G_OBJECT(px1),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(py1),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(px2),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(py2),"value_changed",update1,0);

}

void BiPoints::show() {
	Physic::show();
	gtk_widget_show(px1);
	gtk_widget_show(py1);
	gtk_widget_show(px2);
	gtk_widget_show(py2);
	gtk_widget_show(tx1);
	gtk_widget_show(ty1);
	gtk_widget_show(tx2);
	gtk_widget_show(ty2);
}
void BiPoints::hide() {
	Physic::hide();
	gtk_widget_hide(px1);
	gtk_widget_hide(py1);
	gtk_widget_hide(px2);
	gtk_widget_hide(py2);
	gtk_widget_hide(tx1);
	gtk_widget_hide(ty1);
	gtk_widget_hide(tx2);
	gtk_widget_hide(ty2);
}
void BiPoints::update(BiPoints *p) {
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax1),p->p1.x,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay1),p->p1.y,0,level.h,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax2),p->p2.x,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay2),p->p2.y,0,level.h,grid,0,0);
}
void BiPoints::update1() {
	BiPoints *p=TYPE(BiPoints*,get_selected_object());
	if(!p || point_ch)return;
	p->p1.x=gtk_adjustment_get_value(GTK_ADJUSTMENT(ax1));
	p->p1.y=gtk_adjustment_get_value(GTK_ADJUSTMENT(ay1));
	p->p2.x=gtk_adjustment_get_value(GTK_ADJUSTMENT(ax2));
	p->p2.y=gtk_adjustment_get_value(GTK_ADJUSTMENT(ay2));
	gtk_widget_queue_draw(drawable);
}
void BiPoints::vupdate() {
	show();
	update(this);
	Physic::update(this);
	Object::update(this);
}
Rect::Rect(b2Vec2 _p1,b2Vec2 _p2) {
	p1=_p1;
	p2=_p2;
}
string Rect::name() {
	return "Rect";
}
void Rect::draw(cairo_t *cr) {
	if(!shows[layer])return;
	if(!shows[3+parent(id)->type])return;
	draw_drag_rect(cr,p1,selected&&point_ch==1);
	draw_drag_rect(cr,p2,selected&&point_ch==2);
	set_shape_color(cr,parent(id)->type);
	cairo_rectangle(cr,drawx(p1.x),drawy(p1.y),(p2-p1).x*zoom,(p2-p1).y*zoom);
	cairo_stroke_preserve(cr);
	cairo_fill(cr);
}
Polygon::Polygon() {
	ex=0;
}
Polygon::Polygon(std::vector<b2Vec2>p) {
	points=p;
	ex=0;
}
bool Polygon::drag(float xp,float yp,int dr) {
	if(!shows[layer])return 0;
	if(!shows[3+parent(id)->type])return 0;
	if(dr==0) {
		for(int q=0; q<size(); q++) {
			if(touch(points[q], {xp,yp})) {
				point_ch=q+1;
				hide_all();
				vupdate();
				return 1;
			}
			if(touch((points[(q+1)%size()]+points[q])/2, {xp,yp})) {
				points.insert(points.begin()+q+1, {to_grid(xp),to_grid(yp)});
				point_ch=q+2;
				hide_all();
				vupdate();
				return 1;
			}
		}
		return 0;
	} else if(dr==1&&point_ch) {
		points[point_ch-1]= {to_grid(xp),to_grid(yp)};
		return 1;
	} else if(dr==2)point_ch=0;
	else if(dr==3)vupdate();
	else if(dr==4) {
		b2Vec2 min(10000,10000),max(-10000,-10000);
		for(b2Vec2 p : points) {
			if(p.x<min.x)min.x=p.x;
			if(p.y<min.y)min.y=p.y;
			if(p.x>max.x)max.x=p.x;
			if(p.y>max.y)max.y=p.y;
		}
		cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, max.x-min.x,max.y-min.y);
		cairo_t *cr = cairo_create (surface);
		cairo_move_to(cr,points[points.size()-1].x,points[points.size()-1].y);
		for(int q=0; q<size(); q++)
			cairo_line_to(cr,points[q].x,points[q].y);
		return cairo_in_fill(cr,xp,yp);
	} else if(dr==5) {
		for(int q=0; q<size(); q++) {
			if(touch(points[q], {xp,yp})) {
				points.erase(points.begin()+q);
				return 1;
			}
		}
	}
	return 0;
}
bool Polygon::create(float xp,float yp,int dr) {
	if(dr==0) {
		for(int q=0; q<1+(size()==1); q++)
			points.emplace_back(to_grid(xp),to_grid(yp));
		return 1;
	} else if(dr==1) {
		points[points.size()-1]= {to_grid(xp),to_grid(yp)};
	} else if(dr==2) {
		return 1;
	} else if(dr==3) {
		points.pop_back();
		if(size()<3)return 1;
	}
	return 0;
}

void Polygon::draw(cairo_t *cr) {
	if(!shows[layer])return;
	if(!shows[3+parent(id)->type])return;
	for(int q=0; q<size(); q++) {
		draw_drag_rect(cr,points[q],selected&&point_ch==(q+1));
		draw_drag_rect(cr,(points[(q+1)%size()]+points[q])/2,0);
	}
	set_shape_color(cr,parent(id)->type);
	cairo_move_to(cr,drawx(points[size()-1].x),drawy(points[size()-1].y));
	for(int q=0; q<size(); q++)
		cairo_line_to(cr,drawx(points[q].x),drawy(points[q].y));
	cairo_stroke_preserve(cr);
	cairo_fill(cr);
}
vector<b2Vec2*> Polygon::get_points() {
	vector<b2Vec2*>a;
	for(int q=0; q<size(); q++)
		a.push_back(&points[q]);
	return a;
}
void Polygon::init(GtkWidget* table) {
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
void Polygon::show() {
	Physic::show();
	gtk_widget_show(px);
	gtk_widget_show(py);
	gtk_widget_show(tx);
	gtk_widget_show(ty);
}
void Polygon::hide() {
	Physic::hide();
	gtk_widget_hide(px);
	gtk_widget_hide(py);
	gtk_widget_hide(tx);
	gtk_widget_hide(ty);
}
void Polygon::update(Polygon *p) {
	b2Vec2 vec=p->mean();
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax),vec.x,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay),vec.y,0,level.h,grid,0,0);
}
void Polygon::update1() {
	if(block)return;
	Polygon *p=TYPE(Polygon*,get_selected_object());
	if(!p || point_ch)return;
	int xp=gtk_adjustment_get_value(GTK_ADJUSTMENT(ax));
	int yp=gtk_adjustment_get_value(GTK_ADJUSTMENT(ay));
	b2Vec2 vec=p->mean();
	for(int q=0; q<p->size(); q++) {
		p->points[q]+=(b2Vec2(xp,yp)-vec);
	}
	gtk_widget_queue_draw(drawable);
}
void Polygon::vupdate() {
	show();
	update(this);
	Physic::update(this);
	Object::update(this);
}
string Polygon::name() {
	return "Polygon";
}
int Polygon::size() {
	return points.size();
}
Cover::Cover() {
	ex=0;
}
Cover::Cover(std::vector<b2Vec2>p) {
	points=p;
	ex=0;
}
bool Cover::drag(float xp,float yp,int dr) {
	if(!shows[layer])return 0;
	if(!shows[3+parent(id)->type])return 0;
	if(dr==0) {
		for(int q=0; q<size(); q++) {
			if(touch(points[q], {xp,yp})) {
				point_ch=q+1;
				hide_all();
				vupdate();
				return 1;
			}
			if(touch((points[(q+1)%size()]+points[q])/2, {xp,yp})) {
				points.insert(points.begin()+q+1, {to_grid(xp),to_grid(yp)});
				point_ch=q+2;
				hide_all();
				vupdate();
				return 1;
			}
		}
		return 0;
	} else if(dr==1&&point_ch) {
		points[point_ch-1]= {to_grid(xp),to_grid(yp)};
		return 1;
	} else if(dr==2)point_ch=0;
	else if(dr==3)vupdate();
	else if(dr==4) {
		b2Vec2 min(10000,10000),max(-10000,-10000);
		for(b2Vec2 p : points) {
			if(p.x<min.x)min.x=p.x;
			if(p.y<min.y)min.y=p.y;
			if(p.x>max.x)max.x=p.x;
			if(p.y>max.y)max.y=p.y;
		}
		cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, max.x-min.x,max.y-min.y);
		cairo_t *cr = cairo_create (surface);
		cairo_move_to(cr,points[points.size()-1].x,points[points.size()-1].y);
		for(int q=0; q<size(); q++)
			cairo_line_to(cr,points[q].x,points[q].y);
		return cairo_in_fill(cr,xp,yp);
	} else if(dr==5) {
		for(int q=0; q<size(); q++) {
			if(touch(points[q], {xp,yp})) {
				points.erase(points.begin()+q);
				return 1;
			}
		}
	}
	return 0;
}
bool Cover::create(float xp,float yp,int dr) {
	if(dr==0) {
		for(int q=0; q<1+(size()==1); q++)
			points.emplace_back(to_grid(xp),to_grid(yp));
		return 1;
	} else if(dr==1) {
		points[points.size()-1]= {to_grid(xp),to_grid(yp)};
	} else if(dr==2) {
		return 1;
	} else if(dr==3) {
		points.pop_back();
		if(size()<3)return 1;
	}
	return 0;
}

void Cover::draw(cairo_t *cr) {
	if(!shows[layer])return;
	if(!shows[3+parent(id)->type])return;
	for(int q=0; q<size(); q++) {
		draw_drag_rect(cr,points[q],selected&&point_ch==(q+1));
		if(q!=0)
			draw_drag_rect(cr,(points[q-1]+points[q])/2,0);
	}
	set_shape_color(cr,parent(id)->type);
	for(int q=0; q<size(); q++)
		cairo_line_to(cr,drawx(points[q].x),drawy(points[q].y));
	cairo_stroke(cr);
	for(int q=0; q<size(); q++) {
		b2Vec2 prev=(q==0)			?	2*points[q]-points[q+1]	:	points[q-1];
		b2Vec2 next=(q==size()-1)	?	2*points[q]-points[q-1]	:	points[q+1];

		b2Vec2 b=bis(prev-points[q],next-points[q],0.3f);
		b2Vec2 p=b+points[q];
		cairo_line_to(cr,drawx(p.x),drawy(p.y));
		cairo_line_to(cr,drawx(points[q].x),drawy(points[q].y));
		cairo_line_to(cr,drawx(p.x),drawy(p.y));
	}
	cairo_stroke(cr);
}
vector<b2Vec2*> Cover::get_points() {
	vector<b2Vec2*>a;
	for(int q=0; q<size(); q++)
		a.emplace_back(&points[q]);
	return a;
}
void Cover::init(GtkWidget* table) {
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
void Cover::show() {
	Physic::show();
	gtk_widget_show(px);
	gtk_widget_show(py);
	gtk_widget_show(tx);
	gtk_widget_show(ty);
}
void Cover::hide() {
	Physic::hide();
	gtk_widget_hide(px);
	gtk_widget_hide(py);
	gtk_widget_hide(tx);
	gtk_widget_hide(ty);
}
void Cover::update(Cover *p) {
	b2Vec2 vec=p->mean();
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax),vec.x,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay),vec.y,0,level.h,grid,0,0);
}
void Cover::update1() {
	if(block)return;
	Cover *p=TYPE(Cover*,get_selected_object());
	if(!p || point_ch)return;
	int xp=gtk_adjustment_get_value(GTK_ADJUSTMENT(ax));
	int yp=gtk_adjustment_get_value(GTK_ADJUSTMENT(ay));
	b2Vec2 vec=p->mean();
	for(int q=0; q<p->size(); q++) {
		p->points[q]+=(b2Vec2(xp,yp)-vec);
	}
	gtk_widget_queue_draw(drawable);
}
void Cover::vupdate() {
	show();
	update(this);
	Physic::update(this);
	Object::update(this);
}
string Cover::name() {
	return "Cover";
}
int Cover::size() {
	return points.size();
}
Line::Line(b2Vec2 _p1,b2Vec2 _p2) {
	p1=_p1;
	p2=_p2;
}
void Line::draw(cairo_t *cr) {
	if(!shows[layer])return;
	if(!shows[3+parent(id)->type])return;
	draw_drag_rect(cr,p1,selected&&point_ch==1);
	draw_drag_rect(cr,p2,selected&&point_ch==2);
	set_shape_color(cr,parent(id)->type);
	cairo_move_to(cr,drawx(p1.x),drawy(p1.y));
	cairo_line_to(cr,drawx(p2.x),drawy(p2.y));
	cairo_stroke(cr);
}
string Line::name() {
	return "Line";
}
void Physic::init(GtkWidget *table) {
	a1=gtk_adjustment_new(0,0,DENSITY_MAX,		DENSITY_STEP,		DENSITY_STEP,0);
	a2=gtk_adjustment_new(0,0,FRICTION_MAX,		FRICTION_STEP,		FRICTION_STEP,0);
	a3=gtk_adjustment_new(0,0,RESTITUTION_MAX,	RESTITUTION_STEP,	RESTITUTION_STEP,0);
	p1=gtk_spin_button_new(GTK_ADJUSTMENT(a1),0,4);
	p2=gtk_spin_button_new(GTK_ADJUSTMENT(a2),0,4);
	p3=gtk_spin_button_new(GTK_ADJUSTMENT(a3),0,4);
	t1=gtk_label_new("Density");
	t2=gtk_label_new("Friction");
	t3=gtk_label_new("Restitution");
	t4=gtk_label_new("Collision category");
	text=gtk_label_new("Texture");
	expand=gtk_check_button_new_with_label("Expand texture");
	entry=gtk_entry_new();
	combo=gtk_combo_box_new_text();
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"Background");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"Background physic");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"Physic");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"Foreground physic");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"Liquid");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"Foreground");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"None");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo),2);
	c_category=gtk_combo_box_new_text();
	for(int q=0; q<16; q++) {
		gtk_combo_box_append_text(GTK_COMBO_BOX(c_category),to_str(q).c_str());
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(c_category),0);
	set_but=gtk_button_new_with_label("Set collision mask");
	ins_text(table,t1,cur_table_string);
	ins_widget(table,p1,cur_table_string++);
	ins_text(table,t2,cur_table_string);
	ins_widget(table,p2,cur_table_string++);
	ins_text(table,t3,cur_table_string);
	ins_widget(table,p3,cur_table_string++);
	ins_text(table,text,cur_table_string);
	ins_widget(table,entry,cur_table_string++);
	ins_widget2(table,expand,cur_table_string++);
	ins_widget2(table,combo,cur_table_string++);
	ins_widget2(table,t4,cur_table_string++);
	ins_widget2(table,c_category,cur_table_string++);
	ins_widget2(table,set_but,cur_table_string++);
	g_signal_connect(G_OBJECT(p1),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(p2),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(p3),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(combo),"changed",update1,0);
	g_signal_connect(G_OBJECT(c_category),"changed",update1,0);
	g_signal_connect(G_OBJECT(entry),"notify::text",update1,0);
	g_signal_connect(G_OBJECT(expand),"toggled",update1,0);
	g_signal_connect(G_OBJECT(set_but),"clicked",change_mask,0);
}
void Physic::update(Physic *p) {
	gtk_adjustment_set_value(GTK_ADJUSTMENT(a1),p->density);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(a2),p->friction);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(a3),p->restitution);
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo),p->layer);
	gtk_combo_box_set_active(GTK_COMBO_BOX(c_category),p->category);
	gtk_entry_set_text(GTK_ENTRY(entry),p->texture.c_str());
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(expand),p->ex);
}
void Physic::update1() {
	Physic *p=TYPE(Physic*,get_selected_object());
	if(!p || point_ch)return;
	p->density=gtk_adjustment_get_value(GTK_ADJUSTMENT(a1));
	p->friction=gtk_adjustment_get_value(GTK_ADJUSTMENT(a2));
	p->restitution=gtk_adjustment_get_value(GTK_ADJUSTMENT(a3));
	gtk_widget_queue_draw(drawable);
	p->layer=gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
	p->category=gtk_combo_box_get_active(GTK_COMBO_BOX(c_category));
	string str=gtk_entry_get_text(GTK_ENTRY(entry));
	p->ex=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(expand));
	bool ok=0;
	for(int q=0; q<str.size(); q++)
		if(str[q]=='.')ok=1;
	if(ok || !str.size()) {
		remove_status(CONTEXT_TEXTURE_ERROR);
		p->texture=str;
	} else {
		set_status(CONTEXT_TEXTURE_ERROR,"Image can't be without type");
	}
	gtk_widget_queue_draw(drawable);
}
void Physic::show() {
	Object::show();
	gtk_widget_show(p1);
	gtk_widget_show(p2);
	gtk_widget_show(p3);
	gtk_widget_show(t1);
	gtk_widget_show(t2);
	gtk_widget_show(t3);
	gtk_widget_show(t4);
	gtk_widget_show(combo);
	gtk_widget_show(text);
	gtk_widget_show(entry);
	gtk_widget_show(expand);
	gtk_widget_show(c_category);
	gtk_widget_show(set_but);
}
void Physic::hide() {
	Object::hide();
	gtk_widget_hide(p1);
	gtk_widget_hide(p2);
	gtk_widget_hide(p3);
	gtk_widget_hide(t1);
	gtk_widget_hide(t2);
	gtk_widget_hide(t3);
	gtk_widget_hide(t4);
	gtk_widget_hide(combo);
	gtk_widget_hide(text);
	gtk_widget_hide(entry);
	gtk_widget_hide(expand);
	gtk_widget_hide(c_category);
	gtk_widget_hide(set_but);
}
void Physic::vupdate() {
	show();
	update(this);
	Object::update(this);
}


