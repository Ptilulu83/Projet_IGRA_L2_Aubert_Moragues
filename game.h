#ifndef GAME_H
#define GAME_H
#define NB_TYPE_BILLES 9
#define SHOT_LIMIT 1000

#define SHOT_MAX       10
#define SHOT_SPEED      5
#define TRACK_MAX      10
#define MARBLE_MAX    200
#define SAMPLE_MAX   1000 
#define LEVEL_MAX      10
#define SAMPLE_THETA    0.05

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

typedef enum { TS_INTRO, TS_NORMAL, TS_COMBO2, TS_COMBO3} Track_state;


typedef struct
{
	double x,y;
	double dirx, diry;

	int shot_color;
}Shot;

typedef struct {
  double x, y;      // coordonnées centre
  double t;         // paramètre dans l'échantillonnage
  int color;
  int is_combo_end; // ou encore, facteur vitesse et direction ?
  int step_explode;
} Marble;

typedef struct {
  int    sample_count;          // échantillonnage courbe
  double sample_x[SAMPLE_MAX], 
         sample_y[SAMPLE_MAX];
  int marble_count;
  int first_visible;
  Marble marbles[MARBLE_MAX];
  Track_state state;
} Track;

typedef struct {
  int track_count;
  Track tracks[TRACK_MAX];
} Track_list;


typedef struct {
  Curve_infos curve_infos;
  double canon_x, canon_y;
  int marbles_intro, marbles_total;
} Level;

typedef struct {
  int level_count;
  Level levels[LEVEL_MAX];
} Level_list;

typedef struct
{
	Game_State game_state;
	int score;
	
	cairo_surface_t * cannon_sprite;
	cairo_surface_t ** sprite_ball_table;

	Shot shot_table[SHOT_LIMIT];
	int nb_shot_on_screen;

	

	int current_shot;
	int next_shot;
	double cannon_angle;
	double correction_angle;
	double shot_speed;

}Game;

void game_init(Game *);
void udapte_cannon_angle(gpointer);
void fire(gpointer);
void switch_ammo(gpointer);
int remove_shot(Game *, int);
void process_next_step(gpointer data);

#endif