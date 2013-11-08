#ifndef GUI_HPP
	#define GUI_HPP

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <ctime>
#include <cstdlib>

extern GtkWidget *window;
extern GtkWidget *image;
extern GtkWidget *hbox, *vbox;
extern GtkWidget *start_button, *exit_button;
extern GtkWidget *save_button;
extern GdkPixbuf *pixbuf;
extern GtkWidget *head_label, *gen_label, *animal_label, *spin_label, *eat_label;
extern GtkWidget *draw_spin, *eat_spin;

extern long long generation;

void put_pixel (GdkPixbuf *pixbuf, 
				int x, int y, guchar red,
				guchar green, guchar blue, 
				guchar alpha);

static unsigned n = 0;

int rand_init(int seed);
int r_rand(void);

#endif
