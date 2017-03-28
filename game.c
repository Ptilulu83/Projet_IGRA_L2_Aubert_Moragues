#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
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

	game->shot_speed = 5.0;
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

	if (game->nb_shot_on_screen <= SHOT_LIMIT)
	{
		int tmpw, tmph;

		gtk_window_get_size(GTK_WINDOW(my->window), &tmpw, &tmph);

		int centre_x=tmpw/2;
		int centre_y=(tmph-70)/2;
		

		int sprite_w = cairo_image_surface_get_width(game->cannon_sprite);
		int sprite_h = cairo_image_surface_get_height(game->cannon_sprite);

		double xA = centre_x + cos(game->cannon_angle)*(sprite_w + 10)+ sin(game->cannon_angle)*(sprite_h/2 - 5) , yA = centre_y + sin(game->cannon_angle)*(sprite_w + 10)- cos(game->cannon_angle)*(sprite_h/2 - 5);
		//+ cos(game->cannon_angle)*(sprite_w + 10),- sin(game->cannon_angle)*(sprite_h/2 - 5)
		game->shot_table[game->nb_shot_on_screen].dirx = cos(game->cannon_angle);
		game->shot_table[game->nb_shot_on_screen].diry = sin(game->cannon_angle);

		game->shot_table[game->nb_shot_on_screen].shot_color = game->current_shot;
		

		game->shot_table[game->nb_shot_on_screen].x = xA;
		game->shot_table[game->nb_shot_on_screen].y = yA;

		game->nb_shot_on_screen++;
		game->current_shot=game->next_shot;
		game->next_shot=rand()%NB_TYPE_BILLES;
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

void process_next_step(gpointer data)
{
	Mydata * my = get_mydata(data);

	Game * game = &my->game;

	for (int i = 0; i < game->nb_shot_on_screen; ++i)
	{
		Shot * shot = &game->shot_table[i];

		shot->x += shot->dirx*game->shot_speed;
		shot->y += shot->diry*game->shot_speed;
	}
}
