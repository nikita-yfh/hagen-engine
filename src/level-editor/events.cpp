#include "events.hpp"
#include "level.hpp"
#include "editor.hpp"
#include "utils.hpp"
#include "tree_view.hpp"
#include <fstream>
#include <assert.h>
#include <cmath>
#include <bitset>
using namespace std;
int draw_w=300,draw_h=300;
uint8_t mouse_button=0;
bool block=0;
uint8_t key_state;
void unselect_all_shapes() {
	vector<Object*> all=get_all();
	for(Object *shape : all) {
		shape->selected=0;
	}
}
uint8_t create_status=0;
bool drag_shapes(int x,int y,int mouse) {
	vector<Object*> all=get_all();
	for(Object *shape : all) {
		bool ok=0;
		if(mouse!=1 || shape->selected==1)ok=shape->drag(dragx(x),dragy(y),mouse);
		if(ok) {
			if(mouse==0) {
				unselect_all_shapes();
				shape->selected=1;
				create_status=0;
				select_tree_view(shape->id);
				block=1;
				shape->vupdate();
				block=0;
			} else if(mouse!=2) {
				block=1;
				shape->vupdate();
				block=0;
			}
		}
	}
	return 0;
}
bool create_shapes(int x,int y,int mouse) {
	if(mouse==0&&create_status==0) {
		Body *cr=get_selected_body();
		if(cr==0 && tool_ch<7) {
			set_status(CONTEXT_CREATE_ERROR,"You don't select any body");
		} else {
			remove_status(CONTEXT_CREATE_ERROR);
			Object *obj;
			switch(tool_ch) {
			case 2:
				obj = new Polygon;
				break;
			case 3:
				obj = new Rect;
				break;
			case 4:
				obj = new Square;
				break;
			case 5:
				obj = new Circle;
				break;
			case 6:
				obj = new Line;
				break;
			case 7:
				obj = new RevoluteJoint;
				break;
			case 8:
				obj=new WeldJoint;
				break;
			case 9:
				obj=new PrismaticJoint;
				break;
			case 10:
				obj=new PulleyJoint;
				break;
			case 11:
				obj=new GearJoint;
				break;
			case 12:
				obj=new DistanceJoint;
				break;
			case 13:
				obj=new Entity;
				break;
			}
			if(!obj)return 0;
			obj->id=get_free_name(obj->name());
			unselect_all_shapes();
			obj->selected=1;
			if(obj->create(dragx(x),dragy(y),0)) {
				create_status=2;
			} else create_status=1;
			if(tool_ch<7)
				cr->shapes.push_back((Physic*)obj);
			else if(tool_ch<13)
				level.joints.push_back((Joint*)obj);
			else
				level.entities.push_back((Entity*)obj);
			fill_shapes();
			select_tree_view(obj->id);
			block=1;
			obj->vupdate();
			block=0;
		}
	} else if(mouse==0 && create_status==2) {
		Object *obj=get_selected_object();
		if(!obj)return 0;

		block=1;
		obj->vupdate();
		block=0;
		if(!obj->create(dragx(x),dragy(y),0)) {
			create_status=0;
			if(obj->create(dragx(x),dragy(y),3))
				rem_but();
		}
	} else if(mouse==1 && create_status) {
		Object *obj=get_selected_object();
		if(!obj)return 0;
		obj->create(dragx(x),dragy(y),1);
		block=1;
		obj->vupdate();
		block=0;
	} else if(mouse==2 && create_status) {
		Object *obj=get_selected_object();
		if(!obj)return 0;
		block=1;
		obj->vupdate();
		block=0;
		if(!obj->create(dragx(x),dragy(y),2)) {
			create_status=0;
			if(obj->create(dragx(x),dragy(y),3)) {
				hide_all();
				rem_but();
			}
		}
	} else if(mouse==3) {
		if(create_status) {
			Object *obj=get_selected_object();
			create_status=0;
			if(obj->create(dragx(x),dragy(y),3)) {
				hide_all();
				rem_but();
				return 0;
			}
		} else
			change_tool(1);

	}
	return 0;
}

void rulers_update() {
	gtk_ruler_set_range(GTK_RULER(ruler_h),-cx/zoom,-(cx-draw_w)/zoom,0,80);
	gtk_ruler_set_range(GTK_RULER(ruler_v),-cy/zoom,-(cy-draw_h)/zoom,0,80);
}
void zoom_plus() {
	zoom=max(1.0f,min(10000.0f,sqr(sqrt(zoom)+1)));
	zoom_upd();
}
void zoom_minus() {
	zoom=max(1.0f,min(10000.0f,sqr(sqrt(zoom)-1)));
	zoom_upd();
}
void zoom_normal() {
	zoom=100.0f;
	zoom_upd();
}
void zoom_upd() {
	gtk_adjustment_set_value(GTK_ADJUSTMENT(zoom_a),sqrt(zoom)*5);
	gtk_label_set_text(GTK_LABEL(zoom_text),ssprintf("%d px/m",(int)zoom).c_str());
}
void zoom1_upd() {
	zoom=sqr(gtk_adjustment_get_value(GTK_ADJUSTMENT(zoom_a))/5);
	if(lock)scroll1_upd();
	rulers_update();
	gtk_widget_queue_draw(drawable);
	gtk_label_set_text(GTK_LABEL(zoom_text),ssprintf("%d px/m",(int)zoom).c_str());
}
int scroll(GtkWidget*, GdkEvent *event, gpointer data) {
	if(block)return 0;
	scroll1_upd();
	rulers_update();
	gtk_widget_queue_clear(drawable);
	return 0;
}
int motion(GtkWidget*, GdkEventMotion *event, gpointer data) {
	if(tool_ch==1)
		drag_shapes(event->x,event->y,1);
	else if(tool_ch>1)
		create_shapes(event->x,event->y,1);
	if(create_status==3) {
		get_selected_body()->setpos(to_grid(dragx(event->x)),
									to_grid(dragy(event->y)));
	}
	gtk_widget_queue_draw(drawable);
	if(mouse_button!=2 && (tool_ch!=0||mouse_button!=1))return 0;
	cx=xd+(event->x-xp);
	cy=yd+(event->y-yp);
	scroll_upd();
	if(lock)scroll1_upd();
	rulers_update();
	return 0;
}
int mousebutton_release(GtkWidget *area,  GdkEventButton  *event, gpointer data) {
	if(tool_ch==1)
		drag_shapes(event->x,event->y,2);
	else if(tool_ch>1)
		create_shapes(event->x,event->y,2);
	gtk_widget_queue_draw(drawable);
	mouse_button=0;
	return 0;
}
int mousebutton_press(GtkWidget *area,  GdkEventButton  *event, gpointer data) {
	mouse_button=event->button;
	rulers_update();
	if(mouse_button==1 && create_status==3) {
		create_status=0;
		return 0;
	}
	if(mouse_button==2 || tool_ch==0&&mouse_button==1) {
		xp=event->x;
		yp=event->y;
		xd=cx;
		yd=cy;
	} else if(mouse_button==1 || mouse_button==3) {
		if(tool_ch==1)
			drag_shapes(event->x,event->y,0);
		else if(tool_ch>1)
			create_shapes(event->x,event->y,(mouse_button==1)?0:3);
		gtk_widget_queue_draw(drawable);
	}
	return 0;
}
void getsize(GtkWidget*, GtkAllocation *allocation, void*) {
	draw_w=allocation->width;
	draw_h=allocation->height;
	if(lock)scroll1_upd();
	scroll_upd();
}
float conv1(float v){
	return v;
}
float conv2(float v){
	return std::max(0.0f,std::min(1.0f,v));
}
float get_ph() {
	return std::max(0.0f,std::min(1.0f,draw_w/((level.w+2)*zoom)));
}
float get_pv() {
	return std::max(0.0f,std::min(1.0f,draw_h/((level.h+2)*zoom)));
}
void scroll_upd() {
	float ph=get_ph(),pv=get_pv();
	auto func=conv1;
	if(lock)func=conv2;
	block=1;
	gtk_adjustment_set_page_size(GTK_ADJUSTMENT(adj_h),ph);
	gtk_adjustment_set_page_size(GTK_ADJUSTMENT(adj_v),pv);
	gtk_adjustment_set_value(GTK_ADJUSTMENT(adj_h),func(-(cx-zoom)/(zoom*(level.w+2)-draw_w))*(1.00001f-ph));
	gtk_adjustment_set_value(GTK_ADJUSTMENT(adj_v),func(-(cy-zoom)/(zoom*(level.h+2)-draw_h))*(1.00001f-pv));
	block=0;
}
int key(GtkWidget*, GdkEvent *event, void*) {
	if(event->key.keyval==65505 || event->key.keyval==65506) {
		if(event->key.state==16||event->key.state==18)key_state=1;
		else if(event->key.state==17||event->key.state==19)key_state=0;
	} else if(event->key.keyval==65507 || event->key.keyval==65508) {
		printf("da: %d\n",event->key.state);
		if(event->key.state==16||event->key.state==18)key_state=2;
		else if(event->key.state==20||event->key.state==22)key_state=0;
	} else return 0;
	gtk_widget_queue_draw(drawable);
	return 0;
}
void scroll1_upd() {
	float ph=get_ph(),pv=get_pv();
	gtk_adjustment_set_page_size(GTK_ADJUSTMENT(adj_h),ph);
	gtk_adjustment_set_page_size(GTK_ADJUSTMENT(adj_v),pv);
	if(ph==1)cx=-(zoom*level.w-draw_w)/2;
	else cx=-(gtk_adjustment_get_value(GTK_ADJUSTMENT(adj_h))/(1.00001f-ph)*(zoom*(level.w+2)-draw_w)-zoom);
	if(pv==1)cy=-(zoom*level.h-draw_h)/2;
	else cy=-(gtk_adjustment_get_value(GTK_ADJUSTMENT(adj_v))/(1.00001f-pv)*(zoom*(level.h+2)-draw_h)-zoom);
}
int mouse_scroll(GtkWidget*, GdkEventScroll *event, void*) {
	float zoom_old=zoom;
	if(event->direction==GDK_SCROLL_UP) {
		zoom_plus();
	} else if(event->direction==GDK_SCROLL_DOWN) {
		zoom_minus();
	}
	cx=event->x-(float(event->x-cx)/zoom_old*zoom);
	cy=event->y-(float(event->y-cy)/zoom_old*zoom);
	scroll_upd();
	if(lock)scroll1_upd();
	rulers_update();
	gtk_widget_queue_draw(drawable);
	return 0;
}
void select_template(GtkWidget*,GdkEventButton*,string* str) {
	*str=get_open_path("game/templates").c_str();
}
void add_but() {
	string tmp="";
	GtkWidget *dialog = gtk_dialog_new_with_buttons ("Create new body",
						GTK_WINDOW (window),
						GDF(GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT),
						GTK_STOCK_CANCEL,
						GTK_RESPONSE_CANCEL,
						GTK_STOCK_OK,
						GTK_RESPONSE_OK,
						NULL);
	GtkWidget *text=gtk_label_new("ID");
	GtkWidget *entry=gtk_entry_new();
	GtkWidget *box=gtk_vbox_new(0,10);
	GtkWidget *idbox=gtk_hbox_new(0,10);
	GtkWidget *but=gtk_button_new_with_label("Choose template");
	GtkWidget *combo=gtk_combo_box_new_text();
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"Static");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"Dynamic");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo),"Kinematic");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo),0);
	gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (dialog))), box, 0,0,6);
	gtk_container_add (GTK_CONTAINER(box),idbox);
	gtk_container_add (GTK_CONTAINER(box),combo);
	gtk_container_add (GTK_CONTAINER(box),but);
	gtk_container_add (GTK_CONTAINER(idbox),text);
	gtk_container_add (GTK_CONTAINER(idbox),entry);
	gtk_entry_set_text(GTK_ENTRY(entry),get_free_name("Body").c_str());

	gtk_dialog_set_default_response(GTK_DIALOG(dialog),GTK_RESPONSE_OK);

	g_signal_connect(G_OBJECT(but), "button_press_event", G_CALLBACK(select_template),&tmp);

	gtk_widget_show_all(dialog);

	if(gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_OK) {
		Body *b=new Body;
		if(!tmp.size()) {
			b->id=gtk_entry_get_text(GTK_ENTRY(entry));
			b->type=gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
		} else {
			b->template_open(tmp);
			b->resolve_names();
			create_status=3;
		}
		unselect_all_shapes();
		b->selected=1;
		level.bodies.push_back(b);
		fill_shapes();
		select_tree_view(b->id);
		b->vupdate();
	}
	gtk_widget_destroy(dialog);
	key_state=0;
}
void cpy_but() {
	copy();
	paste();
}
void rem_but() {
	Object *obj=get_selected_object();
	if(!obj)return;
	Object *p=parent(obj->id);
	remove_selected();
	if(p) {
		select_tree_view(p->id);
		select_object();
	}
	delete_id(obj->id);
	gtk_widget_queue_draw(drawable);
}
void views_set() {
	shows[0]=(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Background layer"))->active);
	shows[1]=(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Physic layer"))->active);
	shows[2]=(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Foreground layer"))->active);
	shows[3]=(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Static bodies"))->active);
	shows[4]=(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Dynamic bodies"))->active);
	shows[5]=(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Kinematic bodies"))->active);
	shows[6]=(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Joints"))->active);
	gtk_widget_queue_draw(drawable);
}
void fullscreen_invert() {
	if(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Fullscreen"))->active)
		gtk_window_fullscreen(GTK_WINDOW(window));
	else	gtk_window_unfullscreen(GTK_WINDOW(window));
}
void lock_invert() {
	lock=(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/Editor/Lock level"))->active);
}
void copy() {
	Body *s=get_selected_body();
	if(!s) {
		set_status(CONTEXT_EDIT_ERROR,"You don't select any body");
	} else {
		remove_status(CONTEXT_EDIT_ERROR);
		s->template_save("/tmp/le-clipboard.xml");
	}
}
void paste() {
	ifstream f("/tmp/le-clipboard.xml");
	if(f) {
		Body *s=new Body;
		s->template_open("/tmp/le-clipboard.xml");
		unselect_all_shapes();
		s->selected=1;
		s->resolve_names();
		level.bodies.push_back(s);
		fill_shapes();
		select_tree_view(s->id);
		s->vupdate();
		create_status=3;
	}
	f.close();
}
void cut() {
	Object *obj=get_selected_object();
	if(!obj)return;
	Object *p=parent(obj->id);
	if(!p)return;
	unselect_all_shapes();
	p->selected=1;
	copy();
	rem_but();
	gtk_widget_queue_draw(drawable);
}
void change_mask(){
	Physic *p=(Physic*)get_selected_object();
	GtkWidget *dialog = gtk_dialog_new_with_buttons ("Set grid",
						GTK_WINDOW (window),
						GDF(GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT),
						GTK_STOCK_CANCEL,
						NULL,
						GTK_STOCK_OK,
						GTK_RESPONSE_OK,
						NULL);
	GtkObject *a=gtk_adjustment_new(grid,0.01,100,0.125,0.125,0);
	GtkWidget *table=gtk_table_new(4,4,1);
	GtkWidget *button[16];
	bitset<16>buf(p->mask);
	for(int q=0;q<16;q++){
		button[q]=gtk_check_button_new_with_label(to_str(q).c_str());
		gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(button[q]),buf[q]);
		gtk_table_attach(GTK_TABLE(table),button[q],q/4,q/4+1,q%4,q%4+1,GTK_F,GTK_F,0,0);
	}
	gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (dialog))), table, FALSE, FALSE, 6);
	gtk_widget_show_all(dialog);
	if(gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_OK) {
		for(int q=0;q<16;q++){
			buf[q]=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button[q]));
		}
		p->mask=static_cast<unsigned short>(buf.to_ulong());
	}
	gtk_widget_destroy(dialog);
}
