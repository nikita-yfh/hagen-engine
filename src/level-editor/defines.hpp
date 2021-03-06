#pragma once
#define GTK_EF	GtkAttachOptions(GTK_EXPAND|GTK_FILL)
#define GTK_SF	GtkAttachOptions(GTK_SHRINK|GTK_FILL)
#define GTK_ES	GtkAttachOptions(GTK_EXPAND|GTK_SHRINK)
#define GTK_EFS	GtkAttachOptions(GTK_EXPAND|GTK_SHRINK|GTK_FILL)
#define GTK_E	GTK_EXPAND
#define GTK_F	GTK_FILL
#define GTK_S	GTK_SHRINK
#define GP(a)	gpointer(a)
#define TYPE(a,b) (static_cast<a>(b))
#define CALLBACK_CLASS(a,b) G_CALLBACK (GTK_WIDGET_GET_CLASS(a)->b)
#define RB_N(t) gtk_radio_button_new_with_label(0,t)
#define RB(b,t) gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(b),t)
#define MENU_APPEND(a,b) gtk_menu_shell_append (GTK_MENU_SHELL (a), b);
#define TOOL_COUNT 13
#define L(a) [](){a}
#define LIGHT_GRAY 0.8,0.8,0.8
#define GREEN 0,1,0
#define GRAY 0.7,0.7,0.7
#define RED 1,0,0
#define BLACK 0,0,0
#define STATIC_LAYER 0.8,0,0,0.3
#define DYNAMIC_LAYER 0,0.8,0,0.3
#define KINEMATIC_LAYER 0,0,0.8,0.3
#define GDF(a) GtkDialogFlags(a)
#define sqr(a) ((a)*(a))
#define ALL_PROPERTIES 40
#define DEBUG printf("%d\n",rand())
#define DENSITY_MAX 100000.0f
#define FRICTION_MAX 1
#define RESTITUTION_MAX 1
#define DENSITY_STEP 10
#define FRICTION_STEP 0.05
#define RESTITUTION_STEP 0.05
#define POINT_SIZE 7
#define ICON_PATH "icons"
#define NEW_TOOLRB(bar,prev,text,text1,text2,icon,callback) gtk_toolbar_append_element(GTK_TOOLBAR(bar),\
																		GTK_TOOLBAR_CHILD_RADIOBUTTON,\
																		prev,text,text1,text2,\
																		gtk_image_new_from_file(ICON_PATH"/" icon ),\
																		callback,0);
#define CONTEXT_ID_ERROR		0
#define CONTEXT_FILE_STATUS		1
#define CONTEXT_CREATE_ERROR	2
#define CONTEXT_TEXTURE_ERROR	3
#define CONTEXT_EDIT_ERROR		4

#define TSTAT	0
#define TDYN	1
#define TKNM	2

#define TBGR 0
#define TMGR 1
#define TFGR 2

#define VERSION "0.1"
#define _(a) gettext(a)
