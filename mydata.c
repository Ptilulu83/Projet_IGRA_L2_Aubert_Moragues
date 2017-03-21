#include <gtk/gtk.h>

#include "curve.h"
#include "game.h"
#include "mydata.h"

// Cette fonction permet de tester si le data que l’on a recuper´e dans
// une callback contient bien my ; sinon, cela veut dire que :
// - soit on a oublie de transmettre my dans g_signal_connect,
// - soit on s’est trompé dans le nombre de paramètres de la callback.
Mydata *get_mydata (gpointer data){
	if (data == NULL) {
		fprintf (stderr, "get_mydata: NULL data\n");
		return NULL;
	}
	if (((Mydata *)data)->Magicnumber != MYDATA_MAGIC) {
		fprintf (stderr, "get_mydata: bad magic number\n");
		return NULL;
	}
	return data;
}

void init_mydata(Mydata *my){
	my->title = "Projet de deux abrutis GTK+ 3.0";									//donne le nom de la fennetre
	my->win_width = 800;
	my->win_height = 600;
	my->Magicnumber = MYDATA_MAGIC;
	my->Current_folder = NULL;
	my->pixbuf1 = NULL;
	my->pixbuf2 = NULL;
	my->show_edit=FALSE;
	my->edit_mode=EDIT_ADD_CURVE;
	my->bsp_mode=BSP_OPEN;
	init_curve_infos (&my->curve_infos);
	game_init(&my->game);
}

void set_edit_mode (Mydata * my, int mode){
	if (mode > EDIT_FIRST && mode < EDIT_LAST){
		my->edit_mode=mode ;
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(my->edit_radios[mode]),TRUE);
	}
}

void set_bsp_mode (Mydata * my, int mode){
	if (mode > BSP_FIRST && mode < BSP_LAST){
		my->bsp_mode=mode ;
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(my->bsp_radios[mode]),TRUE);
	}
}