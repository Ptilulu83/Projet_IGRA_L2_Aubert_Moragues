#include <gtk/gtk.h>
#include <glib/gprintf.h>

#include "curve.h"
#include "mydata.h"
#include "util.h"
#include "drawings.h"
#include "menu.h"
#include "gui.h"

void on_scale_value_change(GtkRange * range, gpointer data){
	Mydata * my = get_mydata(data);

	gdouble new_value = gtk_range_get_value (range);
	if (my->pixbuf1!=NULL && my->scale_value != new_value){
		my->scale_value=new_value;
		char msg[200];
		g_sprintf(msg," Scale : %.2f ",my->scale_value);
		set_status(my->status,msg);
		update_area_with_transforms (my);
	}
}

void on_edit_radio_toggled (GtkToggleButton * button, gpointer data){
	Mydata *my = get_mydata(data);

	if (gtk_toggle_button_get_active(button)==TRUE){
		my->edit_mode = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button),"mode"));
		switch(my->edit_mode){
			case 1:
				printf("EDIT_ADD_CURVE\n");
				break;
			case 2:
				printf("EDIT_MOVE_CURVE\n");
				break;
			case 3:
				printf("EDIT_REMOVE_CURVE\n");
				break;
			case 4:
				printf("EDIT_ADD_CONTROL\n");
				break;
			case 5:
				printf("EDIT_MOVE_CONTROL\n");
				break;
			case 6:
				printf("EDIT_REMOVE_CONTROL\n");
				break;
			default:
				printf("%d\n",my->edit_mode);
				break;
		}
	}
}

void on_bsp_radio_toggled (GtkToggleButton * button, gpointer data){
	Mydata *my = get_mydata(data);

	if (gtk_toggle_button_get_active(button)==TRUE){
		my->bsp_mode = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button),"mode"));
		refresh_area(my->area);
		switch(my->bsp_mode){
			case 1:
				printf("BSP_OPEN\n");
				break;
			case 2:
				printf("BSP_CLOSE\n");
				break;
			case 3:
				printf("BSP_PROLONG\n");
				break;
			default:
				printf("%d\n",my->edit_mode);
				break;
		}
	}
}

void window_init(GtkApplication * app,gpointer data){
	Mydata * my = get_mydata(data);

	my->window = gtk_application_window_new (app);

	gtk_window_set_title (GTK_WINDOW (my->window), my->title);
	gtk_window_set_default_size (GTK_WINDOW (my->window), my->win_width, my->win_height);
}

void layout_init(gpointer data){
	Mydata * my = get_mydata(data);

	my->vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
	my->hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_container_add (GTK_CONTAINER (my->window), my->vbox);

		gtk_box_pack_start(GTK_BOX (my->vbox), my->menu_bar, FALSE, TRUE, 0);
		gtk_box_pack_start (GTK_BOX (my->vbox), my->hbox, TRUE, TRUE, 0);
			gtk_container_add(GTK_CONTAINER(my->hbox),my->frame);
			gtk_box_pack_start (GTK_BOX (my->hbox), my->elderscroll, TRUE, TRUE, 0);
				gtk_container_add (GTK_CONTAINER (my->elderscroll), my->area);
		gtk_box_pack_start (GTK_BOX (my->vbox), my->status, FALSE, FALSE, 0);
}

void status_init(gpointer data){
	Mydata * my = get_mydata(data);

	my->status = gtk_statusbar_new();														// Instancie la status bar
	set_status (my->status, "Welcome");
}

void editing_init(gpointer data){
	Mydata * my = get_mydata(data);

	my->frame = gtk_frame_new("Editing");
	GtkWidget *vbox2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
	gtk_container_add(GTK_CONTAINER(my->frame), vbox2);

	char * noms1[8] = {"Add curve","Move curve","Remove curve","Add control","Move control","Remove control","Move clip","Reset clip"};
	GtkWidget *radios[8];
	for (int i = 0; i < 8; i++) {
		radios[i] = gtk_radio_button_new_with_label_from_widget (
			i == 0 ? NULL : GTK_RADIO_BUTTON(radios[0]),noms1[i]);

		my->edit_radios[i+1]=radios[i];

		g_object_set_data(G_OBJECT(radios[i]),"mode",GINT_TO_POINTER(EDIT_ADD_CURVE+i));
		gtk_box_pack_start (GTK_BOX (vbox2),radios[i], FALSE, TRUE, 0);
		g_signal_connect (radios[i], "toggled",G_CALLBACK(on_edit_radio_toggled), my);
	}

	GtkWidget * separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_box_pack_start(GTK_BOX(vbox2),separator,FALSE ,TRUE ,0);

	char * noms2[5] = {"Open","Close","Prolongated","Fill","Clip Image"};
	GtkWidget *bsp[5];
	for (int i = 0; i < 5; i++) {
		bsp[i] = gtk_radio_button_new_with_label_from_widget (
			i == 0 ? NULL : GTK_RADIO_BUTTON(bsp[0]),noms2[i]);

		my->bsp_radios[i+1]=bsp[i];

		g_object_set_data(G_OBJECT(bsp[i]),"mode",GINT_TO_POINTER(EDIT_ADD_CURVE+i));
		gtk_box_pack_start (GTK_BOX (vbox2),bsp[i], FALSE, TRUE, 0);
		g_signal_connect (bsp[i], "toggled",G_CALLBACK(on_bsp_radio_toggled), my);
	}
}

void win_scale_init(gpointer data){
	Mydata * my = get_mydata(data);

	my->win_scale = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(my->win_scale),"Image Scale");
	gtk_window_set_default_size (GTK_WINDOW (my->win_scale), 300, 100);
	g_signal_connect (my->win_scale, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), my);

	GtkWidget *box_scale , *label;

	box_scale = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_container_add (GTK_CONTAINER (my->win_scale), box_scale);

	label = gtk_label_new("Scale :"); 
	gtk_box_pack_start(GTK_BOX(box_scale),label, FALSE ,TRUE,10);

	my->scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.02,20.0,0.02);
	gtk_range_set_value(GTK_RANGE(my->scale),1.0);
	my->scale_value=gtk_range_get_value(GTK_RANGE(my->scale));
	gtk_box_pack_start(GTK_BOX(box_scale),my->scale, TRUE ,TRUE,10);
	g_signal_connect(my->scale, "value-changed",G_CALLBACK(on_scale_value_change),my);

	gtk_widget_show_all(my->win_scale);

	gtk_widget_hide(my->win_scale);
}
