#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <time.h>

#include "curve.h"
#include "util.h"
#include "mydata.h"
#include "drawings.h"
#include "menu.h"
#include "gui.h"

void on_app_activate (GtkApplication * app, gpointer user_data){
	Mydata * my = get_mydata(user_data);				// Récupération des données
	
	window_init(app,my);

	menus_init(my);

	area_init(my);

	status_init(my);

	editing_init(my);

	layout_init(my);

	win_scale_init(my);

	gtk_widget_show_all (my->window);

	gtk_widget_hide(my->frame);
}

int main (int argc, char *argv[]){
	Mydata my;
	init_mydata(&my);
	
	srand(time(NULL));

	GtkApplication *app;
	int status;

	app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);	
	g_signal_connect (app, "activate", G_CALLBACK(on_app_activate), &my);

	status = g_application_run (G_APPLICATION(app), argc, argv);
	g_object_unref (app);

	return status;
}	