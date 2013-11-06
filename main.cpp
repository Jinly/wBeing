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

void put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha) {
	int width, height, rowstride, n_channels;
	guchar *pixels, *p;

	n_channels = gdk_pixbuf_get_n_channels (pixbuf);

	//g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
	//g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
	//g_assert (gdk_pixbuf_get_has_alpha (pixbuf));
	//g_assert (n_channels == 4);

	width = gdk_pixbuf_get_width (pixbuf);
	height = gdk_pixbuf_get_height (pixbuf);

	//g_assert (x >= 0 && x < width);
	//g_assert (y >= 0 && y < height);

	rowstride = gdk_pixbuf_get_rowstride (pixbuf);
	pixels = gdk_pixbuf_get_pixels (pixbuf);

	p = pixels + y * rowstride + x * n_channels;
	p[0] = red;
	p[1] = green;
	p[2] = blue;
	p[3] = alpha;
}

//======================================================================
long long generation = 0;

GtkWidget *window;
GtkWidget *image;
GtkWidget *hbox, *vbox;
GtkWidget *start_button, *exit_button;
GtkWidget *save_button;
GdkPixbuf *pixbuf;
GtkWidget *head_label, *gen_label, *animal_label, *spin_label, *eat_label;
GtkWidget *draw_spin, *eat_spin;

class Animal;

enum cellType {
	EMPTY = 0,
	GRASS,
	STONE,
	MEAT,
	ANIMAL,
	EGG
};

enum direction {
	UP = 0,
	DOWN,
	LEFT,
	RIGHT, 
	NONE
};
//========================= Rand =======================================
static unsigned n = 0;
int rand_init(int seed) {
	n = 0;
	srand(seed);
}
int r_rand(void) {
	if (n >= RAND_MAX)
		srand(time(NULL));
	return rand();
}
//========================= Cell =======================================
class Cell {
	public:
		cellType type;
		Animal *animal;
};

//======================== World =======================================
class World {
	public:
		World();
		void DrawCell(int x, int y, GdkPixbuf *pixbuf);
		void Draw(GdkPixbuf *pixbuf);
		void AddGrass(int n);
		Cell sells[256][256];		
};

void World::DrawCell(int x, int y, GdkPixbuf *pixbuf) {
	switch (sells[x][y].type) {
		case EMPTY:
			put_pixel(pixbuf, x*2, y*2, 0, 0, 0, 255);
			put_pixel(pixbuf, x*2+1, y*2, 0, 0, 0, 255);
			put_pixel(pixbuf, x*2, y*2+1, 0, 0, 0, 255);
			put_pixel(pixbuf, x*2+1, y*2+1, 0, 0, 0, 255);
			break;
		case GRASS:
			put_pixel(pixbuf, x*2, y*2, 0, 255, 0, 255);
			put_pixel(pixbuf, x*2+1, y*2, 0, 255, 0, 255);
			put_pixel(pixbuf, x*2, y*2+1, 0, 255, 0, 255);
			put_pixel(pixbuf, x*2+1, y*2+1, 0, 255, 0, 255);
			break;
		case STONE:
			put_pixel(pixbuf, x*2, y*2, 255, 0, 0, 255);
			put_pixel(pixbuf, x*2+1, y*2, 255, 0, 0, 255);
			put_pixel(pixbuf, x*2, y*2+1, 255, 0, 0, 255);
			put_pixel(pixbuf, x*2+1, y*2+1, 255, 0, 0, 255);
			break;
		case MEAT:
			put_pixel(pixbuf, x*2, y*2, 255, 128, 128, 255);
			put_pixel(pixbuf, x*2+1, y*2, 255, 128, 128, 255);
			put_pixel(pixbuf, x*2, y*2+1, 255, 128, 128, 255);
			put_pixel(pixbuf, x*2+1, y*2+1, 255, 128, 128, 255);
			break;
		case ANIMAL:
			put_pixel(pixbuf, x*2, y*2, 0, 0, 255, 255);
			put_pixel(pixbuf, x*2+1, y*2, 0, 0, 255, 255);
			put_pixel(pixbuf, x*2, y*2+1, 0, 0, 255, 255);
			put_pixel(pixbuf, x*2+1, y*2+1, 0, 0, 255, 255);
			break;
		case EGG:
			put_pixel(pixbuf, x*2, y*2, 255, 255, 0, 255);
			put_pixel(pixbuf, x*2+1, y*2, 255, 255, 0, 255);
			put_pixel(pixbuf, x*2, y*2+1, 255, 255, 0, 255);
			put_pixel(pixbuf, x*2+1, y*2+1, 255, 255, 0, 255);
			break;
	}
}

void World::Draw(GdkPixbuf *pixbuf) {
	for (int x = 0; x < 256; x++)
		for (int y = 0; y < 256; y++) {
			DrawCell(x, y, pixbuf);
		}
	gtk_widget_queue_draw(image);
}

void World::AddGrass(int n) {
	for (int i = 0; i < n; i++) {
		int x = r_rand() % 256;
		int y = r_rand() % 256;
		if (sells[x][y].type == EMPTY)
			sells[x][y].type = GRASS;
	}
}

//========================= Animal =====================================
class Brain;
class Animal {
	public:
		Animal(int _x, int _y, std::vector<Animal> *selfvector, Animal *_last = NULL);
		~Animal();
		void Step(World &world);
		void Go(World &world, direction direct);
		void Eat(World &world, direction direct);
		void Kill(World &world);
		void Out(World &world);
		unsigned x, y;
		double fill;
		std::vector<Animal> *selfvector;
		Brain *selfbrain;
		direction last;
};

//========================= Brain ======================================
class Brain {
	public:
		Brain(Brain *last = NULL);
		~Brain();
		void Mutate(void);
};

Brain::Brain(Brain *last) {
	
}

Brain::~Brain() {
	
}

Animal::Animal(int _x, int _y, std::vector<Animal> *_selfvector, Animal *_last) {
	x = _x; y = _y;
	fill = 100.0;
	selfvector = _selfvector;
	last = UP;
	if (last)
		selfbrain = new Brain(_last->selfbrain);
	else
		selfbrain = new Brain(NULL);
}

Animal::~Animal() {
}

// идтити
void Animal::Go(World &world, direction direct) {
	int rx, ry;
	switch (direct) {
		case UP:
			rx = 0, ry = 1;
			break;
		case DOWN:
			rx = 0, ry = -1;
			break;
		case LEFT:
			rx = -1; ry = 0;
			break;
		case RIGHT:
			rx = 1; ry = 0;
			break;
	} 
	if ((world.sells[x+rx][y+ry].type == STONE) || (world.sells[x+rx][y+ry].type == ANIMAL) || (fill <= 0.5))
		return;
	world.sells[x+rx][y+ry].type = ANIMAL;
	world.sells[x+rx][y+ry].animal = world.sells[x][y].animal;
	world.sells[x][y].type = EMPTY;
	world.sells[x][y].animal = NULL;
	x += rx;
	y += ry;
	fill -= 0.5;
}

// йести
void Animal::Eat(World &world, direction direct) {
	int rx, ry;
	switch (direct) {
		case UP:
			rx = 0, ry = 1;
			break;
		case DOWN:
			rx = 0, ry = -1;
			break;
		case LEFT:
			rx = -1; ry = 0;
			break;
		case RIGHT:
			rx = 1; ry = 0;
			break;
	} 
	if ((world.sells[x+rx][y+ry].type == STONE) || (world.sells[x+rx][y+ry].type == ANIMAL) || (fill <= 0.5))
		return;
	if ((world.sells[x+rx][y+ry].type == GRASS) ||
		(world.sells[x+rx][y+ry].type == MEAT) ||
		(world.sells[x+rx][y+ry].type == EGG))
		fill += 20.0;
	world.sells[x+rx][y+ry].type = ANIMAL;
	world.sells[x+rx][y+ry].animal = world.sells[x][y].animal;
	world.sells[x][y].type = EMPTY;
	world.sells[x][y].animal = NULL;
	x += rx;
	y += ry;
	fill -= 0.5;
}

std::vector<Animal> animals;

// шоб ты здохла
void Animal::Kill(World &world) {
	world.sells[x][y].type = EMPTY;
	world.sells[x][y].animal = NULL;
	for (size_t i = 0; i < selfvector->size(); i++)
		if (&((*selfvector)[i]) == this) {
			selfvector->erase(selfvector->begin()+i);
			return;
		}
}

// да родит коровенка теленка
void Animal::Out(World &world) {
	if (fill <= 100)
		return;
	direction direct = NONE;
	int ox, oy;
	if (world.sells[x][y+1].type == EMPTY)
		direct = UP, ox = x, oy = y+1;
	if (world.sells[x][y-1].type == EMPTY)
		direct = DOWN, ox = x, oy = y-1;
	if (world.sells[x+1][y].type == EMPTY)
		direct = RIGHT, ox = x+1, oy = y;
	if (world.sells[x-1][y].type == EMPTY)
		direct = LEFT, ox = x-1, oy = y;
	if (direct == NONE)
		return;
	world.sells[ox][oy].type = ANIMAL;
	animals.push_back(Animal(ox, oy, &animals));
	world.sells[ox][oy].animal = &animals[animals.size()-1];
	fill -= 150.0;
}

// живтоне буде делайт енто 
void Animal::Step(World &world) {
	fill -= 0.1;
	if (fill <= 0) {
		Kill(world);
		return;
	}
	direction direct = last;
	if (r_rand() % 2)
		direct = (direction) (r_rand() % 4);
	if (world.sells[x][y+1].type == GRASS)
		direct = UP;
	if (world.sells[x][y-1].type == GRASS)
		direct = DOWN;
	if (world.sells[x+1][y].type == GRASS)
		direct = RIGHT;
	if (world.sells[x-1][y].type == GRASS)
		direct = LEFT;
	last = direct;
	Eat(world, direct);
	if (fill >= 300)
		Out(world);
}

//===================== World::World ===================================
World::World() {
	for (int x = 0; x < 256; x++) // делаем пустой мир
		for (int y = 0; y < 256; y++)
			sells[x][y].type = EMPTY,
			sells[x][y].animal = NULL;
	for (int i = 0; i < 2048; i++) // в первый день камни
		sells[r_rand() % 256][r_rand()%256].type = STONE;
	for (int i = 0; i < 256; i++) // во второй день -- границы
		sells[i][0].type = STONE,
		sells[i][255].type = STONE;
	for (int i = 1; i < 255; i++) // тоже второй день
		sells[0][i].type = STONE,
		sells[255][i].type = STONE;
	AddGrass(4000); // в третий и четвертый день -- траву
	for (int i = 0; i < 200; i++) { // а дальше -- всяку тварь
		int x = r_rand() % 256;
		int y = r_rand() % 256;
		if (sells[x][y].type == EMPTY) {
			Animal newAnimal(x, y, &animals);
			animals.push_back(newAnimal);
			sells[x][y].type = ANIMAL;
			sells[x][y].animal = &animals[animals.size()-1];
		} else {
			i--;
			continue;
		}
	}
	// седьмой день -- отдых
}

//====================== callbacks =====================================
void exit_callback(GtkWidget *widget, gpointer data) {
	gtk_main_quit();
}

//======================= stepper ======================================
// тут основная работа
gboolean step(gpointer data) {
	char str[32];
	World *world = (World *) data;
	
	sprintf(str, "Generation : %d", ++generation);
	gtk_label_set_text(GTK_LABEL(gen_label), str);
	
	sprintf(str, "Animals : %d", animals.size());
	gtk_label_set_text(GTK_LABEL(animal_label), str);
	
	world->AddGrass(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(eat_spin)));
	
	for (size_t i = 0; i < animals.size(); i++)
		animals[i].Step(*world);
	if ((generation % gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(draw_spin))) == 0)
		world->Draw(pixbuf);
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
	srand(time(NULL)); // инициализация rand
	
	World myWorld; // наш мирок
	
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
	head_label = gtk_label_new("Wor v0.1\n (c) OlOlO\n"); // ололо версия
	gen_label = gtk_label_new("Generation : 0"); // поколение:
	animal_label = gtk_label_new("Animals : 0"); // жывотных:
	GtkObject *adj = gtk_adjustment_new(1,
										1, 100000000,
										1, 1,
										0);
	draw_spin = gtk_spin_button_new(GTK_ADJUSTMENT(adj), 0.5, 0);
	
	adj = gtk_adjustment_new(	8,
								0, 100000000,
								1, 1,
								0);
	eat_spin = gtk_spin_button_new(GTK_ADJUSTMENT(adj), 0.5, 0);
	
	spin_label = gtk_label_new("draw every X\ngeneration:");
	eat_label = gtk_label_new("add X grass\nevery generation:");
	
	// привязываем их
	gtk_container_add(GTK_CONTAINER(window), hbox);
	gtk_container_add(GTK_CONTAINER(hbox), image);
	gtk_container_add(GTK_CONTAINER(hbox), vbox);
	gtk_container_add(GTK_CONTAINER(vbox), head_label);
	gtk_container_add(GTK_CONTAINER(vbox), gen_label);
	gtk_container_add(GTK_CONTAINER(vbox), animal_label);
	gtk_container_add(GTK_CONTAINER(vbox), spin_label);
	gtk_container_add(GTK_CONTAINER(vbox), draw_spin);
	gtk_container_add(GTK_CONTAINER(vbox), eat_label);
	gtk_container_add(GTK_CONTAINER(vbox), eat_spin);
	gtk_container_add(GTK_CONTAINER(vbox), start_button);
	gtk_container_add(GTK_CONTAINER(vbox), save_button);
	gtk_container_add(GTK_CONTAINER(vbox), exit_button);
	
	// сигналы
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(exit_callback), NULL);
	g_signal_connect(G_OBJECT(exit_button), "clicked", G_CALLBACK(exit_callback), NULL);
	g_signal_connect(G_OBJECT(start_button), "clicked", G_CALLBACK(start_callback), (gpointer) &myWorld);
	
	// рисуем мирок
	myWorld.Draw(pixbuf);
	
	//ни шагу назад
	gtk_widget_show_all(window);
	gtk_main();
}
