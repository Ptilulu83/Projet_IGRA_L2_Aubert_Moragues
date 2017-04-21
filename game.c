#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "curve.h"
#include "mydata.h"
#include "game.h"

void game_init(Game * game)
{
	game->cannon_sprite = cairo_image_surface_create_from_png("resource/Cannon.png");
	game->cannon_angle=0;
	game->sprite_ball_table = malloc(sizeof(cairo_surface_t *)*NB_TYPE_BILLES);
	for (int i = 0; i < NB_TYPE_BILLES; ++i)
	{
		char tmp[200];
		sprintf(tmp, "resource/bille%d.png", i);
		game->sprite_ball_table[i] =  cairo_image_surface_create_from_png(tmp);
	}

	game->current_shot=rand()%NB_TYPE_BILLES;
	game->next_shot=rand()%NB_TYPE_BILLES;

	game->game_state = GS_PLAYING;

	game->nb_shot_on_screen = 0;

	game->correction_angle=(G_PI/180.)*4;
	game->ball_rotation=0.0;
	game->shot_speed = 1.0;

}

void sample_curve_to_track (Curve *curve, Track *track, double theta)
{
	Control bez_points[4];
	int ind = 0;
	if (curve->control_count < 2) return;
	for (int k = 0; k < curve->control_count+1; k++) {
		if (k<2 || k>curve->control_count-2){
			compute_bezier_points_prolong(curve,k,bez_points);
		}
		else if (k>1 && k<curve->control_count-1){
			compute_bezier_points_open(curve,k-2,bez_points);
		}
		if(k==1){
			sample_bezier_curve (bez_points, theta, track->sample_x, track->sample_y, &ind, SAMPLE_MAX, 1);
		}
		else{
			sample_bezier_curve (bez_points, theta, track->sample_x, track->sample_y, &ind, SAMPLE_MAX, 0);
		}		
	}
	track->sample_count = ind;
}

void convert_all_curve_to_track(gpointer data){
	Mydata *my = get_mydata(data);

	Curve_list * curve_list = &my->curve_infos.curve_list;
	Track_list * track_list = &my->game.track_list;
	track_list->track_count = curve_list->curve_count;
	printf("%d courbe \n", track_list->track_count);
	for (int i = 0; i < curve_list->curve_count; ++i){
		sample_curve_to_track (&curve_list->curves[i], &track_list->tracks[i],SAMPLE_THETA);
	}
}

void udapte_cannon_angle(gpointer data){
	Mydata *my = get_mydata(data);

	int tmpw, tmph;

	gtk_window_get_size(GTK_WINDOW(my->window), &tmpw, &tmph);

	int x_center=tmpw/2;
	int y_center=(tmph-69)/2;

	double vx = my->click_x - x_center;
	double vy = my->click_y - y_center;

	double n = sqrt(vx*vx+vy*vy);
	my->game.cannon_angle=acos(vx/n);
	if (vy/n < 0.){
		my->game.cannon_angle= -my->game.cannon_angle;
	}
}

void fire(gpointer data)
{
	Mydata * my = get_mydata(data);

	Game * game = &my->game;

	if (game->nb_shot_on_screen < SHOT_LIMIT)
	{
		// int centre_x=my->area_w/2;
		// int centre_y=my->area_h/2;

		Game * game = &my->game;

		int sprite_w = cairo_image_surface_get_width(game->cannon_sprite);

		int spriteb_w = cairo_image_surface_get_width(game->sprite_ball_table[0]);
		int spriteb_h = cairo_image_surface_get_height(game->sprite_ball_table[0]);

		double xA = ((sprite_w/2 + 200) * cos(game->cannon_angle - game->correction_angle) - spriteb_w/2)*0.17;
		double yA = ((sprite_w/2 + 200) * sin(game->cannon_angle - game->correction_angle) - spriteb_h/2)*0.17;
		
		game->shot_table[game->nb_shot_on_screen].dirx = cos(game->cannon_angle);
		game->shot_table[game->nb_shot_on_screen].diry = sin(game->cannon_angle);

		game->shot_table[game->nb_shot_on_screen].shot_color = game->current_shot;

		game->shot_table[game->nb_shot_on_screen].x = xA;
		game->shot_table[game->nb_shot_on_screen].y = yA;

		game->nb_shot_on_screen++;
		game->current_shot=game->next_shot;
		game->next_shot=rand()%NB_TYPE_BILLES;
	}
	else
	{
		return;
	}
}

void switch_ammo(gpointer data)
{
	Mydata * my = get_mydata(data);

	Game * game = &my->game;

	int tmp = game->current_shot;
	game->current_shot = game->next_shot;
	game->next_shot = tmp;
}

int remove_shot(Game * game, int n)
{
	if (game->nb_shot_on_screen <= 0)
	{
		return -1;
	}

	memmove (game->shot_table+n,game->shot_table+n+1, sizeof(Shot)*(game->nb_shot_on_screen-1-n));
	game->nb_shot_on_screen--;

	return 0;
}

void process_next_step(gpointer data)
{
	Mydata * my = get_mydata(data);

	Game * game = &my->game;
	game->ball_rotation+=(G_PI/180.)*5;
	if(game->ball_rotation > 360){
		game->ball_rotation-=360;
	}
	int spriteb_w = cairo_image_surface_get_width(game->sprite_ball_table[0]);
	int spriteb_h = cairo_image_surface_get_height(game->sprite_ball_table[0]);

	for (int i = 0; i < game->nb_shot_on_screen; ++i)
	{
		Shot * shot = &game->shot_table[i];

		shot->x += shot->dirx*game->shot_speed;
		shot->y += shot->diry*game->shot_speed;

		if (shot->x < -(my->area_w/2 + spriteb_w*0.2 ) || shot->x > (my->area_w/2 + spriteb_w*0.2 ) || shot->y < -(my->area_h/2 + spriteb_h*0.2 ) || shot->y > (my->area_h/2 + spriteb_h*0.2 ) )
		{
			printf("Désinstanciation du tir %d\n", i);
			int res = remove_shot(game, i);
			if(res == -1 ){

			}
		}
	}
}
