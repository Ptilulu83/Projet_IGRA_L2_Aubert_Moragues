#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "curve.h"
#include "mydata.h"
#include "util.h"
#include "drawings.h"
#include "menu.h"
#include "gui.h"
#include "game.h"

void on_save_clicked (GtkWidget *widget, gpointer data){
	Mydata *my = get_mydata(data);

	my->scale_value=1.0;			//
	g_object_unref(my->scale);		//
	win_scale_init(my);				//

	printf ("load file \n");
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new (	"select image",(GtkWindow *) my->window,action,
											"Cancel",GTK_RESPONSE_CANCEL,
											"Open", GTK_RESPONSE_ACCEPT,
											NULL);

	if (my->Current_folder != NULL){
		gtk_file_chooser_set_current_folder((GtkFileChooser *) dialog , my->Current_folder);
	}
	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT){
	    char *filename;
	    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	    filename = gtk_file_chooser_get_filename (chooser);
	    set_status(my->status, "Loading Image...");
	    g_clear_object(&my->pixbuf1);
		my->pixbuf1=gdk_pixbuf_new_from_file(filename,NULL);

		//gtk_range_set_value(GTK_RANGE(my->win_scale),my->scale_value);

		if(my->pixbuf1 == NULL)
		{
			set_status(my->status, "Loading Failed : not an image");
			// gtk_image_set_from_icon_name(GTK_IMAGE(my->image),"image-missing", GTK_ICON_SIZE_DIALOG);
		}
		else
		{
			char msg[200];			
			g_sprintf(msg, "Loading successful : %dx%d", gdk_pixbuf_get_width(my->pixbuf1), gdk_pixbuf_get_height(my->pixbuf1));
			set_status(my->status, msg);
			// gtk_image_set_from_pixbuf (GTK_IMAGE(my->image), my->pixbuf1);
			//gtk_image_set_from_file (GTK_IMAGE(my->image),filename);
		}
		g_free (my->Current_folder);
		my->Current_folder=gtk_file_chooser_get_current_folder(chooser);
		g_free (filename);

		my->scale_value = 1.0f;
		my->rotate_angle = 0.0f;
		update_area_with_transforms(my);
		}
	gtk_widget_destroy (dialog);
}

void on_load_clicked (GtkWidget *widget, gpointer data){
	Mydata *my = get_mydata(data);

	my->scale_value=1.0;			//
	g_object_unref(my->scale);		//
	win_scale_init(my);				//

	printf ("load file \n");
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new (	"select image",(GtkWindow *) my->window,action,
											"Cancel",GTK_RESPONSE_CANCEL,
											"Open", GTK_RESPONSE_ACCEPT,
											NULL);

	if (my->Current_folder != NULL){
		gtk_file_chooser_set_current_folder((GtkFileChooser *) dialog , my->Current_folder);
	}
	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT){
	    char *filename;
	    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	    filename = gtk_file_chooser_get_filename (chooser);
	    set_status(my->status, "Loading Image...");
	    g_clear_object(&my->pixbuf1);
		my->pixbuf1=gdk_pixbuf_new_from_file(filename,NULL);

		//gtk_range_set_value(GTK_RANGE(my->win_scale),my->scale_value);

		if(my->pixbuf1 == NULL)
		{
			set_status(my->status, "Loading Failed : not an image");
			// gtk_image_set_from_icon_name(GTK_IMAGE(my->image),"image-missing", GTK_ICON_SIZE_DIALOG);
		}
		else
		{
			char msg[200];			
			g_sprintf(msg, "Loading successful : %dx%d", gdk_pixbuf_get_width(my->pixbuf1), gdk_pixbuf_get_height(my->pixbuf1));
			set_status(my->status, msg);
			// gtk_image_set_from_pixbuf (GTK_IMAGE(my->image), my->pixbuf1);
			//gtk_image_set_from_file (GTK_IMAGE(my->image),filename);
		}
		g_free (my->Current_folder);
		my->Current_folder=gtk_file_chooser_get_current_folder(chooser);
		g_free (filename);

		my->scale_value = 1.0f;
		my->rotate_angle = 0.0f;
		update_area_with_transforms(my);
		}
	gtk_widget_destroy (dialog);
}

void on_quit_clicked (GtkWidget *widget, gpointer data){
	Mydata *my = get_mydata(data);

	printf ("\nClosing window and app\n");
	gtk_widget_destroy(my->window);								//kill la fenetre et l'application
}

void on_rotate_right_clicked (GtkWidget * widget, gpointer data){
	Mydata *my = get_mydata(data);

	if(my->pixbuf1 != NULL){
		my->rotate_angle += 270.0f;
		while ( my->rotate_angle >= 360.0f ){
			my->rotate_angle -= 360.0f;
		}
		set_status(my->status,"Image rotated (right)");
		update_area_with_transforms (my);
	}
	else{
		set_status(my->status,"No image to rotate");
	}
}

void on_rotate_left_clicked (GtkWidget * widget, gpointer data){
	Mydata *my = get_mydata(data);
	if(my->pixbuf1 != NULL){
		my->rotate_angle += 90.0f;
		while ( my->rotate_angle >= 360.0f ){
			my->rotate_angle -= 360.0f;
		}
		set_status(my->status,"Image rotated (left)");
		update_area_with_transforms (my);
	}
	else{
		set_status(my->status,"No image to rotate");
	}
}

void on_BG_color_clicked (GtkWidget * widget, gpointer data){
	Mydata * my = get_mydata(data);

	GtkWidget *	BG_color_window;
	GdkRGBA bg_color;
	char msg[200];
	gint test;

	BG_color_window=gtk_color_chooser_dialog_new("Background color",NULL);

	test=gtk_dialog_run(GTK_DIALOG(BG_color_window));
	if (test==GTK_RESPONSE_OK){
		gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(BG_color_window),&bg_color);
		g_sprintf(msg, "Selected BG color : %s",gdk_rgba_to_string(&bg_color));
		set_status(my->status,msg);
		gtk_widget_override_background_color(my->area, GTK_STATE_FLAG_NORMAL, &bg_color);
	}
	gtk_widget_destroy(BG_color_window);
}

void on_scale_activate(GtkWidget * widget, gpointer data){
	Mydata * my = get_mydata(data);

	gtk_window_present(GTK_WINDOW(my->win_scale));
}

void on_clip_activate (GtkCheckMenuItem *widget, gpointer data){
	Mydata *my = get_mydata(data);
	
	my->clip_image = gtk_check_menu_item_get_active (widget);
	if (my->clip_image == TRUE)
		set_status(my->status, "Clipping is on");
	else
		set_status(my->status, "Clipping is off");
		
	refresh_area (my->area);
}

void on_editing_activate (GtkCheckMenuItem *widget, gpointer data){
	Mydata *my = get_mydata(data);
	
	my->show_edit = gtk_check_menu_item_get_active (widget);
	if (my->show_edit == TRUE){
		set_status(my->status, "Editing is on");
		gtk_widget_show(my->frame);
		my->game.game_state=GS_EDIT;
	}
	else {
		set_status(my->status, "Editing is off");
		gtk_widget_hide(my->frame);
		my->game.game_state=GS_PLAYING;
	}
}

void on_about_clicked (GtkWidget * widget, gpointer data){
	Mydata * my = get_mydata(data);

	char * auteurs[]={"Lucas Moragues <lucas.moragues@wanadoo.fr>","Gabriel Aubert <uzumakigip@gmail.com>",NULL};

	gtk_show_about_dialog(NULL,
						 "program-name",my->title,
						 "version","2.4",
						 "website","<http://j.mp/optigra",
						 "authors",auteurs,
						 "logo-icon-name","face-cool",
						 NULL);
}

void menus_init(gpointer data){
	Mydata * my = get_mydata(data);
	GtkWidget *item_file, *item_tools , *item_help ;
	GtkWidget *sub_file, *save, *load, *quit;
	GtkWidget *sub_tools, *BG_color, *rotate_right, *rotate_left, *scale, * clip , *editing;
	GtkWidget *sub_help, *about;

	my->menu_bar =gtk_menu_bar_new();

		item_file = gtk_menu_item_new_with_label("File");
		gtk_menu_shell_append(GTK_MENU_SHELL (my->menu_bar),item_file);

			sub_file =gtk_menu_new();
			gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_file),sub_file);

				save = gtk_menu_item_new_with_label("Save");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_file),save);
				g_signal_connect (save, "activate", G_CALLBACK(on_save_clicked), my);

				load = gtk_menu_item_new_with_label("Load");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_file),load);
				g_signal_connect (load, "activate", G_CALLBACK(on_load_clicked), my);

				quit = gtk_menu_item_new_with_label("Quit");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_file),quit);
				g_signal_connect (quit, "activate", G_CALLBACK(on_quit_clicked), my);

		item_tools = gtk_menu_item_new_with_label("Tools");
		gtk_menu_shell_append(GTK_MENU_SHELL (my->menu_bar),item_tools);

			sub_tools =gtk_menu_new();
			gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_tools),sub_tools);

				rotate_right = gtk_menu_item_new_with_label("Rotate right");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_tools),rotate_right);
				g_signal_connect (rotate_right, "activate", G_CALLBACK(on_rotate_right_clicked), my);

				rotate_left = gtk_menu_item_new_with_label("Rotate left");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_tools),rotate_left);
				g_signal_connect (rotate_left, "activate", G_CALLBACK(on_rotate_left_clicked), my);

				BG_color = gtk_menu_item_new_with_label("BG color");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_tools),BG_color);
				g_signal_connect (BG_color, "activate", G_CALLBACK(on_BG_color_clicked), my);

				scale = gtk_menu_item_new_with_label("Scale");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_tools),scale);
				g_signal_connect (scale, "activate", G_CALLBACK(on_scale_activate), my);

				clip= gtk_check_menu_item_new_with_label("Clip");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_tools),clip);
				g_signal_connect (clip, "activate", G_CALLBACK(on_clip_activate), my);

				editing= gtk_check_menu_item_new_with_label("Editing");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_tools),editing);
				g_signal_connect (editing, "activate", G_CALLBACK(on_editing_activate), my);


		item_help = gtk_menu_item_new_with_label("Help !");
		gtk_menu_shell_append(GTK_MENU_SHELL (my->menu_bar),item_help);

			sub_help =gtk_menu_new();
			gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_help),sub_help);

				about = gtk_menu_item_new_with_label("About ?");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_help),about);
				g_signal_connect (about, "activate", G_CALLBACK(on_about_clicked), my);

	g_object_set (gtk_settings_get_default(),"gtk-shell-shows-menubar", FALSE , NULL);
}