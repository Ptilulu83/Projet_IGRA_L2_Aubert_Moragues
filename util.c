#include <gtk/gtk.h>

#include "util.h"

void set_status(GtkWidget *status ,const char *msg){
	gtk_statusbar_pop((GtkStatusbar *) status, 0);
	gtk_statusbar_push((GtkStatusbar *) status, 0, msg);
}

void refresh_area (GtkWidget *area){
	GdkWindow *win = gtk_widget_get_window (area);
	if (win == NULL) return;  // widget pas encore realise
	gdk_window_invalidate_rect (
		win, NULL,   // tout le window
		FALSE); // pas de sous-window
}