#include <gui.hpp>
#include <world.hpp>
#include <animal.hpp>
#include <main.hpp>

void World::DrawCell(int x, int y, GdkPixbuf *pixbuf) {
	switch (sells[x][y].type) {
		case EMPTY:
			put_4pixel(pixbuf, x*2, y*2, Color(0,0,0,255));
			break;
		case GRASS:
			put_4pixel(pixbuf, x*2, y*2, Color(0,255,0,255));
			break;
		case STONE:
			put_4pixel(pixbuf, x*2, y*2, Color(255,0,0,255));
			break;
		case ANIMAL:
			put_4pixel(pixbuf, x*2, y*2, Color(0,0,255,255));
			break;
		case EGG:
			put_4pixel(pixbuf, x*2, y*2, Color(255,255,0,255));
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
