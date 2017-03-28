#ifndef GAME_H
#define GAME_H
#define NB_TYPE_BILLES 9

typedef enum
{
	GS_FIRST,
	GS_PLAYING,
	GS_PAUSED,
	GS_WIN,
	GS_LOSE,
	GS_LAST
}Game_State;

typedef struct
{
	cairo_surface_t * cannon_sprite;
	cairo_surface_t ** sprite_ball_table;

	int current_shot;
	int next_shot;

	double cannon_angle;

}Game;

void game_init(Game *);
void udapte_cannon_angle();
void fire();

#endif