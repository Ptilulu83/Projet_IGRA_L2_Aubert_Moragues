#ifndef GAME_H
#define GAME_H
#define NB_TYPE_BILLES 9
#define SHOT_LIMIT 100

typedef enum
{
	GS_FIRST,
	GS_PLAYING,
	GS_PAUSED,
	GS_WON,
	GS_LOST,
	GS_EDIT,
	GS_LAST
}Game_State;

typedef struct
{
	double x,y;
	double dirx, diry;

	int shot_color;
}Shot;

typedef struct
{
	cairo_surface_t * cannon_sprite;
	cairo_surface_t ** sprite_ball_table;

	Shot shot_table[SHOT_LIMIT];

	int nb_shot_on_screen;

	int current_shot;
	int next_shot;

	Game_State game_state;

	double cannon_angle;
	double shot_speed;

}Game;

void game_init(Game *);
void udapte_cannon_angle();
void fire(gpointer);
void switch_ammo(gpointer);
void process_next_step(gpointer data);

#endif