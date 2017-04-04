#ifndef MYDATA_H
#define MYDATA_H

#define MYDATA_MAGIC 0x46EA7E05

#include"game.h"

typedef unsigned int uint;

typedef enum {
	BSP_FIRST,
	BSP_OPEN,
	BSP_CLOSE,
	BSP_PROLONG,
	BSP_FILL,
	BSP_CLIP,
	BSP_LAST
}Bsp_state;

typedef enum {
	EDIT_FIRST,
	EDIT_ADD_CURVE,
	EDIT_MOVE_CURVE,
	EDIT_REMOVE_CURVE,
	EDIT_ADD_CONTROL,
	EDIT_MOVE_CONTROL,
	EDIT_REMOVE_CONTROL,
	EDIT_MOVE_CLIP,
	EDIT_RESET_CLIP,
	EDIT_LAST
}Edit_state;

typedef struct {
	GtkWidget *window;
	GtkWidget *win_scale;
	GtkWidget *scale;

	Edit_state edit_mode;
	Bsp_state bsp_mode;
	GtkWidget *edit_radios[EDIT_LAST];
	GtkWidget *bsp_radios[BSP_LAST];

	char *title;
	char *Current_folder;

	gint win_width;		//width ==> largeur
	gint win_height;

	// GtkWidget *image;
	GtkWidget * vbox;
		GtkWidget * menu_bar;
		GtkWidget * hbox;
			GtkWidget * frame;
			GtkWidget * elderscroll;
				GtkWidget * area;
		GtkWidget * status;

	GdkPixbuf *pixbuf1;
	GdkPixbuf *pixbuf2;

	double area_w;
	double area_h;

	gdouble scale_value;
	gdouble rotate_angle;
	gdouble click_x;
	gdouble click_y;
	gdouble last_x;
	gdouble last_y;

	gint click_n;
	gint clip_image;
	gint show_edit;

	uint Magicnumber;	// 3 is a magic number

	Curve_infos curve_infos;

	Game game;
} Mydata;

Mydata *get_mydata (gpointer data);

void init_mydata(Mydata *my);
void set_edit_mode (Mydata * my, int mode);
void set_bsp_mode (Mydata * my, int mode);

#endif