#include <gui.hpp>
#include <world.hpp>
#include <animal.hpp>
#include <main.hpp>

//debug
#include <iostream>

void World::DrawCell(int x, int y, GdkPixbuf *pixbuf) {
	switch (sells[x][y].type) {
		case Cell::EMPTY:
			put_4pixel(pixbuf, x*2, y*2, Color(0,0,0,255));
			break;
		case Cell::GRASS:
			put_4pixel(pixbuf, x*2, y*2, Color(0,255,0,255));
			break;
		case Cell::STONE:
			put_4pixel(pixbuf, x*2, y*2, Color(255,0,0,255));
			break;
		case Cell::ANIMAL:
			put_4pixel(pixbuf, x*2, y*2, sells[x][y].animal->GetColor());
			break;
		case Cell::EGG:
			put_4pixel(pixbuf, x*2, y*2, Color(255,255,0,255));
			std::cerr << "ololo\n";
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
		if (sells[x][y].type == Cell::EMPTY)
			sells[x][y].type = Cell::GRASS;
	}
}

World::World() {
	for (int x = 0; x < 256; x++) // делаем пустой мир
		for (int y = 0; y < 256; y++)
			sells[x][y].type = Cell::EMPTY,
			sells[x][y].animal = NULL;
	for (int i = 0; i < 1024; i++) // в первый день камни
		sells[r_rand() % 256][r_rand()%256].type = Cell::STONE;
	AddGrass(1000); // в третий и четвертый день -- траву
	for (int i = 0; i < 50000; i++) { // а y6y -- всяку тварь
		int x = r_rand() % 256;
		int y = r_rand() % 256;
		if (sells[x][y].type == Cell::EMPTY) {
			AnimalSell newAnimal(x, y, &animals);
			animals.push_back(newAnimal);
			sells[x][y].type = Cell::ANIMAL;
			sells[x][y].animal = &animals[animals.size()-1];
		}
	}
	// седьмой день -- отдых
}

void World::GetPoint(int x, int y, direction direct, int &ox, int &oy) {
	int rx, ry;
	switch (direct) {
		case DOWN:
			rx = 0; ry = -1; break;
		case UP:
			rx = 0; ry = 1; break;
		case RIGHT:
			rx = 1; ry = 0; break;
		case LEFT:
			rx = -1; ry = 0; break;
	}
	x += rx; y += ry;
	if (x == 256)
		x = 0;
	if (x == -1)
		x = 255;
	if (y == 256)
		y = 0;
	if (y == -1)
		y = 255;
	ox = x; oy = y;
}

void World::GrowGrass(void) {
	for (int x = 1; x < 255; x++)
		for (int y = 1; y < 255; y++)
			if (sells[x][y].type == Cell::EMPTY) {
				int n = 0;
				if (sells[x+1][y].type == Cell::GRASS) n++;
				if (sells[x+1][y+1].type == Cell::GRASS) n++;
				if (sells[x+1][y-1].type == Cell::GRASS) n++;
				if (sells[x][y-1].type == Cell::GRASS) n++;
				if (sells[x][y+1].type == Cell::GRASS) n++;
				if (sells[x-1][y+1].type == Cell::GRASS) n++;
				if (sells[x-1][y-1].type == Cell::GRASS) n++;
				if (sells[x-1][y].type == Cell::GRASS) n++;
				if (n == 3)
					sells[x][y].type = Cell::GRASS;
			}
}
