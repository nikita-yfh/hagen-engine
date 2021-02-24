#include "editor.hpp"
#include "level.hpp"
#include "events.hpp"
#include "defines.hpp"
#include "utils.hpp"
#include "tree_view.hpp"
#include <cmath>
#include <cairo.h>
using namespace std;
unsigned int tool_ch=1, point_ch=0;
GtkWidget	*window,
			*box_v, *box_h,
			*main_area,
			*scroll_h, *scroll_v,
			*drawable,
			*ruler_h, *ruler_v,
			*panel,
			*buttons,
			*tools,
			*tool[TOOL_COUNT],
			*zoomer,
			*prop_table, *prop_scroll,
			*shapes_scroll,
			*eventbox, *status,
			*zoom_b, *zoom_text,
			*down_panel, *view_a,
			*shape_view,
			*p1;
GtkObject	*adj_h, *adj_v, *zoom_a;
GtkItemFactory
*menu;
GtkTreeStore
*store;
GtkCellRenderer
*text_ren;
string save_path="";
bool shows[7]= {1,1,1,1,1,1,1};
void newl() {
	for(int q=0;q<level.joints.size();q++)
		delete level.joints[q];
	for(int q=0;q<level.bodies.size();q++)
		delete level.bodies[q];
	level.joints.resize(0);
	level.bodies.resize(0);
}
void run_callback(void*){
	string name=save_path;
	int pos=name.rfind("/");
	name.erase(name.begin(),name.begin()+pos);
	pos=name.rfind(".");
	name.erase(name.begin()+pos,name.end());
	name="./engine "+name;
	system(name.c_str());
}
void run(){
	save();
	if(save_path=="")return;
	g_thread_new("run",GThreadFunc(run_callback),0);
}
string get_save_path(string path) {
	GtkWidget *dialog = gtk_file_chooser_dialog_new ("Save file",
						GTK_WINDOW(window),GTK_FILE_CHOOSER_ACTION_SAVE,
						"_Cancel",GTK_RESPONSE_CANCEL,
						"_Save",GTK_RESPONSE_ACCEPT, NULL);
	GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	gtk_file_chooser_set_current_folder(chooser, path.c_str());
	string str;
	if(gtk_dialog_run (GTK_DIALOG (dialog))==GTK_RESPONSE_ACCEPT) {
		str=gtk_file_chooser_get_filename (chooser);
	}
	gtk_widget_destroy (dialog);
	key_state=0;
	return str;
}
string get_folder_path(string path) {
	GtkWidget *dialog = gtk_file_chooser_dialog_new ("Save in folder",
						GTK_WINDOW(window),GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER,
						"_Cancel",GTK_RESPONSE_CANCEL,
						"_Save",GTK_RESPONSE_ACCEPT, NULL);
	GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	gtk_file_chooser_set_current_folder(chooser, path.c_str());
	string str;
	if(gtk_dialog_run (GTK_DIALOG (dialog))==GTK_RESPONSE_ACCEPT) {
		str=gtk_file_chooser_get_filename (chooser);
	}
	gtk_widget_destroy (dialog);
	key_state=0;
	return str;
}
string get_open_path(string path) {
	GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open file",
						GTK_WINDOW(window),GTK_FILE_CHOOSER_ACTION_OPEN,
						"_Cancel",GTK_RESPONSE_CANCEL,
						"_Open",GTK_RESPONSE_ACCEPT, NULL);
	GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	gtk_file_chooser_set_current_folder(chooser, path.c_str());
	string str;
	if(gtk_dialog_run (GTK_DIALOG (dialog))==GTK_RESPONSE_ACCEPT) {
		str=gtk_file_chooser_get_filename (chooser);
	}
	gtk_widget_destroy (dialog);
	key_state=0;
	return str;
}
void save_entity(){
	GtkWidget *dialog = gtk_dialog_new_with_buttons ("Resize",
							GTK_WINDOW (window),
							GDF(GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT),
							GTK_STOCK_CANCEL,
							NULL,
							GTK_STOCK_OK,
							GTK_RESPONSE_OK,
							NULL);
	GtkWidget *hl_box=gtk_hbox_new(0,0);

	gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (dialog))), hl_box, FALSE, FALSE, 6);
	gtk_widget_show_all(dialog);
	if(gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_OK) {
	}
	gtk_widget_destroy(dialog);
}
void save_as() {
	string str=get_save_path("levels");
	if(str.size()) {
		save_path=str;
		if(level.save_file(save_path))
			set_status(CONTEXT_FILE_STATUS, "Save error");
		else
			set_status(CONTEXT_FILE_STATUS, "Saved successfully");
	}
}
void save() {
	if(save_path=="")
		save_as();
	else if(level.save_file(save_path))
        set_status(CONTEXT_FILE_STATUS, "Save error");
    else
        set_status(CONTEXT_FILE_STATUS, "Saved successfully");
}
void open() {
	string str=get_save_path("levels");
	if(str.size()) {
		save_path=str;
		if(level.open_file(str))
			set_status(CONTEXT_FILE_STATUS, "Load error");
		else
			set_status(CONTEXT_FILE_STATUS, "Loaded successfully");
	}
	fill_shapes();
	gtk_widget_queue_draw(drawable);
}

void change_tool(unsigned int a) {
	tool_ch=a;
	if(block)return;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tool[a]),1);
}
int cx=0,cy=0,xp=0,yp=0,xd=0,yd=0;
float zoom=100.0f;
float grid=0.5f;
float mod(float a,float b) {
	return a-b*int(a/b);
}
int draw_callback (GtkWidget *da, GdkEventExpose *event, void*) {
	cairo_t *cr = gdk_cairo_create (gtk_widget_get_window (da));
	gdk_cairo_rectangle (cr, &event->area);
	cairo_clip (cr);
	if(zoom*get_grid()>5) {
		cairo_set_source_rgb (cr, LIGHT_GRAY);//светлая сетка
		for(float q=mod(cx,zoom)-zoom; q<mod(cx,zoom)+draw_w+zoom; q+=get_grid()*zoom)	cairo_rectangle (cr, q,0,1,draw_h); //горизонтальные полоски
		for(float q=mod(cy,zoom)-zoom; q<mod(cy,zoom)+draw_h+zoom; q+=get_grid()*zoom)	cairo_rectangle (cr, 0,q,draw_w,1); //вертикальные полоски
		cairo_fill(cr);
	}
	cairo_set_source_rgb (cr,BLACK);//границы сетки темные
	if(cx>=0)cairo_rectangle(cr,cx,0,1,draw_h);
	if(cy>=0)cairo_rectangle(cr,0,cy,draw_w,1);
	if(-cx<=level.w*zoom)cairo_rectangle(cr,level.w*zoom+cx,0,1,draw_h);
	if(-cy<=level.h*zoom)cairo_rectangle(cr,0,level.h*zoom+cy,draw_w,1);
	cairo_fill (cr);
	vector<Object*>vec=get_all();
	for(auto *shape : vec)
		shape->draw(cr);
	cairo_destroy(cr);
	return 0;
}
void resize() {
	GtkWidget *dialog = gtk_dialog_new_with_buttons ("Resize",
						GTK_WINDOW (window),
						GDF(GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT),
						GTK_STOCK_CANCEL,
						NULL,
						GTK_STOCK_OK,
						GTK_RESPONSE_OK,
						NULL);
	GtkWidget *table=gtk_table_new(2,2,0);
	GtkObject *adj_w=gtk_adjustment_new(level.w,1,500,1,10,0);
	GtkObject *adj_h=gtk_adjustment_new(level.h,1,500,1,10,0);
	GtkWidget *w=gtk_spin_button_new(GTK_ADJUSTMENT(adj_w),0.2,0);
	GtkWidget *h=gtk_spin_button_new(GTK_ADJUSTMENT(adj_h),0.2,0);
	GtkWidget *wt=gtk_label_new("Width: ");
	GtkWidget *ht=gtk_label_new("Height:");
	gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (dialog))), table, FALSE, FALSE, 6);
	gtk_table_attach(GTK_TABLE(table),wt,0,1,0,1,GTK_F,GTK_F,0,0);
	gtk_table_attach(GTK_TABLE(table),ht,0,1,1,2,GTK_F,GTK_F,0,0);
	gtk_table_attach(GTK_TABLE(table),w,1,2,0,1,GTK_EFS,GTK_EFS,0,0);
	gtk_table_attach(GTK_TABLE(table),h,1,2,1,2,GTK_EFS,GTK_EFS,0,0);

	gtk_widget_show_all(dialog);
	if(gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_OK) {
		level.w=gtk_adjustment_get_value(GTK_ADJUSTMENT(adj_w));
		level.h=gtk_adjustment_get_value(GTK_ADJUSTMENT(adj_h));
	}
	gtk_widget_destroy(dialog);
	scroll_upd();
	scroll1_upd();
	key_state=0;
}
void set_bgr() {
	const char *bgr_path="backgrounds/";
	string str=get_open_path(bgr_path);
	if(str.size()) {
		int n=str.find(bgr_path);  //find
		if(n>=0 && n<1000)  //found
			str.erase(str.begin(),str.begin()+n+strlen(bgr_path));
		level.background=str;
	}
}
void quit() {
	gtk_main_quit();
}
void create_menu() {
	GtkIconTheme *icon_theme = gtk_icon_theme_get_default();
	GError *error;

	GtkItemFactoryEntry menu_items[] = {
		{"/File/New level",					"<control>N",			newl,				0,	"<StockItem>",	"gtk-new"},
		{"/File/Open",						"<control>O",			open,				0,	"<StockItem>",	"gtk-open"},
		{"/File/Save",						"<control>S",			save,				0,	"<StockItem>",	"gtk-save"},
		{"/File/Save as",					"<control><shift>S",	save_as,			0,	"<StockItem>",	"gtk-save-as"},
		{"/File/Save as template",			"",						template_save,		0,	"<StockItem>",	"gtk-save-as"},
		{"/File/Save as entity",			"",						save_entity,		0,	"<StockItem>",	"gtk-save-as"},
		{"/File/Quit",						"<control>Q",			quit,				0,	"<StockItem>",	"gtk-quit"},
		{"/Edit/Copy",						"<control>C",			copy,				0,	"<StockItem>",	"gtk-copy"},
		{"/Edit/Paste",						"<control>V",			paste,				0,	"<StockItem>",	"gtk-paste"},
		{"/Edit/Cut",						"<control>X",			cut,				0,	"<StockItem>",	"gtk-cut"},
		{"/View/Fullscreen",				"F11",					fullscreen_invert,	0,	"<ToggleItem>"},
		{"/View/Zoom in",					"KP_Add",				zoom_plus,			0,	"<StockItem>",	"gtk-zoom-in"},
		{"/View/Zoom out",					"KP_Subtract",			zoom_minus,			0,	"<StockItem>",	"gtk-zoom-out"},
		{"/View/Zoom normal",				"<control>0",			zoom_normal,		0,	"<StockItem>",	"gtk-zoom-100"},
		{"/View/Background layer",			"<control>B",			views_set,			0,	"<ToggleItem>"},
		{"/View/Physic layer",				"<control>P",			views_set,			0,	"<ToggleItem>"},
		{"/View/Foreground layer",			"<control>F",			views_set,			0,	"<ToggleItem>"},
		{"/View/Static bodies",				"",						views_set,			0,	"<ToggleItem>"},
		{"/View/Dynamic bodies",			"",						views_set,			0,	"<ToggleItem>"},
		{"/View/Kinematic bodies",			"",						views_set,			0,	"<ToggleItem>"},
		{"/View/Joints",					"<control>J",			views_set,			0,	"<ToggleItem>"},
		{"/Tools/View",						"<control>W",			L(change_tool(0);)},
		{"/Tools/Move",						"<control>M",			L(change_tool(1);)},
		{"/Tools/Shapes/Polygon",			"<control>1",			L(change_tool(2);)},
		{"/Tools/Shapes/Rectangular",		"<control>2",			L(change_tool(3);)},
		{"/Tools/Shapes/Square",			"<control>3",			L(change_tool(4);)},
		{"/Tools/Shapes/Circle",			"<control>4",			L(change_tool(5);)},
		{"/Tools/Shapes/Line",				"<control>5",			L(change_tool(6);)},
		{"/Tools/Joints/Revolute joint",	"<control><shift>1",	L(change_tool(7);)},
		{"/Tools/Joints/Weld joint",		"<control><shift>2",	L(change_tool(8);)},
		{"/Tools/Joints/Prismatic joint",	"<control><shift>3",	L(change_tool(9);)},
		{"/Tools/Joints/Pulley joint",		"<control><shift>4",	L(change_tool(10);)},
		{"/Tools/Joints/Gear joint",		"<control><shift>5",	L(change_tool(11);)},
		{"/Tools/Joints/Distance joint",	"<control><shift>6",	L(change_tool(12);)},
		{"/Properties/Level size", 			"<control>R",           resize},
		{"/Properties/Level background",	"<control>I",           set_bgr}
	};
	GtkAccelGroup *accel_group;
	accel_group = gtk_accel_group_new();
	menu = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<menu>", accel_group);
	gtk_item_factory_create_items(menu, 36, menu_items, NULL);
	gtk_window_add_accel_group(GTK_WINDOW(window),accel_group);
	gtk_box_pack_start(GTK_BOX(box_v), gtk_item_factory_get_widget(menu, "<menu>"), FALSE, FALSE, 0);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Background layer")),1);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Physic layer")),1);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Foreground layer")),1);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Static bodies")),1);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Dynamic bodies")),1);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Kinematic bodies")),1);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_widget(menu, "/View/Joints")),1);
}
void create_tree_view() {
	store = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
	shape_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	text_ren = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(shape_view),0, "ID", text_ren,"text", 0,0);
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(shape_view),1, "Type", text_ren,"text",1,0);
	gtk_container_add(GTK_CONTAINER(shapes_scroll), shape_view);
	g_signal_connect(G_OBJECT(shape_view),"cursor-changed",select_object,0);

	gtk_container_add(GTK_CONTAINER(panel),shapes_scroll);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(prop_scroll),prop_table);
}

void create_tools() {
	tools=gtk_toolbar_new();
	gtk_toolbar_insert_stock(GTK_TOOLBAR(tools),"gtk-new",			"New level","",				newl,	0,-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(tools),"gtk-open",			"Open existing level","",	open,	0,-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(tools),"gtk-save",			"Save this level","",		save,	0,-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(tools),"gtk-copy",			"Copy selection","",		copy,	0,-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(tools),"gtk-paste",		"Paste clipboard","",		paste,	0,-1);
	gtk_toolbar_append_space (GTK_TOOLBAR(tools));
	gtk_toolbar_insert_stock(GTK_TOOLBAR(tools),"gtk-media-play",	"Run","",					run,	0,-1);
	gtk_toolbar_append_space (GTK_TOOLBAR(tools));
	tool[0]=	NEW_TOOLRB(tools,0,			0,0,0,"move.png",		L(block=1; change_tool(0); block=0;));
	tool[1]=	NEW_TOOLRB(tools,tool[0],	0,0,0,"cursor.png",		L(block=1; change_tool(1); block=0;));
	tool[2]=	NEW_TOOLRB(tools,tool[1],	0,0,0,"polygon.png",	L(block=1; change_tool(2); block=0;));
	tool[3]=	NEW_TOOLRB(tools,tool[2],	0,0,0,"rect.png",		L(block=1; change_tool(3); block=0;));
	tool[4]=	NEW_TOOLRB(tools,tool[3],	0,0,0,"square.png",		L(block=1; change_tool(4); block=0;));
	tool[5]=	NEW_TOOLRB(tools,tool[4],	0,0,0,"circle.png",		L(block=1; change_tool(5); block=0;));
	tool[6]=	NEW_TOOLRB(tools,tool[5],	0,0,0,"line.png",		L(block=1; change_tool(6); block=0;));
	tool[7]=	NEW_TOOLRB(tools,tool[6],	0,0,0,"revolute.png",	L(block=1; change_tool(7); block=0;));
	tool[8]=	NEW_TOOLRB(tools,tool[7],	0,0,0,"weld.png",		L(block=1; change_tool(8); block=0;));
	tool[9]=	NEW_TOOLRB(tools,tool[8],	0,0,0,"prismatic.png",	L(block=1; change_tool(9); block=0;));
	tool[10]=	NEW_TOOLRB(tools,tool[9],	0,0,0,"pulley.png",		L(block=1; change_tool(10); block=0;));
	tool[11]=	NEW_TOOLRB(tools,tool[10],	0,0,0,"gear.png",		L(block=1; change_tool(11); block=0;));
	tool[12]=	NEW_TOOLRB(tools,tool[11],	0,0,0,"distance.png",	L(block=1; change_tool(12); block=0;));
	gtk_toolbar_set_style(GTK_TOOLBAR(tools),GTK_TOOLBAR_ICONS);
	gtk_box_pack_start(GTK_BOX(box_v),tools,0,1,0);
}
void create_buttons() {
	buttons=gtk_toolbar_new();
	gtk_box_pack_end(GTK_BOX(panel),buttons,0,0,0);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(buttons),"gtk-new",	0,0,add_but,0,-1);
	GtkWidget *c=gtk_toolbar_insert_stock(GTK_TOOLBAR(buttons),"gtk-copy",	0,0,cpy_but,0,-1);
	GtkWidget *d=gtk_toolbar_insert_stock(GTK_TOOLBAR(buttons),"gtk-delete",	0,0,rem_but,0,-1);

	GtkAccelGroup *accels = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(window), accels);
	gtk_widget_add_accelerator(d, "clicked", accels,
							   65535,GdkModifierType(0), GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(c, "clicked", accels,
							   'd',GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
}

void init() {
	//create
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	box_v=gtk_vbox_new(0,0);
	box_h=gtk_hbox_new(0,0);
	p1=gtk_hpaned_new();
	down_panel=gtk_hbox_new(0,0);
	status=gtk_statusbar_new();
	panel=gtk_vbox_new(0,0);
	ruler_h=gtk_hruler_new();
	ruler_v=gtk_vruler_new();
	main_area=gtk_table_new(3,4,0);
	drawable=gtk_drawing_area_new();
	eventbox=gtk_event_box_new();
	prop_scroll=gtk_scrolled_window_new(0,0);
	shapes_scroll=gtk_scrolled_window_new(0,0);
	adj_h=gtk_adjustment_new(0.5,0,1,0.01,0.01,0.5);
	adj_v=gtk_adjustment_new(0.5,0,1,0.01,0.01,0.5);
	zoom_a=gtk_adjustment_new(10,5.0,100.5,0.5,0,0.1);
	zoom_b=gtk_hscale_new(GTK_ADJUSTMENT(zoom_a));
	scroll_h=gtk_hscrollbar_new(GTK_ADJUSTMENT(adj_h));
	scroll_v=gtk_vscrollbar_new(GTK_ADJUSTMENT(adj_v));
	prop_table=gtk_table_new(2,2,0);
	zoom_text=gtk_label_new("100 px/m");

	create_all(prop_table);

	create_menu();
	create_tree_view();
	create_buttons();
	create_tools();
	//set
	gtk_widget_set_events (drawable, GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK);
	g_signal_connect_swapped (G_OBJECT (drawable),	"motion_notify_event",		CALLBACK_CLASS(ruler_v, motion_notify_event),G_OBJECT (ruler_v));
	g_signal_connect_swapped (G_OBJECT (drawable),	"motion_notify_event",		CALLBACK_CLASS(ruler_h, motion_notify_event),G_OBJECT (ruler_h));
	g_signal_connect(G_OBJECT(window),				"delete_event",				G_CALLBACK(quit), 0);
	g_signal_connect(G_OBJECT(drawable),			"expose_event",				G_CALLBACK(draw_callback), 0);
	g_signal_connect(G_OBJECT(eventbox),			"button_press_event",		G_CALLBACK(mousebutton_press), 0);
	g_signal_connect(G_OBJECT(eventbox),			"button_release_event",		G_CALLBACK(mousebutton_release), 0);
	g_signal_connect(G_OBJECT(eventbox),			"motion_notify_event",		G_CALLBACK(motion), 0);
	g_signal_connect(G_OBJECT(eventbox),			"scroll_event",				G_CALLBACK(mouse_scroll), 0);
	g_signal_connect(G_OBJECT(window),				"event",					G_CALLBACK(key), 0);
	g_signal_connect(G_OBJECT(scroll_h),			"value_changed",			G_CALLBACK(scroll),0);
	g_signal_connect(G_OBJECT(scroll_v),			"value_changed",			G_CALLBACK(scroll),0);
	g_signal_connect(G_OBJECT(drawable),			"size-allocate",			G_CALLBACK(getsize), 0);
	g_signal_connect(G_OBJECT(zoom_b),				"value_changed",			G_CALLBACK(zoom1_upd), 0);
	gtk_ruler_set_metric (GTK_RULER (ruler_h), GTK_PIXELS);
	gtk_ruler_set_metric (GTK_RULER (ruler_v), GTK_PIXELS);
	gtk_ruler_set_range (GTK_RULER (ruler_h), 7, 13, 0, 80);
	gtk_ruler_set_range (GTK_RULER (ruler_v), 7, 13, 0, 80);
	gtk_scale_set_draw_value(GTK_SCALE(zoom_b),0);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(prop_scroll),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(shapes_scroll),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	//pack
	gtk_container_add(GTK_CONTAINER(window),	box_v);
	gtk_container_add(GTK_CONTAINER(box_v),		box_h);
	gtk_container_add(GTK_CONTAINER(eventbox),	drawable);

	gtk_box_pack_start(GTK_BOX(panel),		prop_scroll,	1,1,0);
	gtk_box_pack_start(GTK_BOX(box_h),p1,1,1,0);

	gtk_paned_pack2(GTK_PANED(p1),main_area,1,0);
	gtk_paned_pack1(GTK_PANED(p1),panel,	0,0);
	gtk_paned_set_position(GTK_PANED(p1),256);

	gtk_box_pack_start(GTK_BOX(down_panel),zoom_text,0,1,10);
	gtk_box_pack_start(GTK_BOX(down_panel),status,1,1,10);

	gtk_box_pack_start(GTK_BOX(panel),zoom_b,0,1,0);

	gtk_table_attach(GTK_TABLE(main_area),	eventbox,	1,2,1,2,	GTK_EF,GTK_EF,	0,0);
	gtk_table_attach(GTK_TABLE(main_area),	ruler_h,	1,2,0,1,	GTK_EFS,GTK_F,	0,0);
	gtk_table_attach(GTK_TABLE(main_area),	ruler_v,	0,1,1,2,	GTK_F,GTK_EFS,	0,0);
	gtk_table_attach(GTK_TABLE(main_area),	scroll_h,	1,2,2,3,	GTK_EFS,GTK_F,	0,0);
	gtk_table_attach(GTK_TABLE(main_area),	scroll_v,	2,3,1,2,	GTK_F,GTK_EFS,	0,0);
	gtk_table_attach(GTK_TABLE(main_area),	down_panel,	0,3,3,4,	GTK_F,GTK_F,	0,0);

	zoom_upd();
	fill_shapes();
	gtk_widget_show_all(window);
	hide_all();

	change_tool(1);

	gtk_window_resize(GTK_WINDOW(window),800,600);
}
