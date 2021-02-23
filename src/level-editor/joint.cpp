#include "joint.h"
#include "level.h"
#include "utils.h"
#include "events.h"
#include "editor.h"
#include "tree_view.h"
using namespace std;
void Joint::init(GtkWidget* table) {
	t1=gtk_label_new("ID1");
	t2=gtk_label_new("ID2");
	p1=gtk_entry_new();
	p2=gtk_entry_new();
	c_collide=gtk_check_button_new_with_label("Collide connected");
	ins_text(table,t1,cur_table_string);
	ins_widget(table,p1,cur_table_string++);
	ins_text(table,t2,cur_table_string);
	ins_widget(table,p2,cur_table_string++);
	ins_widget2(table,c_collide,cur_table_string++);
	g_signal_connect(G_OBJECT(p1),"notify::text",update1,0);
	g_signal_connect(G_OBJECT(p2),"notify::text",update1,0);
	g_signal_connect(G_OBJECT(c_collide),"toggled",update1,0);
}
void Joint::update(Joint *p) {
	gtk_entry_set_text(GTK_ENTRY(p1),p->id1.c_str());
	gtk_entry_set_text(GTK_ENTRY(p2),p->id2.c_str());
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(c_collide),p->collide);
	gtk_widget_queue_draw(drawable);
}
void Joint::update1() {
	Joint *p=TYPE(Joint*,get_selected_object());
	if(!p || point_ch)return;
	p->id1=gtk_entry_get_text(GTK_ENTRY(p1));
	p->id2=gtk_entry_get_text(GTK_ENTRY(p2));
	p->collide=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(c_collide));
}
void Joint::hide() {
	Object::hide();
	gtk_widget_hide(p1);
	gtk_widget_hide(p2);
	gtk_widget_hide(t1);
	gtk_widget_hide(t2);
	gtk_widget_hide(c_collide);
}
void Joint::show() {
	Object::show();
	gtk_widget_show(p1);
	gtk_widget_show(p2);
	gtk_widget_show(t1);
	gtk_widget_show(t2);
	gtk_widget_show(c_collide);
}
void Joint::vupdate() {
	update(this);
	Object::update(this);
}
vector<float*> PointJoint::get_xpoints() {
	return {&x};
}
vector<float*> PointJoint::get_ypoints() {
	return {&y};
}
void PointJoint::init(GtkWidget *table) {
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
void PointJoint::show() {
	Joint::show();
	gtk_widget_show(px);
	gtk_widget_show(py);
	gtk_widget_show(tx);
	gtk_widget_show(ty);
}
void PointJoint::hide() {
	Joint::hide();
	gtk_widget_hide(px);
	gtk_widget_hide(py);
	gtk_widget_hide(tx);
	gtk_widget_hide(ty);
}
void PointJoint::update(PointJoint *p) {
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax),p->x,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay),p->y,0,level.h,grid,0,0);
}
void PointJoint::update1() {
	PointJoint *p=TYPE(PointJoint*,get_selected_object());
	if(!p || point_ch)return;
	p->x=gtk_adjustment_get_value(GTK_ADJUSTMENT(ax));
	p->y=gtk_adjustment_get_value(GTK_ADJUSTMENT(ay));
	gtk_widget_queue_draw(drawable);
}
void PointJoint::vupdate() {
	update(this);
	Joint::update(TYPE(Joint*,this));
	Object::update(TYPE(Object*,this));
}
void PointJoint::draw_lines(cairo_t* cr) {
	cairo_set_source_rgb(cr,GRAY);
	if(id1.size()) {
		Object *o=get_id(id1);
		if(o) {
			cairo_move_to(cr,drawx(x),drawy(y));
			cairo_line_to(cr,drawx(o->mean_x()),drawy(o->mean_y()));
		}
	}
	if(id2.size()) {
		Object *o=get_id(id2);
		if(o) {
			cairo_move_to(cr,drawx(x),drawy(y));
			cairo_line_to(cr,drawx(o->mean_x()),drawy(o->mean_y()));
		}
	}
	cairo_stroke(cr);
}

bool PointJoint::create(float xp,float yp,int dr) {
	if(dr==0) {
		x=to_grid(xp);
		y=to_grid(yp);
		id1=find_shape_1(x,y);
		id2=find_shape_2(x,y);
	}else if(dr==1) {
		id2=find_shape_2(xp,yp);
	}
	return 0;
}
void WeldJoint::init(GtkWidget *table) {
	as=gtk_adjustment_new(0,0,1000000000,1,1,0);
	ad=gtk_adjustment_new(0,0,2,0.1,0.1,0);
	ps=gtk_spin_button_new(GTK_ADJUSTMENT(as),0,4);
	pd=gtk_spin_button_new(GTK_ADJUSTMENT(ad),0,4);
	ts=gtk_label_new("Stiffness");
	td=gtk_label_new("Damping");

	ins_text	(table,ts,cur_table_string);
	ins_widget	(table,ps,cur_table_string++);
	ins_text	(table,td,cur_table_string);
	ins_widget	(table,pd,cur_table_string++);
	g_signal_connect(G_OBJECT(ps),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(pd),"value_changed",update1,0);
}
void WeldJoint::draw(cairo_t *cr) {
	if(!shows[6])return;
	draw_lines(cr);
	draw_drag_joint_rect(cr,x,y,selected&&point_ch);
	cairo_set_source_rgb(cr,BLACK);
	cairo_rectangle(cr,drawx(x)-1,drawy(y)-1,3,3);
	cairo_fill(cr);
}
void WeldJoint::show() {
	PointJoint::show();
	gtk_widget_show(ps);
	gtk_widget_show(pd);
	gtk_widget_show(ts);
	gtk_widget_show(td);
}
void WeldJoint::hide() {
	PointJoint::hide();
	gtk_widget_hide(ps);
	gtk_widget_hide(pd);
	gtk_widget_hide(ts);
	gtk_widget_hide(td);
}
void WeldJoint::update(WeldJoint *p) {
	gtk_adjustment_configure(GTK_ADJUSTMENT(as),p->stiffness,0,1000000000,100,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ad),p->damping,0,2,1,1,0);
}
void WeldJoint::update1() {
	WeldJoint *p=TYPE(WeldJoint*,get_selected_object());
	if(!p || point_ch)return;
	p->stiffness=gtk_adjustment_get_value(GTK_ADJUSTMENT(as));
	p->damping=gtk_adjustment_get_value(GTK_ADJUSTMENT(ad));
	gtk_widget_queue_draw(drawable);
}
void WeldJoint::vupdate() {
	update(this);
	PointJoint::update(this);
	Joint::update(this);
	Object::update(this);
}
string WeldJoint::name() {
	return "WeldJoint";
}
bool WeldJoint::drag(float xp,float yp,int dr) {
	if(!shows[6])return 0;
	if(dr==0 && touch(x,y,xp,yp)) {
		hide_all();
		show();
		point_ch=1;
		return 1;
	} else if(dr==1&&point_ch==1) {
		x=to_grid(xp);
		y=to_grid(yp);
		return 1;
	} else if(dr==2)point_ch=0;
	else if(dr==3)show();
	return 0;
}
void GearJoint::init(GtkWidget *table) {
	ar=gtk_adjustment_new(1,0.05,20,0.01,0.01,0);
	rat=gtk_spin_button_new(GTK_ADJUSTMENT(ar),0,2);
	info=gtk_label_new("value1 + ratio * value2 = const");
	text=gtk_label_new("Ratio");

	ins_widget2	(table,info,cur_table_string++);
	ins_text	(table,text,cur_table_string);
	ins_widget	(table,rat,cur_table_string++);
	g_signal_connect(G_OBJECT(rat),"value_changed",update1,0);
}
void GearJoint::draw(cairo_t *cr) {
	if(!shows[6])return;
	if(id1.size()&&id2.size()) {
		Object *a=get_id(id1);
		Object *b=get_id(id2);
		if(a&&(a->name()=="RevoluteJoint"||a->name()=="PrismaticJoint")&&
				b&&(b->name()=="RevoluteJoint"||b->name()=="PrismaticJoint")) {
			cairo_set_source_rgb(cr,BLACK);
			cairo_move_to(cr,drawx(a->mean_x()),drawy(a->mean_y()));
			cairo_line_to(cr,drawx(b->mean_x()),drawy(b->mean_y()));
			cairo_stroke(cr);
		}
	}
}
bool GearJoint::drag(float xp,float yp,int dr) {
	if(dr==0)
		return 0;
	else if(dr==1)
		return 1;
	else if(dr==3)show();
	return 0;
}
bool GearJoint::create(float xp,float yp,int dr) {
	if(dr==0)
		return 1;
	return 0;
}
void GearJoint::show() {
	Joint::show();
	gtk_widget_show(rat);
	gtk_widget_show(text);
	gtk_widget_show(info);
}
void GearJoint::hide() {
	Joint::hide();
	gtk_widget_hide(rat);
	gtk_widget_hide(text);
	gtk_widget_hide(info);
}

void GearJoint::update(GearJoint *p) {
	gtk_adjustment_configure(GTK_ADJUSTMENT(ar),p->ratio,0.01,100,0.01,0,0);
}
void GearJoint::update1() {
	GearJoint *p=TYPE(GearJoint*,get_selected_object());
	if(!p || point_ch)return;
	p->ratio=gtk_adjustment_get_value(GTK_ADJUSTMENT(ar));
	gtk_widget_queue_draw(drawable);
}
void GearJoint::vupdate() {
	update(this);
	Joint::update(this);
	Object::update(this);
}
string GearJoint::name() {
	return "GearJoint";
}
void RevoluteJoint::init(GtkWidget *table) {
	al=gtk_adjustment_new(0,-1000*M_PI,0,M_PI/12,0,0);
	pl=gtk_spin_button_new(GTK_ADJUSTMENT(al),0,4);
	tl=gtk_label_new("Lower angle");
	au=gtk_adjustment_new(0,0,0,1000*M_PI,M_PI/12,0);
	pu=gtk_spin_button_new(GTK_ADJUSTMENT(au),0,4);
	tu=gtk_label_new("Upper angle");
	as=gtk_adjustment_new(0,-1000,1000,1,1,0);
	ps=gtk_spin_button_new(GTK_ADJUSTMENT(as),0,4);
	ts=gtk_label_new("Speed");
	at=gtk_adjustment_new(1,-1000000000,1000000000,1,1,0);
	pt=gtk_spin_button_new(GTK_ADJUSTMENT(at),0,4);
	tt=gtk_label_new("Max torque");
	sl=gtk_check_button_new_with_label("Enable limit");
	sm=gtk_check_button_new_with_label("Enable motor");

	ins_widget2	(table,sl,cur_table_string++);
	ins_text	(table,tl,cur_table_string);
	ins_widget	(table,pl,cur_table_string++);
	ins_text	(table,tu,cur_table_string);
	ins_widget	(table,pu,cur_table_string++);
	ins_widget2	(table,sm,cur_table_string++);
	ins_text	(table,ts,cur_table_string);
	ins_widget	(table,ps,cur_table_string++);
	ins_text	(table,tt,cur_table_string);
	ins_widget	(table,pt,cur_table_string++);
	g_signal_connect(G_OBJECT(pl),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(pu),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(ps),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(pt),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(sl),"toggled",update1,0);
	g_signal_connect(G_OBJECT(sm),"toggled",update1,0);
}
void RevoluteJoint::draw(cairo_t *cr) {
	if(!shows[6])return;
	draw_lines(cr);
	draw_drag_joint_rect(cr,x,y,selected&&point_ch);
	cairo_set_source_rgb(cr,BLACK);
	cairo_rectangle(cr,drawx(x)-1,drawy(y)-1,3,3);
	cairo_fill(cr);
	if(limit) {
		cairo_set_source_rgb(cr,RED);
		cairo_arc(cr,drawx(x),drawy(y),15,lower,upper);
		cairo_move_to(cr,drawx(x)+cos(lower)*20,drawy(y)+sin(lower)*20);
		cairo_line_to(cr,drawx(x),drawy(y));
		cairo_line_to(cr,drawx(x)+cos(upper)*20,drawy(y)+sin(upper)*20);
		cairo_stroke(cr);
		cairo_set_source_rgb(cr,GRAY);
		cairo_move_to(cr,drawx(x),drawy(y));
		cairo_line_to(cr,drawx(x)+30,drawy(y));
		cairo_stroke(cr);
		//cairo
	}
}
bool RevoluteJoint::drag(float xp,float yp,int dr) {
	if(!shows[6])return 0;
	if(dr==0 && touch(x,y,xp,yp)) {
		hide_all();
		show();
		point_ch=1;
		return 1;
	} else if(dr==1&&point_ch==1) {
		x=to_grid(xp);
		y=to_grid(yp);
		return 1;
	} else if(dr==2)point_ch=0;
	else if(dr==3)show();
	return 0;
}
void RevoluteJoint::show() {
	PointJoint::show();
	gtk_widget_show(sl);
	gtk_widget_show(sm);
	gtk_widget_show(pl);
	gtk_widget_show(pu);
	gtk_widget_show(pt);
	gtk_widget_show(ps);
	gtk_widget_show(tl);
	gtk_widget_show(tu);
	gtk_widget_show(ts);
	gtk_widget_show(tt);
}
void RevoluteJoint::hide() {
	PointJoint::hide();
	gtk_widget_hide(sl);
	gtk_widget_hide(sm);
	gtk_widget_hide(pl);
	gtk_widget_hide(pu);
	gtk_widget_hide(pt);
	gtk_widget_hide(ps);
	gtk_widget_hide(tl);
	gtk_widget_hide(tu);
	gtk_widget_hide(ts);
	gtk_widget_hide(tt);
}
void RevoluteJoint::update(RevoluteJoint *p) {
	gtk_adjustment_configure(GTK_ADJUSTMENT(al),p->lower,-1000*M_PI,0,M_PI/12,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(au),p->upper,0,1000*M_PI,M_PI/12,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(as),p->speed,-1000000,1000000,1,1,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(at),p->torque,-1000000000,1000000000,1,1,0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sl),p->limit);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sm),p->motor);
}
void RevoluteJoint::update1() {
	RevoluteJoint *p=TYPE(RevoluteJoint*,get_selected_object());
	if(!p || point_ch)return;
	p->lower=gtk_adjustment_get_value(GTK_ADJUSTMENT(al));
	p->upper=gtk_adjustment_get_value(GTK_ADJUSTMENT(au));
	p->speed=gtk_adjustment_get_value(GTK_ADJUSTMENT(as));
	p->torque=gtk_adjustment_get_value(GTK_ADJUSTMENT(at));
	p->limit=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sl));
	p->motor=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sm));
	gtk_widget_queue_draw(drawable);
}
void RevoluteJoint::vupdate() {
	update(this);
	PointJoint::update(this);
	Joint::update(this);
	Object::update(this);
}
string RevoluteJoint::name() {
	return "RevoluteJoint";
}
void PrismaticJoint::init(GtkWidget *table) {
	aa=gtk_adjustment_new(0,0,M_PI*2,M_PI/12,0,0);
	pa=gtk_spin_button_new(GTK_ADJUSTMENT(aa),0,4);
	ta=gtk_label_new("Angle");
	al=gtk_adjustment_new(0,-1000,0,grid,0,0);
	pl=gtk_spin_button_new(GTK_ADJUSTMENT(al),0,4);
	tl=gtk_label_new("Lower");
	au=gtk_adjustment_new(0,0,1000,grid,0,0);
	pu=gtk_spin_button_new(GTK_ADJUSTMENT(au),0,4);
	tu=gtk_label_new("Upper");
	as=gtk_adjustment_new(0,-1000,1000,1,1,0);
	ps=gtk_spin_button_new(GTK_ADJUSTMENT(as),0,4);
	ts=gtk_label_new("Speed");
	at=gtk_adjustment_new(1,-1000000000,1000000000,1,1,0);
	pt=gtk_spin_button_new(GTK_ADJUSTMENT(at),0,4);
	tt=gtk_label_new("Max force");
	sl=gtk_check_button_new_with_label("Enable limit");
	sm=gtk_check_button_new_with_label("Enable motor");

	ins_widget	(table,pa,cur_table_string);
	ins_text	(table,ta,cur_table_string++);
	ins_widget2	(table,sl,cur_table_string++);
	ins_text	(table,tl,cur_table_string);
	ins_widget	(table,pl,cur_table_string++);
	ins_text	(table,tu,cur_table_string);
	ins_widget	(table,pu,cur_table_string++);
	ins_widget2	(table,sm,cur_table_string++);
	ins_text	(table,ts,cur_table_string);
	ins_widget	(table,ps,cur_table_string++);
	ins_text	(table,tt,cur_table_string);
	ins_widget	(table,pt,cur_table_string++);
	g_signal_connect(G_OBJECT(pl),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(pu),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(ps),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(pt),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(pa),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(sl),"toggled",update1,0);
	g_signal_connect(G_OBJECT(sm),"toggled",update1,0);
}
void PrismaticJoint::draw(cairo_t *cr) {
	if(!shows[6])return;
	draw_lines(cr);
	draw_drag_joint_rect(cr,x,y,selected&&point_ch==1);
	cairo_set_source_rgb(cr,BLACK);
	cairo_rectangle(cr,drawx(x)-1,drawy(y)-1,3,3);
	cairo_fill(cr);
	cairo_set_source_rgb(cr,RED);
	cairo_move_to(cr,drawx(x),drawy(y));
	cairo_line_to(cr,drawx(x)+cos(angle)*(limit?lower*zoom:-10000),drawy(y)+sin(angle)*(limit?lower*zoom:-10000));
	cairo_stroke(cr);
	cairo_set_source_rgb(cr,GREEN);
	cairo_move_to(cr,drawx(x),drawy(y));
	cairo_line_to(cr,drawx(x)+cos(angle)*(limit?upper*zoom:10000),drawy(y)+sin(angle)*(limit?upper*zoom:10000));
	cairo_stroke(cr);
	if(limit) {
		draw_drag_joint_rect(cr,x+cos(angle)*lower,y+sin(angle)*lower,selected&&point_ch==2);
		draw_drag_joint_rect(cr,x+cos(angle)*upper,y+sin(angle)*upper,selected&&point_ch==3);
	}
}
bool PrismaticJoint::drag(float xp,float yp,int dr) {
	if(!shows[6])return 0;
	if(dr==0) {
		if(touch(x,y,xp,yp))
			point_ch=1;
		else if(limit && touch(x+cos(angle)*lower,y+sin(angle)*lower,xp,yp))
			point_ch=2;
		else if(limit && touch(x+cos(angle)*upper,y+sin(angle)*upper,xp,yp))
			point_ch=3;
		else return 0;
		hide_all();
		show();
		return 1;
	} else if(dr==1) {
		if(point_ch==1) {
			x=to_grid(xp);
			y=to_grid(yp);
		} else if(point_ch==2) {
			angle=to_angle_grid(vec_angle1(xp-x,yp-y));
			lower=-to_grid(hypot(xp-x,yp-y));
		} else if(point_ch==3) {
			angle=to_angle_grid(vec_angle2(xp-x,yp-y));
			upper=to_grid(hypot(xp-x,yp-y));
		}
		return 1;
	} else if(dr==2)point_ch=0;
	else if(dr==3)show();
	return 0;
}
void PrismaticJoint::show() {
	PointJoint::show();
	gtk_widget_show(sl);
	gtk_widget_show(sm);
	gtk_widget_show(pl);
	gtk_widget_show(pu);
	gtk_widget_show(pt);
	gtk_widget_show(ps);
	gtk_widget_show(tl);
	gtk_widget_show(tu);
	gtk_widget_show(ts);
	gtk_widget_show(tt);
	gtk_widget_show(pa);
	gtk_widget_show(ta);
}
void PrismaticJoint::hide() {
	PointJoint::hide();
	gtk_widget_hide(sl);
	gtk_widget_hide(sm);
	gtk_widget_hide(pl);
	gtk_widget_hide(pu);
	gtk_widget_hide(pt);
	gtk_widget_hide(ps);
	gtk_widget_hide(tl);
	gtk_widget_hide(tu);
	gtk_widget_hide(ts);
	gtk_widget_hide(tt);
	gtk_widget_hide(pa);
	gtk_widget_hide(ta);
}
void PrismaticJoint::update(PrismaticJoint *p) {
	gtk_adjustment_configure(GTK_ADJUSTMENT(al),p->lower,-1000,0,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(au),p->upper,0,1000,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(as),p->speed,-1000000,1000000,1,1,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(at),p->force,-1000000000,1000000000,1,1,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(aa),p->angle,0,M_PI*2,M_PI/12,0,0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sl),p->limit);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sm),p->motor);
}
void PrismaticJoint::update1() {
	PrismaticJoint *p=TYPE(PrismaticJoint*,get_selected_object());
	if(!p || point_ch)return;
	p->lower=gtk_adjustment_get_value(GTK_ADJUSTMENT(al));
	p->upper=gtk_adjustment_get_value(GTK_ADJUSTMENT(au));
	p->speed=gtk_adjustment_get_value(GTK_ADJUSTMENT(as));
	p->force=gtk_adjustment_get_value(GTK_ADJUSTMENT(at));
	p->angle=gtk_adjustment_get_value(GTK_ADJUSTMENT(aa));
	p->limit=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sl));
	p->motor=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sm));
	gtk_widget_queue_draw(drawable);
}
void PrismaticJoint::vupdate() {
	update(this);
	PointJoint::update(this);
	Joint::update(this);
	Object::update(this);
}
string PrismaticJoint::name() {
	return "PrismaticJoint";
}
void DistanceJoint::init(GtkWidget *table) {
	ax1=gtk_adjustment_new(0,0,level.w,grid,grid,0);
	ay1=gtk_adjustment_new(0,0,level.h,grid,grid,0);
	ax2=gtk_adjustment_new(0,0,level.w,grid,grid,0);
	ay2=gtk_adjustment_new(0,0,level.h,grid,grid,0);
	as=gtk_adjustment_new(0,0,1000000000,1,1,0);
	ad=gtk_adjustment_new(0,0,2,0.1,0.1,0);
	amin=gtk_adjustment_new(0,-1000,1000,1,1,0);
	amax=gtk_adjustment_new(0,-1000,1000,1,1,0);
	px1=gtk_spin_button_new(GTK_ADJUSTMENT(ax1),0,4);
	py1=gtk_spin_button_new(GTK_ADJUSTMENT(ay1),0,4);
	px2=gtk_spin_button_new(GTK_ADJUSTMENT(ax2),0,4);
	py2=gtk_spin_button_new(GTK_ADJUSTMENT(ay2),0,4);
	ps=gtk_spin_button_new(GTK_ADJUSTMENT(as),0,4);
	pd=gtk_spin_button_new(GTK_ADJUSTMENT(ad),0,4);
	pmax=gtk_spin_button_new(GTK_ADJUSTMENT(amax),0,4);
	pmin=gtk_spin_button_new(GTK_ADJUSTMENT(amin),0,4);
	tx1=gtk_label_new("X1");
	ty1=gtk_label_new("Y1");
	tx2=gtk_label_new("X2");
	ty2=gtk_label_new("Y2");
	ts=gtk_label_new("Stiffness");
	td=gtk_label_new("Damping");
	tmax=gtk_label_new("Max");
	tmin=gtk_label_new("Min");
	ins_text(table,tx1,cur_table_string);
	ins_widget(table,px1,cur_table_string++);
	ins_text(table,ty1,cur_table_string);
	ins_widget(table,py1,cur_table_string++);
	ins_text(table,tx2,cur_table_string);
	ins_widget(table,px2,cur_table_string++);
	ins_text(table,ty2,cur_table_string);
	ins_widget(table,py2,cur_table_string++);
	ins_text(table,tmin,cur_table_string);
	ins_widget(table,pmin,cur_table_string++);
	ins_text(table,tmax,cur_table_string);
	ins_widget(table,pmax,cur_table_string++);
	ins_text(table,ts,cur_table_string);
	ins_widget(table,ps,cur_table_string++);
	ins_text(table,td,cur_table_string);
	ins_widget(table,pd,cur_table_string++);


	g_signal_connect(G_OBJECT(px1),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(py1),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(px2),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(py2),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(pmax),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(pmin),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(ps),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(pd),"value_changed",update1,0);
}
bool DistanceJoint::create(float xp,float yp,int dr) {
	if(dr==0) {
		x1=x2=to_grid(xp);
		y1=y2=to_grid(yp);
		id1=find_shape_1(x1,y1);
	} else if(dr==1) {
		x2=to_grid(xp);
		y2=to_grid(yp);
		id2=find_shape_1(x2,y2);
	} else if(dr==3) {
		if(x1==x2&&y1==y2)return 1;
	}
	return 0;
}
void DistanceJoint::draw(cairo_t *cr) {
	if(!shows[6])return;
	draw_drag_joint_rect(cr,x1,y1,selected&&point_ch==1);
	draw_drag_joint_rect(cr,x2,y2,selected&&point_ch==2);
	cairo_move_to(cr,drawx(x1),drawy(y1));
	cairo_line_to(cr,drawx(x2),drawy(y2));
	cairo_stroke(cr);
	cairo_set_source_rgb(cr,RED);
	cairo_move_to(cr,drawx(x2),drawy(y2));
	cairo_line_to(cr,drawx(x2+cos(vec_angle2(x2-x1,y2-y1))*max),
				  drawy(y2+sin(vec_angle2(x2-x1,y2-y1))*max));
	cairo_stroke(cr);
	cairo_set_source_rgb(cr,GREEN);
	cairo_move_to(cr,drawx(x2),drawy(y2));
	cairo_line_to(cr,drawx(x2+cos(vec_angle2(x2-x1,y2-y1))*min),
				  drawy(y2+sin(vec_angle2(x2-x1,y2-y1))*min));
	cairo_stroke(cr);
	draw_lines(cr);
}
bool DistanceJoint::drag(float xp,float yp,int dr) {
	if(!shows[6])return 0;
	if(dr==0) {
		if(touch(x1,y1,xp,yp))	point_ch=1;
		else	if(touch(x2,y2,xp,yp))	point_ch=2;
		else return 0;
		hide_all();
		show();
		return 1;
	} else if(dr==1&&point_ch) {
		if(point_ch==1) {
			x1=to_grid(xp);
			y1=to_grid(yp);
		} else if(point_ch==2) {
			x2=to_grid(xp);
			y2=to_grid(yp);
		}
		return 1;
	} else if(dr==2)point_ch=0;
	else if(dr==3)show();
	return 0;
}
void DistanceJoint::show() {
	Joint::show();
	gtk_widget_show(pmin);
	gtk_widget_show(pmax);
	gtk_widget_show(ps);
	gtk_widget_show(pd);
	gtk_widget_show(px1);
	gtk_widget_show(py1);
	gtk_widget_show(px2);
	gtk_widget_show(py2);
	gtk_widget_show(tx1);
	gtk_widget_show(ty1);
	gtk_widget_show(tx2);
	gtk_widget_show(ty2);
	gtk_widget_show(tmin);
	gtk_widget_show(tmax);
	gtk_widget_show(ts);
	gtk_widget_show(td);
}
void DistanceJoint::hide() {
	Joint::hide();
	gtk_widget_hide(pmin);
	gtk_widget_hide(pmax);
	gtk_widget_hide(ps);
	gtk_widget_hide(pd);
	gtk_widget_hide(px1);
	gtk_widget_hide(py1);
	gtk_widget_hide(px2);
	gtk_widget_hide(py2);
	gtk_widget_hide(tx1);
	gtk_widget_hide(ty1);
	gtk_widget_hide(tx2);
	gtk_widget_hide(ty2);
	gtk_widget_hide(tmin);
	gtk_widget_hide(tmax);
	gtk_widget_hide(ts);
	gtk_widget_hide(td);
}
void DistanceJoint::update(DistanceJoint *p) {
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax1),p->x1,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay1),p->y1,0,level.h,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax2),p->x2,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay2),p->y2,0,level.h,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(amax),p->max,-1000,1000,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(amin),p->min,-1000,1000,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(as),p->stiffness,0,1000000000000,100,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ad),p->damping,0,2,0.1,0,0);
}
void DistanceJoint::update1() {
	DistanceJoint *p=TYPE(DistanceJoint*,get_selected_object());
	if(!p || point_ch)return;
	p->x1=gtk_adjustment_get_value(GTK_ADJUSTMENT(ax1));
	p->y1=gtk_adjustment_get_value(GTK_ADJUSTMENT(ay1));
	p->x2=gtk_adjustment_get_value(GTK_ADJUSTMENT(ax2));
	p->y2=gtk_adjustment_get_value(GTK_ADJUSTMENT(ay2));
	p->max=gtk_adjustment_get_value(GTK_ADJUSTMENT(amax));
	p->min=gtk_adjustment_get_value(GTK_ADJUSTMENT(amin));
	p->stiffness=gtk_adjustment_get_value(GTK_ADJUSTMENT(as));
	p->damping=gtk_adjustment_get_value(GTK_ADJUSTMENT(ad));
	gtk_widget_queue_draw(drawable);
}
void DistanceJoint::vupdate() {
	update(this);
	Joint::update(this);
	Object::update(this);
}
string DistanceJoint::name() {
	return "DistanceJoint";
}
void DistanceJoint::draw_lines(cairo_t* cr) {
	cairo_set_source_rgb(cr,GRAY);
	if(id1.size()) {
		Object *o=get_id(id1);
		if(o) {
			cairo_move_to(cr,drawx(x1),drawy(y1));
			cairo_line_to(cr,drawx(o->mean_x()),drawy(o->mean_y()));
		}
	}
	if(id2.size()) {
		Object *o=get_id(id2);
		if(o) {
			cairo_move_to(cr,drawx(x2),drawy(y2));
			cairo_line_to(cr,drawx(o->mean_x()),drawy(o->mean_y()));
		}
	}
	cairo_stroke(cr);
}

void PulleyJoint::init(GtkWidget *table) {
	ax1=gtk_adjustment_new(0,0,level.w,grid,grid,0);
	ay1=gtk_adjustment_new(0,0,level.h,grid,grid,0);
	ax2=gtk_adjustment_new(0,0,level.w,grid,grid,0);
	ay2=gtk_adjustment_new(0,0,level.h,grid,grid,0);
	ax3=gtk_adjustment_new(0,0,level.w,grid,grid,0);
	ay3=gtk_adjustment_new(0,0,level.h,grid,grid,0);
	ax4=gtk_adjustment_new(0,0,level.w,grid,grid,0);
	ay4=gtk_adjustment_new(0,0,level.h,grid,grid,0);
	ar=gtk_adjustment_new(0,0.01,100,0.01,0.01,0);
	px1=gtk_spin_button_new(GTK_ADJUSTMENT(ax1),0,4);
	py1=gtk_spin_button_new(GTK_ADJUSTMENT(ay1),0,4);
	px2=gtk_spin_button_new(GTK_ADJUSTMENT(ax2),0,4);
	py2=gtk_spin_button_new(GTK_ADJUSTMENT(ay2),0,4);
	px3=gtk_spin_button_new(GTK_ADJUSTMENT(ax3),0,4);
	py3=gtk_spin_button_new(GTK_ADJUSTMENT(ay3),0,4);
	px4=gtk_spin_button_new(GTK_ADJUSTMENT(ax4),0,4);
	py4=gtk_spin_button_new(GTK_ADJUSTMENT(ay4),0,4);
	pr=gtk_spin_button_new(GTK_ADJUSTMENT(ar),0,4);
	tx1=gtk_label_new("X1");
	ty1=gtk_label_new("Y1");
	tx2=gtk_label_new("X2");
	ty2=gtk_label_new("Y2");
	tx3=gtk_label_new("GX1");
	ty3=gtk_label_new("GY1");
	tx4=gtk_label_new("GX2");
	ty4=gtk_label_new("GY2");
	tr=gtk_label_new("Ratio");
	info=gtk_label_new("length1 + ratio * length2 = const");
	ins_text(table,tx1,cur_table_string);
	ins_widget(table,px1,cur_table_string++);
	ins_text(table,ty1,cur_table_string);
	ins_widget(table,py1,cur_table_string++);
	ins_text(table,tx2,cur_table_string);
	ins_widget(table,px2,cur_table_string++);
	ins_text(table,ty2,cur_table_string);
	ins_widget(table,py2,cur_table_string++);
	ins_text(table,tx3,cur_table_string);
	ins_widget(table,px3,cur_table_string++);
	ins_text(table,ty3,cur_table_string);
	ins_widget(table,py3,cur_table_string++);
	ins_text(table,tx4,cur_table_string);
	ins_widget(table,px4,cur_table_string++);
	ins_text(table,ty4,cur_table_string);
	ins_widget(table,py4,cur_table_string++);
	ins_widget2(table,info,cur_table_string++);
	ins_text(table,tr,cur_table_string);
	ins_widget(table,pr,cur_table_string++);


	g_signal_connect(G_OBJECT(px1),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(py1),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(px2),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(py2),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(px3),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(py3),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(px4),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(py4),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(pr),"value_changed",update1,0);
}
bool PulleyJoint::create(float xp,float yp,int dr) {
	if(dr==0) {
		if(x1==-1000){
			x1=x3=to_grid(xp);
			y1=y3=to_grid(yp);
		}else{
			x2=x4=to_grid(xp);
			y2=y4=to_grid(yp);
		}
		return 1;
	} else if(dr==1) {
		if(x2==-1000){
			x1=to_grid(xp);
			y1=to_grid(yp);
			id1=find_shape_1(x1,y1);
		}else{
			x2=to_grid(xp);
			y2=to_grid(yp);
			id2=find_shape_1(x2,y2);
			DEBUG;
		}
	} else if(dr==2) {
		if(x2==-1000){
			x2=-1001;
			return 1;
		}
	}
	return 0;
}
void PulleyJoint::draw(cairo_t *cr) {
	if(!shows[6])return;
	draw_drag_joint_rect(cr,x1,y1,selected&&point_ch==1);
	draw_drag_joint_rect(cr,x3,y3,selected&&point_ch==3);
	if(x4>-1000){
		draw_drag_joint_rect(cr,x2,y2,selected&&point_ch==2);
		draw_drag_joint_rect(cr,x4,y4,selected&&point_ch==3);
	}
	cairo_set_source_rgb(cr,BLACK);
	cairo_move_to(cr,drawx(x1),drawy(y1));
	cairo_line_to(cr,drawx(x3),drawy(y3));
	if(x4>-1000){
		cairo_line_to(cr,drawx(x4),drawy(y4));
		cairo_line_to(cr,drawx(x2),drawy(y2));
	}
	cairo_stroke(cr);
	draw_lines(cr);
}
bool PulleyJoint::drag(float xp,float yp,int dr) {
	if(!shows[6])return 0;
	if(dr==0) {
		if(touch(x1,y1,xp,yp))	point_ch=1;
		else	if(touch(x2,y2,xp,yp))	point_ch=2;
		else	if(touch(x3,y3,xp,yp))	point_ch=3;
		else	if(touch(x4,y4,xp,yp))	point_ch=4;
		else return 0;
		hide_all();
		show();
		return 1;
	} else if(dr==1&&point_ch) {
		if(point_ch==1) {
			x1=to_grid(xp);
			y1=to_grid(yp);
		} else if(point_ch==2) {
			x2=to_grid(xp);
			y2=to_grid(yp);
		}else if(point_ch==3) {
			x3=to_grid(xp);
			y3=to_grid(yp);
		} else if(point_ch==4) {
			x4=to_grid(xp);
			y4=to_grid(yp);
		}
		return 1;
	} else if(dr==2)point_ch=0;
	else if(dr==3)show();
	return 0;
}
void PulleyJoint::show() {
	Joint::show();
	gtk_widget_show(px1);
	gtk_widget_show(py1);
	gtk_widget_show(px2);
	gtk_widget_show(py2);
	gtk_widget_show(px3);
	gtk_widget_show(py3);
	gtk_widget_show(px4);
	gtk_widget_show(py4);
	gtk_widget_show(tx1);
	gtk_widget_show(ty1);
	gtk_widget_show(tx2);
	gtk_widget_show(ty2);
	gtk_widget_show(tx3);
	gtk_widget_show(ty3);
	gtk_widget_show(tx4);
	gtk_widget_show(ty4);
	gtk_widget_show(tr);
	gtk_widget_show(pr);
	gtk_widget_show(info);
}
void PulleyJoint::hide() {
	Joint::hide();
	gtk_widget_hide(px1);
	gtk_widget_hide(py1);
	gtk_widget_hide(px2);
	gtk_widget_hide(py2);
	gtk_widget_hide(px3);
	gtk_widget_hide(py3);
	gtk_widget_hide(px4);
	gtk_widget_hide(py4);
	gtk_widget_hide(tx1);
	gtk_widget_hide(ty1);
	gtk_widget_hide(tx2);
	gtk_widget_hide(ty2);
	gtk_widget_hide(tx3);
	gtk_widget_hide(ty3);
	gtk_widget_hide(tx4);
	gtk_widget_hide(ty4);
	gtk_widget_hide(tr);
	gtk_widget_hide(pr);
	gtk_widget_hide(info);
}
void PulleyJoint::update(PulleyJoint *p) {
	block=1;
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax1),p->x1,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay1),p->y1,0,level.h,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax2),p->x2,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay2),p->y2,0,level.h,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax3),p->x3,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay3),p->y3,0,level.h,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax4),p->x4,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay4),p->y4,0,level.h,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ar),p->ratio,0.01,100,0.01,0,0);
	block=0;
}
void PulleyJoint::update1() {
	if(block)return;
	PulleyJoint *p=TYPE(PulleyJoint*,get_selected_object());
	if(!p || point_ch)return;
	p->x1=gtk_adjustment_get_value(GTK_ADJUSTMENT(ax1));
	p->y1=gtk_adjustment_get_value(GTK_ADJUSTMENT(ay1));
	p->x2=gtk_adjustment_get_value(GTK_ADJUSTMENT(ax2));
	p->y2=gtk_adjustment_get_value(GTK_ADJUSTMENT(ay2));
	p->x3=gtk_adjustment_get_value(GTK_ADJUSTMENT(ax3));
	p->y3=gtk_adjustment_get_value(GTK_ADJUSTMENT(ay3));
	p->x4=gtk_adjustment_get_value(GTK_ADJUSTMENT(ax4));
	p->ratio=gtk_adjustment_get_value(GTK_ADJUSTMENT(ar));
	gtk_widget_queue_draw(drawable);
}
void PulleyJoint::vupdate() {
	update(this);
	Joint::update(this);
	Object::update(this);
}
string PulleyJoint::name() {
	return "PulleyJoint";
}
void PulleyJoint::draw_lines(cairo_t* cr) {
	cairo_set_source_rgb(cr,GRAY);
	if(id1.size()) {
		Object *o=get_id(id1);
		if(o) {
			cairo_move_to(cr,drawx(x1),drawy(y1));
			cairo_line_to(cr,drawx(o->mean_x()),drawy(o->mean_y()));
		}
	}
	if(id2.size()) {
		Object *o=get_id(id2);
		if(o) {
			cairo_move_to(cr,drawx(x2),drawy(y2));
			cairo_line_to(cr,drawx(o->mean_x()),drawy(o->mean_y()));
		}
	}
	cairo_stroke(cr);
}
