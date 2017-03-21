#ifndef GAME_H
#define GAME_H

typedef struct
{
	cairo_surface_t * cannon_sprite;
	double cannon_angle;
}Game;

void game_init();
void udapte_cannon_angle();

#endif