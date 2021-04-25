#include "body.hpp"
#include "level.hpp"
#include "utils.hpp"
#include "events.hpp"
#include "editor.hpp"
#include "tree_view.hpp"
#include <cmath>
using namespace std;
void Body::show() {
	Object::show();
	gtk_widget_show(combo);
	gtk_widget_show(px);
	gtk_widget_show(py);
	gtk_widget_show(ps);
	gtk_widget_show(tx);
	gtk_widget_show(ty);
	gtk_widget_show(ts);
	gtk_widget_show(cr);
	gtk_widget_show(cb);
	gtk_widget_show(tt);
	gtk_widget_show(en_script);
}
void Body::hide() {
	Object::hide();
	gtk_widget_hide(combo);
	gtk_widget_hide(px);
	gtk_widget_hide(py);
	gtk_widget_hide(ps);
	gtk_widget_hide(tx);
	gtk_widget_hide(ty);
	gtk_widget_hide(ts);
	gtk_widget_hide(cr);
	gtk_widget_hide(cb);
	gtk_widget_hide(tt);
	gtk_widget_hide(en_script);
}
Body::~Body() {
	for(int q=0; q<shapes.size(); q++) {
		delete shapes[q];
	}
}
bool Body::drag(float xp,float yp,int dr) {
	if(dr==3) {
		hide_all();
		vupdate();
	}
	return 0;
}
string Body::name() {
	return "Body";
}
void Body::vupdate() {
	show();
	update(this);
	Object::update(this);
}

void Body::init(GtkWidget *table) {
	combo=gtk_combo_box_new_text();
	ax=gtk_adjustment_new(0,0,level.w,grid,grid,0);
	ay=gtk_adjustment_new(0,0,level.h,grid,grid,0);
	as=gtk_adjustment_new(1.0,0,1000,0.1,0.1,0);
	px=gtk_spin_button_new(GTK_ADJUSTMENT(ax),0,4);
	py=gtk_spin_button_new(GTK_ADJUSTMENT(ay),0,4);
	ps=gtk_spin_button_new(GTK_ADJUSTMENT(as),0,4);
	tx=gtk_label_new("X");
	ty=gtk_label_new("Y");
	ts=gtk_label_new("Gravity\nscale");
	cb=gtk_check_button_new_with_label("Bullet");
	cr=gtk_check_button_new_with_label("Fixed rotate");
	tt=gtk_label_new("Script");
	en_script=gtk_entry_new();

	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"Static");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"Dynamic");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"Kinematic");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo),0);

	ins_text	(table,tx,cur_table_string);
	ins_widget	(table,px,cur_table_string++);
	ins_text	(table,ty,cur_table_string);
	ins_widget	(table,py,cur_table_string++);
	ins_widget2(table,combo,cur_table_string++);
	ins_text	(table,ts,cur_table_string);
	ins_widget	(table,ps,cur_table_string++);
	ins_widget2(table,cb,cur_table_string++);
	ins_widget2(table,cr,cur_table_string++);
	ins_text(table,tt,cur_table_string);
	ins_widget(table,en_script,cur_table_string++);

	g_signal_connect(G_OBJECT(px),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(py),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(ps),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(cr),"toggled",update1,0);
	g_signal_connect(G_OBJECT(cb),"toggled",update1,0);
	g_signal_connect(G_OBJECT(combo),"changed",update1,0);
	g_signal_connect(G_OBJECT(en_script),"notify::text",update1,0);
}
void Body::update(Body *l) {
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo),l->type);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax),l->mean_x(),0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay),l->mean_y(),0,level.h,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(as),l->gravity_scale,0,1000,0.1,0.1,0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb),l->bullet);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cr),l->fixed_rot);
	gtk_entry_set_text(GTK_ENTRY(en_script),l->script.c_str());
}
void Body::update1() {
	Body *p=TYPE(Body*,get_selected_object());
	if(!p || point_ch)return;
	p->type=gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
	p->setpos(gtk_adjustment_get_value(GTK_ADJUSTMENT(ax)),
			  gtk_adjustment_get_value(GTK_ADJUSTMENT(ay)));
	p->bullet=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cb));
	p->fixed_rot=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cr));
	p->gravity_scale=gtk_adjustment_get_value(GTK_ADJUSTMENT(as));
	p->script=gtk_entry_get_text(GTK_ENTRY(en_script));
	gtk_widget_queue_draw(drawable);
}
void Body::save(XMLNode &parent,bool p) {
	float xb=mean_x(),yb=mean_y();
	XMLNode bd=parent.addChild("body");
	if(p) {
		bd.addAttribute("x",xb);
		bd.addAttribute("y",yb);
		bd.addAttribute("id",id);
	}
	{
		XMLNode phs=bd.addChild("physic");
		phs.addAttribute("bullet",bullet);
		phs.addAttribute("fixed_rotation",fixed_rot);
		phs.addAttribute("gravity_scale",gravity_scale);
	}
	{
		//type
		string type;
		switch(this->type) {
		case TSTAT:
			type="static";
			break;
		case TDYN:
			type="dynamic";
			break;
		case TKNM:
			type="kinematic";
			break;
		}
		bd.addAttribute("type",type);
	}
	bd.addAttribute("script",script.c_str());
	bd.addAttribute("shapes",shapes.size());
	for(int w=0; w<shapes.size(); w++) { //shape
		Physic *shape=TYPE(Physic*,shapes[w]);
		XMLNode sh=bd.addChild("shape");
		sh.addAttribute("id",shape->id);
		sh.addAttribute("type",shape->name());
		{
			//pos
			string type;
			switch(shape->layer) {
			case TBGR:
				type="background";
				break;
			case TPHS1:
				type="b_physic";
				break;
			case TPHS2:
				type="physic";
				break;
			case TPHS3:
				type="f_physic";
				break;
			case TFGR:
				type="foreground";
				break;
			case TNONE:
				type="none";
				break;
			}
			sh.addAttribute("pos",type.c_str());
			sh.addAttribute("texture",shape->texture.c_str());
			sh.addAttribute("expand",shape->ex);
		}
		{
			//position
			XMLNode pos=sh.addChild("position");
			if(shape->name()=="Square" ||
					shape->name()=="Circle") {
				BiSymmetrical *c=TYPE(BiSymmetrical*,shape);
				pos.addAttribute("x",c->x-xb);
				pos.addAttribute("y",c->y-yb);
				pos.addAttribute("r",c->r);
			} else if(shape->name()=="Rect" ||
					  shape->name()=="Line") {
				BiPoints *rect=TYPE(BiPoints*,shape);
				pos.addAttribute("x1",rect->x1-xb);
				pos.addAttribute("y1",rect->y1-yb);
				pos.addAttribute("x2",rect->x2-xb);
				pos.addAttribute("y2",rect->y2-yb);
			} else if(shape->name()=="Polygon") {
				Polygon *poly=TYPE(Polygon*,shape);
				pos.addAttribute("point_count",poly->size());
				for(int e=0; e<poly->size(); e++) { //points
					XMLNode point=pos.addChild("point");
					point.addAttribute("x",poly->x[e]-xb);
					point.addAttribute("y",poly->y[e]-yb);
				}
			}
		}
		{
			//physic
			XMLNode phs=sh.addChild("physic");
			phs.addAttribute("density",    shape->density);
			phs.addAttribute("friction",   shape->friction);
			phs.addAttribute("restitution",shape->restitution);
		}
		{
			XMLNode collision=sh.addChild("collision");
			collision.addAttribute("category",shape->category);
			collision.addAttribute("mask",shape->mask);
		}
	}
}
void Body::load(XMLNode &node,bool p) {
	float x=0,y=0;
	if(p) {
		XMLNode phs=node.getChildNode("physic");
		id=node.getAttribute("id");
		bullet=stoi(phs.getAttribute("bullet"));
		fixed_rot=stoi(phs.getAttribute("fixed_rotation"));
		gravity_scale=stof(phs.getAttribute("gravity_scale"));
		x=to_fl(node.getAttribute("x"));
		y=to_fl(node.getAttribute("y"));
	}
	{
		string str=node.getAttribute("type");
		if(str=="static")			type=TSTAT;
		else if(str=="dynamic")		type=TDYN;
		else if(str=="kinematic")	type=TKNM;
	}
	script=node.getAttribute("script");
	shapes.clear();
	int shapes_count=stoi(node.getAttribute("shapes"));
	for(int w=0; w<shapes_count; w++) {
		XMLNode sh=node.getChildNode("shape",w);
		Physic *shp;
		{
			//position
			XMLNode pos=sh.getChildNode("position");
			string str=sh.getAttribute("type");
			if(str=="Square") {
				shp=new Square(to_fl(pos.getAttribute("x"))+x,
							   to_fl(pos.getAttribute("y"))+y,
							   to_fl(pos.getAttribute("r")));
			} else if(str=="Circle") {
				shp=new Circle(to_fl(pos.getAttribute("x"))+x,
							   to_fl(pos.getAttribute("y"))+y,
							   to_fl(pos.getAttribute("r")));
			} else if(str=="Rect") {
				shp=new Rect(to_fl(pos.getAttribute("x1"))+x,
							 to_fl(pos.getAttribute("y1"))+y,
							 to_fl(pos.getAttribute("x2"))+x,
							 to_fl(pos.getAttribute("y2"))+y);
			} else if(str=="Line") {
				shp=new Line(to_fl(pos.getAttribute("x1"))+x,
							 to_fl(pos.getAttribute("y1"))+y,
							 to_fl(pos.getAttribute("x2"))+x,
							 to_fl(pos.getAttribute("y2"))+y);
			} else if(str=="Polygon") {
				int c=stoi(pos.getAttribute("point_count"));
				vector<float>xp(c);
				vector<float>yp(c);
				for(int e=0; e<c; e++) {
					XMLNode point=pos.getChildNode("point",e);
					xp[e]=to_fl(point.getAttribute("x"))+x;
					yp[e]=to_fl(point.getAttribute("y"))+y;
				}
				shp=new Polygon(xp,yp);
			}
		}
		shp->id     =sh.getAttribute("id");
		shp->texture=sh.getAttribute("texture");
		shp->ex		=stoi(sh.getAttribute("expand"));
		{
			string str=sh.getAttribute("pos");
			if(str=="background")		shp->layer=TBGR;
			else if(str=="b_physic")	shp->layer=TPHS1;
			else if(str=="physic")		shp->layer=TPHS2;
			else if(str=="f_physic")	shp->layer=TPHS3;
			else if(str=="foreground")	shp->layer=TFGR;
			else if(str=="none")		shp->layer=TNONE;
		}
		{
			//physic
			XMLNode phs=sh.getChildNode("physic");
			shp->density=    to_fl(phs.getAttribute("density"));
			shp->friction=   to_fl(phs.getAttribute("friction"));
			shp->restitution=to_fl(phs.getAttribute("restitution"));
		}
		{
			XMLNode collision=sh.getChildNode("collision");
			shp->category=stoi(collision.getAttribute("category"));
			shp->mask=stoi(collision.getAttribute("mask"));
		}
		shapes.push_back(shp);
	}
}
bool Body::template_open(string str) {
	XMLNode bd=XMLNode::openFileHelper(str.c_str(),"body");
	load(bd,0);
	return 0;
}
bool Body::template_save(string str) {
	XMLNode Main=XMLNode::createXMLTopNode("xml",1);
	Main.addAttribute("version","1.0");
	Main.addAttribute("encoding","UTF-8");
	save(Main,0);
	if(Main.writeToFile(str.c_str(),"UTF-8",1))return 1;
	return 0;
}
void template_save() {
	string str=get_save_path("templates");
	if(str.size()) {
		if([&]()->bool {
		Body *b=get_selected_body();
			if(!b)return 1;
			if(b->template_save(str))return 1;
			return 0;
		}())
		set_status(CONTEXT_FILE_STATUS, "Save error");
		else
			set_status(CONTEXT_FILE_STATUS, "Saved successfully");
	}
}
float Body::mean_x() {
	float x=0;
	for(int q=0; q<shapes.size(); q++)
		x+=shapes[q]->mean_x();
	x/=shapes.size();
	return x;
}
float Body::mean_y() {
	float y=0;
	for(int q=0; q<shapes.size(); q++)
		y+=shapes[q]->mean_y();
	y/=shapes.size();
	return y;
}
void Body::setpos(float x, float y) {
	float xb=mean_x();
	float yb=mean_y();
	for(Physic *sh : shapes) {
		vector<float*>points_x=sh->get_xpoints();
		vector<float*>points_y=sh->get_ypoints();
		for(int q=0; q<points_x.size(); q++) {
			*points_x[q]+=(x-xb);
			*points_y[q]+=(y-yb);
		}
	}
}
void Body::resolve_names() {
	id="";
	for(int q=0; q<shapes.size(); q++) {
		shapes[q]->id="";
	}
	for(int q=0; q<shapes.size(); q++) {
		shapes[q]->id=get_free_name(shapes[q]->name());
	}
	id=get_free_name(name());
}
