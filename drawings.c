#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "curve.h"
#include "game.h"
#include "util.h"
#include "mydata.h"
#include "drawings.h"
#include "menu.h"
#include "font.h"


void apply_image_transforms (Mydata *data){
	Mydata *my = get_mydata(data);    
	g_clear_object(&my->pixbuf2);
	if (my->pixbuf1 != NULL){
		GdkPixbuf *tmp =gdk_pixbuf_rotate_simple (my->pixbuf1, my->rotate_angle);
    	gint width = gdk_pixbuf_get_width (tmp);
    	gint height = gdk_pixbuf_get_height (tmp);
		my->pixbuf2 = gdk_pixbuf_scale_simple (tmp, width * my->scale_value, height * my->scale_value, GDK_INTERP_BILINEAR);

		g_object_unref (tmp);
	}
}

void update_area_with_transforms (Mydata *data) {
	Mydata *my = get_mydata(data);
	apply_image_transforms (my);
	if (my->pixbuf2 != NULL) {
		gtk_widget_set_size_request (my->area, gdk_pixbuf_get_width (my->pixbuf2), gdk_pixbuf_get_height (my->pixbuf2));    
		refresh_area (my->area);
	}
}

void draw_control_labels(cairo_t * cr, PangoLayout * layout, Curve_infos * ci){
	font_set_name(layout, "Sans");
	font_set_size(layout, 8);
	cairo_set_source_rgb(cr, 80/255., 80/255., 80/255.);
	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve = &ci->curve_list.curves[i];
		for (int j = 0; j < curve->control_count; ++j){
			font_draw_text (cr, layout, FONT_TC, curve->controls[j].x, curve->controls[j].y-15, "%d", j);
		}
	}	
}

void draw_control_polygons_open(cairo_t * cr,Curve_infos * ci){
	cairo_set_line_width(cr,1.5);
	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve=&ci->curve_list.curves[i];
		for (int j = 0; j < curve->control_count; ++j){
			int tmpx = curve->controls[j].x;
			int tmpy = curve->controls[j].y;
			if (j == ci->current_control && i == ci->current_curve){
		 		cairo_set_source_rgb(cr,1,0,0);
			}
			else{
				cairo_set_source_rgb(cr,0,0,0);
			}
			cairo_rectangle(cr, tmpx-3,tmpy-3,6,6); 
			cairo_stroke(cr);
		}
	}
	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve=&ci->curve_list.curves[i];
		for (int j = 0; j < curve->control_count-1; ++j){
			int tmpx1 = curve->controls[j].x;
			int tmpy1 = curve->controls[j].y;
			int tmpx2 = curve->controls[j+1].x;
			int tmpy2 = curve->controls[j+1].y;
			if (i==ci->current_curve){
				cairo_set_source_rgb(cr,1,1,0);
			}
			else{
				cairo_set_source_rgb(cr,0.5,0.5,0.5);
			}
			cairo_move_to(cr,tmpx1,tmpy1);
			cairo_line_to(cr,tmpx2,tmpy2);
			cairo_stroke(cr);
		}
	}
}

void draw_control_polygons_close(cairo_t * cr,Curve_infos * ci){
	cairo_set_line_width(cr,1.5);
	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve=&ci->curve_list.curves[i];
		for (int j = 0; j < curve->control_count; ++j){
			int tmpx = curve->controls[j].x;
			int tmpy = curve->controls[j].y;
			if (j == ci->current_control && i == ci->current_curve){
		 		cairo_set_source_rgb(cr,1,0,0);
			}
			else{
				cairo_set_source_rgb(cr,0,0,0);
			}
			cairo_rectangle(cr, tmpx-3,tmpy-3,6,6); 
			cairo_stroke(cr);
		}
	}
	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve=&ci->curve_list.curves[i];
		for (int j = 0; j < curve->control_count; ++j){
			int tmpx1 = curve->controls[j].x;
			int tmpy1 = curve->controls[j].y;
			int tmpx2 = curve->controls[(j+1)%curve->control_count].x;
			int tmpy2 = curve->controls[(j+1)%curve->control_count].y;
			if (i==ci->current_curve){
				cairo_set_source_rgb(cr,1,1,0);
			}
			else{
				cairo_set_source_rgb(cr,0.5,0.5,0.5);
			}
			cairo_move_to(cr,tmpx1,tmpy1);
			cairo_line_to(cr,tmpx2,tmpy2);
			cairo_stroke(cr);
		}
	}
}

void draw_bezier_polygons_open(cairo_t * cr, Curve_infos * ci){
	Control bez_pts[4];

	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve = &ci->curve_list.curves[i];

		for (int j = 0; j < curve->control_count -3; ++j){
			cairo_set_source_rgb(cr,0/255., 255/255., 0/255.);
			compute_bezier_points_open(curve, j, bez_pts);
			cairo_move_to(cr, bez_pts[0].x, bez_pts[0].y);
			cairo_line_to(cr, bez_pts[1].x, bez_pts[1].y);
			cairo_move_to(cr, bez_pts[2].x, bez_pts[2].y);
			cairo_line_to(cr, bez_pts[3].x, bez_pts[3].y);
			cairo_stroke(cr);
		}
	}
}

void draw_bezier_polygons_close(cairo_t * cr, Curve_infos * ci){
	Control bez_pts[4];

	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve = &ci->curve_list.curves[i];

		for (int j = 0; j < curve->control_count; ++j){
			cairo_set_source_rgb(cr,0/255., 255/255., 0/255.);
			compute_bezier_points_close(curve, j, bez_pts);
			cairo_move_to(cr, bez_pts[0].x, bez_pts[0].y);
			cairo_line_to(cr, bez_pts[1].x, bez_pts[1].y);
			cairo_move_to(cr, bez_pts[2].x, bez_pts[2].y);
			cairo_line_to(cr, bez_pts[3].x, bez_pts[3].y);
			cairo_stroke(cr);
		}
	}
}

void draw_bezier_polygons_prolong(cairo_t * cr, Curve_infos * ci){
	Control bez_pts[4];

	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve = &ci->curve_list.curves[i];

		for (int j = 0; j < curve->control_count+1; ++j){
			if (curve->control_count>2){
				if (j<2 || j>curve->control_count-2){
					compute_bezier_points_prolong(curve,j,bez_pts);
				}
				else if (j>1 && j<curve->control_count-1){
					compute_bezier_points_open(curve,j-2,bez_pts);
				}
			}
			cairo_set_source_rgb(cr,0/255., 255/255., 0/255.);
			cairo_move_to(cr, bez_pts[0].x, bez_pts[0].y);
			cairo_line_to(cr, bez_pts[1].x, bez_pts[1].y);
			cairo_move_to(cr, bez_pts[2].x, bez_pts[2].y);
			cairo_line_to(cr, bez_pts[3].x, bez_pts[3].y);
			cairo_stroke(cr);
		}
	}
}

void draw_bezier_curve(cairo_t *cr, Control bez_points[4], double theta){
	double bx[4], by[4];
	for (int i = 0; i < 4; ++i){
		bx[i]=bez_points[i].x;
		by[i]=bez_points[i].y;
	}
	for (double t= 0.; t+theta <= 1.; t+=theta){
		cairo_move_to(cr , compute_bezier_cubic(bx,t), compute_bezier_cubic(by,t));
		cairo_line_to(cr , compute_bezier_cubic(bx,t+theta), compute_bezier_cubic(by,t+theta));
		cairo_stroke(cr);
	}
}

void draw_bezier_curves_open (cairo_t *cr , Curve_infos *ci , double theta){
	Control bez_pts[4];

	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve = &ci->curve_list.curves[i];
		for (int j = 0; j < curve->control_count -3; ++j){
			compute_bezier_points_open(curve,j,bez_pts);
			/*
														//affichage points de bezier pour fail de gab
			for (int prout = 0; prout < 4; ++prout){
				cairo_set_source_rgb(cr,0,0,1);
				cairo_rectangle(cr, bez_pts[prout].x-3,bez_pts[prout].y-3,6,6); 
				cairo_stroke(cr);
			}
			*/
			cairo_set_source_rgb(cr,140/255., 0/255., 140/255.);
			draw_bezier_curve(cr,bez_pts,theta);
		}
	}
}

void draw_bezier_curves_close(cairo_t *cr , Curve_infos *ci , double theta){
	Control bez_pts[4];

	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve = &ci->curve_list.curves[i];
		for (int j = 0; j < curve->control_count; ++j){
			if (curve->control_count>2){
				compute_bezier_points_close(curve,j,bez_pts);
				cairo_set_source_rgb(cr,140/255., 0/255., 140/255.);
				draw_bezier_curve(cr,bez_pts,theta);
			}
			/*
														//affichage points de bezier pour fail de gab
			for (int prout = 0; prout < 4; ++prout){
				cairo_set_source_rgb(cr,0,0,1);
				cairo_rectangle(cr, bez_pts[prout].x-3,bez_pts[prout].y-3,6,6); 
				cairo_stroke(cr);
			}
			*/
		}
	}
}

void draw_bezier_curves_prolong(cairo_t *cr , Curve_infos *ci , double theta){
	Control bez_pts[4];
	cairo_set_source_rgb(cr,140/255., 0/255., 140/255.);
	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve = &ci->curve_list.curves[i];
		for (int j = 0; j < curve->control_count+1; ++j){
			if (curve->control_count>2){
				if (j<2 || j>curve->control_count-2){
					compute_bezier_points_prolong(curve,j,bez_pts);
				}
				else if (j>1 && j<curve->control_count-1){
					compute_bezier_points_open(curve,j-2,bez_pts);
				}
				draw_bezier_curve(cr,bez_pts,theta);
			}
			/*
														//affichage points de bezier pour fail de gab
			for (int prout = 0; prout < 4; ++prout){
				cairo_set_source_rgb(cr,0,0,1);
				cairo_rectangle(cr, bez_pts[prout].x-3,bez_pts[prout].y-3,6,6); 
				cairo_stroke(cr);
			}
			*/
		}
	}
}

void generate_bezier_path(cairo_t *cr, Control bez_points[4], double theta, int is_first){
	double bx[4], by[4];
	for (int i = 0; i < 4; ++i){
		bx[i]=bez_points[i].x;
		by[i]=bez_points[i].y;
	}
	if (is_first){
		cairo_move_to(cr , compute_bezier_cubic(bx,0.), compute_bezier_cubic(by,0.));
	}

	for (double t= 0.; t+theta <= 1.; t+=theta){
		cairo_line_to(cr , compute_bezier_cubic(bx,t+theta), compute_bezier_cubic(by,t+theta));
	}
}

void draw_bezier_curves_fill(cairo_t *cr , Curve_infos *ci , double theta){
	Control bez_pts[4];
	cairo_set_source_rgb(cr,140/255., 0/255., 140/255.);
	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve = &ci->curve_list.curves[i];
		for (int j = 0; j < curve->control_count; ++j){

			if (curve->control_count>2){
				compute_bezier_points_close(curve,j,bez_pts);
				generate_bezier_path(cr,bez_pts,theta,j==0);
			}
		}
		cairo_fill(cr);
	}
}

void draw_bezier_curves_clip(cairo_t *cr , Curve_infos *ci , double theta, gpointer data){
	Mydata * my = get_mydata(data);
	Control bez_pts[4];

	for (int i = 0; i < ci->curve_list.curve_count; ++i){
		Curve * curve = &ci->curve_list.curves[i];
		if (my->pixbuf2 !=NULL){
			gdk_cairo_set_source_pixbuf(cr,my->pixbuf2,curve->shift_x,curve->shift_y);
		}
		else{
			cairo_set_source_rgb(cr,140/255., 0/255., 140/255.);
		}
		for (int j = 0; j < curve->control_count; ++j){

			if (curve->control_count>2){
				compute_bezier_points_close(curve,j,bez_pts);
				generate_bezier_path(cr,bez_pts,theta,j==0);
			}
		}
		cairo_fill(cr);
	}
}

void draw_cannon(cairo_t * cr, gpointer data){
	Mydata * my = get_mydata(data);

	int tmpw, tmph;

	gtk_window_get_size(GTK_WINDOW(my->window), &tmpw, &tmph);

	int centre_x=tmpw/2;
	int centre_y=(tmph-70)/2;

	Game * game = &my->game;

	int sprite_w = cairo_image_surface_get_width(game->cannon_sprite);
	int sprite_h = cairo_image_surface_get_height(game->cannon_sprite);

	double xA = centre_x - sprite_w/2, yA = centre_y - sprite_h/2;

	//cairo_identity_matrix (cr);
	cairo_save(cr);
	cairo_translate (cr, centre_x, centre_y);
	cairo_rotate (cr, my->game.cannon_angle);
	//cairo_scale (cr, 0.8, 0.8);
	cairo_translate (cr, -centre_x, -centre_y);

	cairo_set_source_surface(cr, game->cannon_sprite, xA, yA);
	cairo_rectangle(cr, xA, yA, sprite_w, sprite_h);
	cairo_fill(cr);
	cairo_restore(cr);

	//cairo_surface_destroy(game->cannon_sprite);
}

void draw_munition(cairo_t * cr, gpointer data)
{
	Mydata * my = get_mydata(data);

	int centre_x=my->area_w/2;
	int centre_y=my->area_h/2;

	Game * game = &my->game;

	int sprite_w = cairo_image_surface_get_width(game->cannon_sprite);
	int sprite_h = cairo_image_surface_get_height(game->cannon_sprite);

	double xA = centre_x + sprite_w + 10, yA = centre_y - sprite_h/2 - 5;

	sprite_w = cairo_image_surface_get_width(game->sprite_ball_table[game->current_shot]);
	sprite_h = cairo_image_surface_get_height(game->sprite_ball_table[game->current_shot]);

	//cairo_identity_matrix (cr);
	cairo_save(cr);
	cairo_translate (cr, centre_x, centre_y);
	cairo_rotate (cr, my->game.cannon_angle);
	cairo_scale (cr, 0.35, 0.35);
	cairo_translate (cr, -centre_x, -centre_y);

	cairo_set_source_surface(cr, game->sprite_ball_table[game->current_shot], xA, yA);
	cairo_rectangle(cr, xA, yA, sprite_w, sprite_h);
	cairo_fill(cr);

	cairo_restore(cr);

	//cairo_surface_destroy(game->sprite_ball_table[randomized_val]);
}
void draw_next_munition(cairo_t * cr, gpointer data)
{
	Mydata * my = get_mydata(data);

	int centre_x=my->area_w/2;
	int centre_y=my->area_h/2;

	Game * game = &my->game;

	int sprite_w = cairo_image_surface_get_width(game->cannon_sprite);
	int sprite_h = cairo_image_surface_get_height(game->cannon_sprite);

	double xA = centre_x - 14.5*(sprite_w/10), yA = centre_y - sprite_h - 150;

	sprite_w = cairo_image_surface_get_width(game->sprite_ball_table[game->next_shot]);
	sprite_h = cairo_image_surface_get_height(game->sprite_ball_table[game->next_shot]);

	//cairo_identity_matrix (cr);
	cairo_save(cr);
	cairo_translate (cr, centre_x, centre_y);
	cairo_rotate (cr, my->game.cannon_angle);
	cairo_scale (cr, 0.15, 0.15);
	cairo_translate (cr, -centre_x, -centre_y);

	cairo_set_source_surface(cr, game->sprite_ball_table[game->next_shot], xA, yA);
	cairo_rectangle(cr, xA, yA, sprite_w, sprite_h);
	cairo_fill(cr);

	cairo_restore(cr);

	//cairo_surface_destroy(game->sprite_ball_table[randomized_val]);
}

void draw_shots(cairo_t * cr, gpointer data)
{
	Mydata * my = get_mydata(data);

	int centre_x=my->area_w/2;
	int centre_y=my->area_h/2;

	Game * game = &my->game;

	for (int i = 0; i < game->nb_shot_on_screen ; ++i)
	{
		Shot * shot = &game->shot_table[i];

		int sprite_w = cairo_image_surface_get_width(game->sprite_ball_table[shot->shot_color]);
		int sprite_h = cairo_image_surface_get_height(game->sprite_ball_table[shot->shot_color]);

		cairo_save(cr);
		cairo_translate (cr, centre_x, centre_y);
		//cairo_rotate (cr, my->game.cannon_angle);
		cairo_scale (cr, 0.35, 0.35);
		cairo_translate (cr, -centre_x, -centre_y);
	
		cairo_set_source_surface(cr, game->sprite_ball_table[shot->shot_color], shot->x, shot->y);
		cairo_rectangle(cr, shot->x, shot->y, sprite_w, sprite_h);
		cairo_fill(cr);
		cairo_restore(cr);
	}
}

gboolean on_timeout_1(gpointer data)
{
	Mydata * my = get_mydata(data);

	process_next_step(my);
	refresh_area(my->area);
	return TRUE;
}

gboolean on_area_draw(GtkWidget * widget,cairo_t * cr, gpointer data){
	Mydata * my = get_mydata(data);

	PangoLayout * layout = pango_cairo_create_layout(cr);
	draw_control_labels(cr, layout, &my->curve_infos);
	if (my->pixbuf2 != NULL){
		double width=gdk_pixbuf_get_width(my->pixbuf2);
		double height=gdk_pixbuf_get_height(my->pixbuf2);
		gdk_cairo_set_source_pixbuf(cr,my->pixbuf2,0,0);
		gtk_widget_set_size_request(my->area,width,height);
		if (my->clip_image == FALSE && my->bsp_mode !=BSP_CLIP){
			cairo_rectangle(cr,0,0,width,height);
			cairo_fill(cr);
		}
	}
	pango_cairo_show_layout(cr, layout);
	g_object_unref(layout);

	if (my->click_n == 1 && my->clip_image== TRUE){
		cairo_set_line_width (cr, 2);
		cairo_set_source_rgb (cr, 0, 0, 100/255.);
		cairo_move_to(cr,my->click_x+100.0, my->click_y);
		cairo_arc (cr, my->click_x, my->click_y, 100.0, 0, 2 * G_PI);
		cairo_stroke (cr);
		if (my->pixbuf2 != NULL){
			gdk_cairo_set_source_pixbuf (cr, my->pixbuf2, 0, 0);
			cairo_arc (cr, my->click_x, my->click_y, 100.0, 0, 2 * G_PI);
			cairo_fill (cr);
		}
	}

	
	switch(my->bsp_mode){
			case BSP_OPEN:
				draw_control_polygons_open (cr,&my->curve_infos);
				draw_bezier_polygons_open(cr, &my->curve_infos);
				draw_bezier_curves_open(cr,&my->curve_infos,0.1);
				break;
			case BSP_CLOSE:
				draw_control_polygons_close(cr,&my->curve_infos);
				draw_bezier_polygons_close(cr, &my->curve_infos);
				draw_bezier_curves_close(cr,&my->curve_infos,0.1);
				break;
			case BSP_PROLONG:
				draw_control_polygons_open(cr,&my->curve_infos);
				draw_bezier_polygons_prolong(cr, &my->curve_infos);
				draw_bezier_curves_prolong(cr,&my->curve_infos,0.1);
				break;
			case BSP_FILL:
				draw_control_polygons_close(cr,&my->curve_infos);
				draw_bezier_polygons_close(cr, &my->curve_infos);
				draw_bezier_curves_fill(cr,&my->curve_infos,0.1);
				break;
			case BSP_CLIP:
				draw_control_polygons_close(cr,&my->curve_infos);
				draw_bezier_polygons_close(cr, &my->curve_infos);
				draw_bezier_curves_clip(cr,&my->curve_infos,0.1,my);
				break;
			default:
				printf("%d\n",my->edit_mode);
				break;
		}

	//char msg[200];
	//g_sprintf(msg,"on area draw: %dx%d",gtk_widget_get_allocated_width(my->area),gtk_widget_get_allocated_width(my->area));
	//set_status(my->status,msg);

	draw_shots(cr, my);
	draw_munition(cr, my);
	draw_next_munition(cr, my);
	draw_cannon(cr, my);

	return TRUE;
}

gboolean on_area_key_press (GtkWidget *area, GdkEvent *event, gpointer data){
	Mydata *my = get_mydata(data);

	GdkEventKey *evk = &event->key;
	printf ("%s: GDK_KEY_%s\n",__func__, gdk_keyval_name(evk->keyval));
	switch (evk->keyval) {
	case GDK_KEY_q :
		on_quit_clicked(my->window,my);
		break;
	case GDK_KEY_ampersand:
		set_edit_mode(my,EDIT_ADD_CURVE);
		break;
	case GDK_KEY_1:
		set_bsp_mode(my,BSP_OPEN);
		break;
	case GDK_KEY_eacute:
		set_edit_mode(my,EDIT_MOVE_CURVE);
		break;
	case GDK_KEY_2:
		set_bsp_mode(my,BSP_CLOSE);
		break;
	case GDK_KEY_quotedbl:
		set_edit_mode(my,EDIT_REMOVE_CURVE);
		break;
	case GDK_KEY_3:
		set_bsp_mode(my,BSP_PROLONG);
		break;
	case GDK_KEY_apostrophe:
		set_edit_mode(my,EDIT_ADD_CONTROL);
		break;
	case GDK_KEY_4:
		set_bsp_mode(my,BSP_FILL);
		break;
	case GDK_KEY_parenleft:
		set_edit_mode(my,EDIT_MOVE_CONTROL);
		break;
	case GDK_KEY_5:
		set_bsp_mode(my,BSP_CLIP);
		break;
	case GDK_KEY_minus:
		set_edit_mode(my,EDIT_REMOVE_CONTROL);
		break;
	case GDK_KEY_6:
		break;
	case GDK_KEY_egrave:
		set_edit_mode(my,EDIT_MOVE_CLIP);
		break;
	case  GDK_KEY_7:
		break;
	case GDK_KEY_underscore:
		set_edit_mode(my,EDIT_RESET_CLIP);
		break;
	case GDK_KEY_8:
		break;
	case GDK_KEY_ccedilla:
		break;
	case GDK_KEY_9:
		break;
	case GDK_KEY_agrave:
		break;
	case GDK_KEY_0:
		break;
	case GDK_KEY_parenright:
		break;
	case GDK_KEY_degree:
		break;
	case GDK_KEY_equal:
		break;
	case GDK_KEY_plus:
		break;
	case GDK_KEY_space:
		switch_ammo(my);
		refresh_area(area);
		break;
	}

	return TRUE;
}

gboolean on_area_key_release (GtkWidget *area, GdkEvent *event, gpointer data){
	//Mydata *my = get_mydata(data);

	return TRUE;
}

gboolean on_area_button_press (GtkWidget *area, GdkEvent *event, gpointer data){
	Mydata *my = get_mydata(data);

	Game * game = &my->game;

	GdkEventButton *evb = &event->button;

	my->click_n = evb->button;
	my->click_x = evb->x;
	my->click_y = evb->y;

	if(my->click_n ==1 && my->show_edit == TRUE){
		int tmp;
		switch(my->edit_mode){
			case EDIT_ADD_CURVE:
				tmp=add_curve(&my->curve_infos);
				if (tmp < 0){
					break;
				}
				set_edit_mode(my,EDIT_ADD_CONTROL);
				tmp=add_control(&my->curve_infos, my->click_x , my->click_y);
				if(tmp < 0){
					break;
				}
				break;
			case EDIT_MOVE_CURVE:
				tmp=find_control(&my->curve_infos, my->click_x , my->click_y);
				if(tmp < 0){
					break;
				}
				break;
			case EDIT_REMOVE_CURVE:
				tmp=find_control(&my->curve_infos, my->click_x , my->click_y);
				if(tmp < 0){
					break;
				}
				tmp=remove_curve(&my->curve_infos);
				if(tmp < 0){
					break;
				}
				break;
			case EDIT_ADD_CONTROL:
				tmp=add_control(&my->curve_infos, my->click_x , my->click_y);
				if(tmp < 0){
					break;
				}
				break;
			case EDIT_MOVE_CONTROL:
				tmp=find_control(&my->curve_infos, my->click_x , my->click_y);
				if(tmp < 0){
					break;
				}
				break;
			case EDIT_REMOVE_CONTROL:
				tmp=find_control(&my->curve_infos, my->click_x , my->click_y);
				if(tmp < 0){
					break;
				}
				tmp=remove_control(&my->curve_infos);
				if(tmp < 0){
					break;
				}
				break;
			case EDIT_MOVE_CLIP:
				tmp=find_control(&my->curve_infos, my->click_x , my->click_y);
				if(tmp < 0){
					break;
				}
				break;
			case EDIT_RESET_CLIP:
				tmp=find_control(&my->curve_infos, my->click_x , my->click_y);
				if(tmp < 0){
					break;
				}
				reset_shift(&my->curve_infos, (my->click_x-my->last_x), (my->click_y-my->last_y));
				break;
			default:
				break;	
		}
	}

	if(my->click_n == 1 && game->game_state)
	{
		switch(game->game_state)
		{
			case GS_PLAYING:
				fire(my);
				break;
			default:
				break;
		}
	}
	printf ("%s: %d %.1f %.1f\n", __func__, evb->button, evb->x, evb->y);
	refresh_area(area);

	return TRUE;
}

gboolean on_area_button_release (GtkWidget *area, GdkEvent *event, gpointer data){
	Mydata *my = get_mydata(data);
	my->click_n = 0;
	
	refresh_area(area);
	return TRUE;
}

gboolean on_area_motion_notify (GtkWidget *area, GdkEvent *event, gpointer data){
	Mydata *my = get_mydata(data);
	GdkEventMotion *evm = &event->motion;
	my->last_x = my->click_x;
	my->last_y = my->click_y;

	my->click_x = evm->x;
	my->click_y = evm->y;

	udapte_cannon_angle(my);

	if (my->click_n == 1 && my->show_edit == TRUE){
		int tmp;
		switch(my->edit_mode){
			case EDIT_ADD_CURVE:
				break;
			case EDIT_MOVE_CURVE:
				tmp = move_curve(&my->curve_infos, (my->click_x-my->last_x), (my->click_y-my->last_y));
				if(tmp < 0){
					break;
				}
				break;
			case EDIT_REMOVE_CURVE:
				break;
			case EDIT_ADD_CONTROL:
				break;
			case EDIT_MOVE_CONTROL:
				tmp = move_control(&my->curve_infos, (my->click_x-my->last_x), (my->click_y-my->last_y));
				if(tmp < 0){
					break;
				}
				break;
			case EDIT_REMOVE_CONTROL:
				break;
			case EDIT_MOVE_CLIP:
				tmp=move_curve(&my->curve_infos, (my->click_x-my->last_x), (my->click_y-my->last_y));
				if(tmp < 0){
					break;
				}
				tmp=move_shift(&my->curve_infos, (my->click_x-my->last_x), (my->click_y-my->last_y));
				if(tmp < 0){
					break;
				}
				break;
			case EDIT_RESET_CLIP:

				break;
			default:
				break;	
		}
	}

	refresh_area(area);
	return TRUE;
}

gboolean on_area_enter_notify (GtkWidget *area, GdkEvent *event, gpointer data){
	Mydata *my = get_mydata(data);

	GdkEventCrossing *evc = &event->crossing;
	printf ("%s: %.1f %.1f\n", __func__, evc->x, evc->y);

	gtk_widget_grab_focus (my->area);
	return TRUE;
}

gboolean on_area_leave_notify (GtkWidget *area, GdkEvent *event, gpointer data){
	//Mydata *my = get_mydata(data);

	return TRUE;
}

gboolean on_area_size_allocate(GtkWidget * area, GdkRectangle * rect, gpointer data)
{
	Mydata * my = get_mydata(data);

	my->area_w = rect->width;
	my->area_h = rect->height;

	return TRUE;
}

void area_init(gpointer data){
	Mydata * my = get_mydata(data);

	my->elderscroll = gtk_scrolled_window_new(NULL,NULL);

	my->area = gtk_drawing_area_new ();	
		
	g_signal_connect (my->area, "draw", G_CALLBACK(on_area_draw), my);
	g_signal_connect (my->area, "key-press-event", G_CALLBACK(on_area_key_press), my);
	g_signal_connect (my->area, "key-release-event", G_CALLBACK(on_area_key_release), my);
	g_signal_connect (my->area, "button-press-event", G_CALLBACK(on_area_button_press), my);
	g_signal_connect (my->area, "button-release-event", G_CALLBACK(on_area_button_release), my);
	g_signal_connect (my->area, "motion-notify-event", G_CALLBACK(on_area_motion_notify), my);
	g_signal_connect (my->area, "enter-notify-event", G_CALLBACK(on_area_enter_notify), my);
	g_signal_connect (my->area, "leave-notify-event", G_CALLBACK(on_area_leave_notify), my);
	g_signal_connect (my->area, "size-allocate", G_CALLBACK(on_area_size_allocate), my);

	gtk_widget_add_events (my->area,
	GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |
	GDK_FOCUS_CHANGE_MASK |
	GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
	GDK_POINTER_MOTION_MASK |
	GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK );

	gtk_widget_set_can_focus (my->area, TRUE);

	//g_object_set (gtk_settings_get_default(), "gtk-shell-shows-menubar", FALSE, NULL);
}
