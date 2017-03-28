#ifndef GAME_H
#define GAME_H

typedef struct
{
	cairo_surface_t * cannon_sprite;
	cairo_surface_t * sprite_ball_table;

	double cannon_angle;
	
}Game;

void game_init();
void udapte_cannon_angle();

#endif