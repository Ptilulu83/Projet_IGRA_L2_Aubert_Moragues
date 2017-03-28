#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>

#include "curve.h"
#include "mydata.h"
#include "game.h"

void game_init(Game * game)
{
	game->cannon_sprite = NULL;
	game->cannon_angle=0;
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