#pragma once
#include <gtk/gtk.h>
#include <string>
#include "main.hpp"
extern GtkWidget
*window,
*box_v,
*box_h,
*main_area,
*scroll_h, *scroll_v,
*drawable,
*ruler_h, *ruler_v,
*left_panel, *right_panel,
*tools,
*tool[TOOL_COUNT],
*zoomer,
*prop_table, *prop_scroll,
*shapes_scroll,
*eventbox, *status,
*zoom_b, *zoom_text,
*down_panel, *view_a,
*shape_view,
*p1, *p2;
extern GtkObject
*adj_h, *adj_v, *zoom_a;
extern GtkItemFactory
*menu;
extern GtkTreeStore
*store;
extern GtkCellRenderer
*text_ren;

extern unsigned int tool_ch, point_ch;
extern bool shows[7],lock;
extern float zoom,grid;
extern int cx,cy,xp,yp,xd,yd;
void change_tool(unsigned int);
void fill_shapes();
void init();
void save();
void open();
void save_as();
void save_only_physic();
std::string get_save_path(std::string path);
std::string get_open_path(std::string path);
std::string get_folder_path(std::string path);
extern std::string save_path;
