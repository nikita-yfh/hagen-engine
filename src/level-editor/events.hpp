#pragma once
#include <gtk/gtk.h>
#include <stdint.h>
#include <algorithm>
#include <string>
void scroll_upd();
void scroll1_upd();
int motion(GtkWidget*, GdkEventMotion *event, gpointer data);
int mousebutton_release(GtkWidget *area,  GdkEventButton  *event, gpointer data);
int mousebutton_press(GtkWidget *area,  GdkEventButton  *event, gpointer data);
void getsize(GtkWidget*, GtkAllocation *allocation, void*);
int scroll(GtkWidget*, GdkEvent *event, gpointer data);
int mouse_scroll(GtkWidget*, GdkEventScroll *event, gpointer data);
int key(GtkWidget*, GdkEvent *event, void*);
void zoom_plus();
void zoom_minus();
void zoom_normal();
void zoom_upd();
void zoom1_upd();
void unselect_all_shapes();

void rem_but();
void cpy_but();
void add_but();

void copy();
void paste();
void cut();

extern int draw_w,draw_h;
extern uint8_t mouse_button;
extern bool block;
extern uint8_t key_state;
void views_set();
void fullscreen_invert();

extern uint8_t create_status;
