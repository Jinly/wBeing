// STL
#include <iostream>
#include <fstream>
#include <vector>

// libc
#include <cstdlib>
#include <ctime>

//libgtk
#include <gtk/gtk.h>
#include <gdk/gdk.h>

// headers
#include <gui.hpp>
#include <world.hpp>
#include <main.hpp>

//======================================================================
long long generation = 0;

GtkWidget *window;
GtkWidget *image;
GtkWidget *hbox, *vbox;
GtkWidget *start_button, *exit_button;
GtkWidget *save_button;
GdkPixbuf *pixbuf;
GtkWidget *gen_label, *animal_label, *spin_label, *eat_label;
GtkWidget *grow_label;
GtkWidget *draw_spin, *eat_spin, *grow_spin;

//====================== callbacks =====================================
void exit_callback(GtkWidget *widget, gpointer data) {
	gtk_main_quit();
}

//======================= stepper ======================================
// тут основная работа
gboolean step(gpointer data) {
	World &world = *(World*)data;
	char str[32];
	sprintf(str, "Generation : %d", ++generation);
	gtk_label_set_text(GTK_LABEL(gen_label), str);
	
	sprintf(str, "Animals : %d", world.animals.size());
	gtk_label_set_text(GTK_LABEL(animal_label), str);
	
	world.AddGrass(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(eat_spin)));
	
	if (generation % gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(grow_spin)) == 0)
		world.GrowGrass();
	
	world.Step();
	
	if (generation % gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(draw_spin)) == 0) {
		world.Draw(pixbuf);
		gtk_widget_queue_draw(image);
	}
	
	return true;
}

// мутит шагалку
void start_callback(GtkWidget *widget, gpointer data) {
	static gint tag;
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		tag = gtk_idle_add(step, data);
	} else {
		gtk_idle_remove(tag);
	}
}

//==================== МАИН ============================================
int main(int argc, char **argv) {
	gtk_init(&argc, &argv);// инициализация gtk
	rand_init(); // инициализация rand
	
	World myWorld(256, 256);
	
	// мутим виджеты
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //окошко
	hbox = gtk_hbox_new(false, 0); // боксы
	vbox = gtk_vbox_new(false, 0);
	start_button = gtk_toggle_button_new_with_label("start"); // кнопка запуска
	save_button = gtk_button_new_with_label("   save\nbest animal"); // сейв
	exit_button = gtk_button_new_with_label("exit"); // выхид
	pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,
							true,
							8,
							512,
							512); // буфффер
	image = gtk_image_new_from_pixbuf(pixbuf); // сюда рисуется буфффер
	gen_label = gtk_label_new("Generation : 0"); // поколение:
	animal_label = gtk_label_new("Animals : 0"); // жывотных:
	GtkObject *adj = gtk_adjustment_new(1,
										1, 100000000,
										1, 1,
										0);
	draw_spin = gtk_spin_button_new(GTK_ADJUSTMENT(adj), 0.5, 0);
	
	adj = gtk_adjustment_new(	4,
								0, 100000000,
								1, 1,
								0);
	eat_spin = gtk_spin_button_new(GTK_ADJUSTMENT(adj), 0.5, 0);
	
	adj = gtk_adjustment_new(	32,
								1, 100000000,
								1, 1,
								0);
	grow_spin = gtk_spin_button_new(GTK_ADJUSTMENT(adj), 0.5, 0);
	
	spin_label = gtk_label_new("draw every X\ngeneration:");
	eat_label = gtk_label_new("add X grass\nevery generation:");
	grow_label = gtk_label_new("grow grass every\nX generation:");
	
	// привязываем их
	gtk_container_add(GTK_CONTAINER(window), hbox);
	gtk_container_add(GTK_CONTAINER(hbox), image);
	gtk_container_add(GTK_CONTAINER(hbox), vbox);
	gtk_container_add(GTK_CONTAINER(vbox), gen_label);
	gtk_container_add(GTK_CONTAINER(vbox), animal_label);
	gtk_container_add(GTK_CONTAINER(vbox), spin_label);
	gtk_container_add(GTK_CONTAINER(vbox), draw_spin);
	gtk_container_add(GTK_CONTAINER(vbox), eat_label);
	gtk_container_add(GTK_CONTAINER(vbox), eat_spin);
	gtk_container_add(GTK_CONTAINER(vbox), grow_label);
	gtk_container_add(GTK_CONTAINER(vbox), grow_spin);
	gtk_container_add(GTK_CONTAINER(vbox), start_button);
	gtk_container_add(GTK_CONTAINER(vbox), save_button);
	gtk_container_add(GTK_CONTAINER(vbox), exit_button);
	
	// сигналы
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(exit_callback), NULL);
	g_signal_connect(G_OBJECT(exit_button), "clicked", G_CALLBACK(exit_callback), NULL);
	g_signal_connect(G_OBJECT(start_button), "clicked", G_CALLBACK(start_callback), (gpointer) &myWorld);
	
	myWorld.Draw(pixbuf);
	
	//ни шагу назад
	gtk_widget_show_all(window);
	gtk_main();
}
