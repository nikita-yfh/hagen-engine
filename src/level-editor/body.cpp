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
	gtk_widget_show(tad);
	gtk_widget_show(tld);
	gtk_widget_show(pad);
	gtk_widget_show(pld);
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
	gtk_widget_hide(tad);
	gtk_widget_hide(tld);
	gtk_widget_hide(pad);
	gtk_widget_hide(pld);
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
	aad=gtk_adjustment_new(0,0,1000000,0.1,0.1,0);
	ald=gtk_adjustment_new(0,0,1000000,0.1,0.1,0);
	px=gtk_spin_button_new(GTK_ADJUSTMENT(ax),0,4);
	py=gtk_spin_button_new(GTK_ADJUSTMENT(ay),0,4);
	ps=gtk_spin_button_new(GTK_ADJUSTMENT(as),0,4);
	pad=gtk_spin_button_new(GTK_ADJUSTMENT(aad),0,4);
	pld=gtk_spin_button_new(GTK_ADJUSTMENT(ald),0,4);
	tx=gtk_label_new("X");
	ty=gtk_label_new("Y");
	tad=gtk_label_new("Angular\ndamping");
	tld=gtk_label_new("Linear\ndamping");
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
	ins_text	(table,tad,cur_table_string);
	ins_widget	(table,pad,cur_table_string++);
	ins_text	(table,tld,cur_table_string);
	ins_widget	(table,pld,cur_table_string++);

	g_signal_connect(G_OBJECT(px),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(py),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(pld),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(pad),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(ps),"value_changed",update1,0);
	g_signal_connect(G_OBJECT(cr),"toggled",update1,0);
	g_signal_connect(G_OBJECT(cb),"toggled",update1,0);
	g_signal_connect(G_OBJECT(combo),"changed",update1,0);
	g_signal_connect(G_OBJECT(en_script),"notify::text",update1,0);
}
void Body::update(Body *l) {
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo),l->type);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ax),l->mean().x,0,level.w,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ay),l->mean().y,0,level.h,grid,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(aad),l->angular_damping,0,1000000,0.1,0,0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(ald),l->linear_damping, 0,1000000,0.1,0,0);
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
	p->angular_damping=gtk_adjustment_get_value(GTK_ADJUSTMENT(aad));
	p->linear_damping=gtk_adjustment_get_value(GTK_ADJUSTMENT(ald));
	gtk_widget_queue_draw(drawable);
}
void Body::save(XMLNode &parent,bool p) {
	b2Vec2 d=mean();
	XMLNode bd=parent.addChild("body");
	if(p) {
		bd.addAttribute("x",d.x);
		bd.addAttribute("y",d.y);
		bd.addAttribute("id",id);
	}
	{
		XMLNode phs=bd.addChild("physic");
		phs.addAttribute("bullet",bullet);
		phs.addAttribute("fixed_rotation",fixed_rot);
		phs.addAttribute("gravity_scale",gravity_scale);
		phs.addAttribute("angular_damping",angular_damping);
		phs.addAttribute("linear_damping",linear_damping);
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
			sh.addAttribute("layer",(int)shape->layer);
			sh.addAttribute("sensor",(int)shape->sn);
			sh.addAttribute("texture",shape->texture.c_str());
			sh.addAttribute("expand",shape->ex);
		}
		{
			//position
			XMLNode pos=sh.addChild("position");
			if(shape->name()=="Square" ||
					shape->name()=="Circle") {
				BiSymmetrical *c=TYPE(BiSymmetrical*,shape);
				pos.addAttribute("x",c->pos.x-d.x);
				pos.addAttribute("y",c->pos.y-d.y);
				pos.addAttribute("r",c->r);
			} else if(shape->name()=="Rect" ||
					  shape->name()=="Line") {
				BiPoints *rect=TYPE(BiPoints*,shape);
				pos.addAttribute("x1",rect->p1.x-d.x);
				pos.addAttribute("y1",rect->p1.y-d.y);
				pos.addAttribute("x2",rect->p2.x-d.x);
				pos.addAttribute("y2",rect->p2.y-d.y);
			} else if(shape->name()=="Polygon") {
				Polygon *poly=TYPE(Polygon*,shape);
				pos.addAttribute("point_count",poly->size());
				for(int e=0; e<poly->size(); e++) { //points
					XMLNode point=pos.addChild("point");
					point.addAttribute("x",poly->points[e].x-d.x);
					point.addAttribute("y",poly->points[e].y-d.y);
				}
			} else if(shape->name()=="Cover") {
				Cover *poly=TYPE(Cover*,shape);
				pos.addAttribute("point_count",poly->size());
				pos.addAttribute("width",poly->w);
				for(int e=0; e<poly->size(); e++) { //points
					XMLNode point=pos.addChild("point");
					point.addAttribute("x",poly->points[e].x-d.x);
					point.addAttribute("y",poly->points[e].y-d.y);
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
		bullet=phs.getAttributei("bullet");
		fixed_rot=phs.getAttributei("fixed_rotation");
		gravity_scale=phs.getAttributef("gravity_scale");
		x=node.getAttributef("x");
		y=node.getAttributef("y");
		angular_damping=phs.getAttributef("angular_damping");
		linear_damping=phs.getAttributef("linear_damping");
	}
	{
		string str=node.getAttribute("type");
		if(str=="static")			type=TSTAT;
		else if(str=="dynamic")		type=TDYN;
		else if(str=="kinematic")	type=TKNM;
	}
	script=node.getAttribute("script");
	shapes.clear();
	int shapes_count=node.getAttributei("shapes");
	for(int w=0; w<shapes_count; w++) {
		XMLNode sh=node.getChildNode("shape",w);
		Physic *shp;
		{
			//position
			XMLNode pos=sh.getChildNode("position");
			string str=sh.getAttribute("type");
			if(str=="Square") {
				shp=new Square({pos.getAttributef("x")+x,
								pos.getAttributef("y")+y},
							   pos.getAttributef("r"));
			} else if(str=="Circle") {
				shp=new Circle({pos.getAttributef("x")+x,
								pos.getAttributef("y")+y},
							   pos.getAttributef("r"));
			} else if(str=="Rect") {
				shp=new Rect({pos.getAttributef("x1")+x,
				pos.getAttributef("y1")+y}, {
					pos.getAttributef("x2")+x,
					pos.getAttributef("y2")+y
				});
			} else if(str=="Line") {
				shp=new Line({pos.getAttributef("x1")+x,
				pos.getAttributef("y1")+y}, {
					pos.getAttributef("x2")+x,
					pos.getAttributef("y2")+y
				});
			} else if(str=="Polygon") {
				int c=pos.getAttributei("point_count");
				vector<b2Vec2>p(c);
				for(int e=0; e<c; e++) {
					XMLNode point=pos.getChildNode("point",e);
					p[e].x=point.getAttributef("x")+x;
					p[e].y=point.getAttributef("y")+y;
				}
				shp=new Polygon(p);
			} else if(str=="Cover") {
				int c=pos.getAttributei("point_count");
				float w=pos.getAttributef("width");
				vector<b2Vec2>p(c);
				for(int e=0; e<c; e++) {
					XMLNode point=pos.getChildNode("point",e);
					p[e].x=point.getAttributef("x")+x;
					p[e].y=point.getAttributef("y")+y;
				}
				shp=new Cover(p,w);
			}
		}
		shp->id     =sh.getAttribute("id");
		shp->texture=sh.getAttribute("texture");
		shp->ex		=sh.getAttributei("expand");
		shp->layer  =sh.getAttributei("layer");
		shp->sn     =sh.getAttributei("sensor");
		{
			//physic
			XMLNode phs=sh.getChildNode("physic");
			shp->density=    phs.getAttributef("density");
			shp->friction=   phs.getAttributef("friction");
			shp->restitution=phs.getAttributef("restitution");
		}
		{
			XMLNode collision=sh.getChildNode("collision");
			shp->category=collision.getAttributei("category");
			shp->mask=collision.getAttributei("mask");
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
	if(Main.writeToFile(str.c_str()))return 1;
	return 0;
}
void template_save() {
	string str=get_save_path("bodies");
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
b2Vec2 Body::mean() {
	b2Vec2 p(0,0);
	for(int q=0; q<shapes.size(); q++)
		p+=shapes[q]->mean();
	p.x/=shapes.size();
	p.y/=shapes.size();
	return p;
}
void Body::setpos(float x, float y) {
	b2Vec2 vec=mean();
	for(Physic *sh : shapes) {
		vector<b2Vec2*>points=sh->get_points();
		for(int q=0; q<points.size(); q++)
			*points[q]+=(b2Vec2(x,y)-vec);
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
