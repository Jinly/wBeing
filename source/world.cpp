#include <gui.hpp>
#include <world.hpp>
#include <animal.hpp>
#include <main.hpp>

//debug
#include <iostream>

// EmptyCell
EmptyCell::EmptyCell() {}
EmptyCell::~EmptyCell() {}
CellType EmptyCell::GetType(void) { return Empty; }
Color EmptyCell::GetColor(void) { return Color(0,0,0,255); }

// GrassCell
GrassCell::GrassCell() {}
GrassCell::~GrassCell() {}
CellType GrassCell::GetType(void) {return Grass; }
Color GrassCell::GetColor(void) { return Color(0,255,0,255); }

// AnimalCell
AnimalCell::AnimalCell(int _x, int _y, Animal *_selfanimal) {
	selfanimal = _selfanimal;
	fill = 100.0;
	energy = 10.0;
	x = _x; y = _y;
}

AnimalCell::~AnimalCell() {};

CellType AnimalCell::GetType(void) { return Being; }

Color AnimalCell::GetColor(void) { return selfanimal->GetColor(); }

// World
World::World(int x, int y) {
	cells = new EmptyCell* [x*y];
	w = x; h = y;
	AddGrass(1000);
	for (size_t i = 0; i < 20; i++) {
		int x = r_rand() % w;
		int y = r_rand() % h;
		Animal *animal = new Animal(*this, x, y);
		animals.push_back(animal);
	}
}

EmptyCell &World::GetCell(int x, int y) {
	static EmptyCell ret;
	if (*(cells + w*y + x) == NULL)
		return ret;
	return **(cells + w*y + x);
}

void World::SetCell(int x, int y, EmptyCell *cell) {
	*(cells + w*y + x) = cell;
}

void World::PasteCell(int x, int y, EmptyCell *cell) {
	if (*(cells + w*y + x) != NULL)
		delete *(cells + w*y + x);
	*(cells + w*y + x) = cell;
}

void World::Draw(GdkPixbuf *pixbuf) {
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			put_4pixel(pixbuf, x*2, y*2, GetCell(x, y).GetColor());
}

void World::AddGrass(unsigned n) {
	for (int i = 0; i < n; i++) {
		int x = r_rand() % w;
		int y = r_rand() % h;
		if (GetCell(x, y).GetType() == Empty)
			SetCell(x, y, new GrassCell);
	}
}

void World::GrowGrass(void) {
	for (int x = 1; x < w-1; x++)
		for (int y = 1; y < h-1; y++)
			if (GetCell(x, y).GetType() == Empty) {
				int n = 0;
				if (GetCell(x+1, y+1).GetType() == Grass) n++;
				if (GetCell(x+1, y).GetType() == Grass) n++;
				if (GetCell(x+1, y-1).GetType() == Grass) n++;
				if (GetCell(x, y+1).GetType() == Grass) n++;
				if (GetCell(x, y-1).GetType() == Grass) n++;
				if (GetCell(x-1, y+1).GetType() == Grass) n++;
				if (GetCell(x-1, y).GetType() == Grass) n++;
				if (GetCell(x-1, y-1).GetType() == Grass) n++;
				if (n == 3)
					SetCell(x, y, new GrassCell);
			}
}

void World::Step(void) {
	for (size_t i = 0; i < animals.size(); i++)
		animals[i]->Step(*this);
}

void World::GetRCoords(int x, int y, Direction direct, int &rx, int &ry) {
	switch (direct) {
		case Up:
			rx = x;
			ry = y -1;
			break;
		case Down:
			rx = x;
			ry = y + 1;
			break;
		case Left:
			rx = x - 1;
			ry = y;
			break;
		case Right:
			rx = x + 1;
			ry = y;
			break;
	}
	if (rx == -1)
		rx = w-1;
	if (rx == w)
		rx = 0;
	if (ry == -1)
		ry = h-1;
	if (ry == h)
		ry = 0;
}

void World::AnimalGo(AnimalCell *cell, Direction direct) {
	int rx, ry;
	GetRCoords(cell->x, cell->y, direct, rx, ry);
	CellType type = GetCell(rx, ry).GetType();
	if ((type == Empty) || (type == Grass)) {
		PasteCell(rx, ry, cell);
		SetCell(cell->x, cell->y, NULL);
		cell->x = rx;
		cell->y = ry;
		cell->energy -= 0.5;
	}
}

void World::AnimalEat(AnimalCell *cell, Direction direct) {
	int rx, ry;
	GetRCoords(cell->x, cell->y, direct, rx, ry);
	CellType type = GetCell(rx, ry).GetType();
	if ((type == Empty) || (type == Grass)) {
		if (type == Grass)
			cell->fill += 20.0;
		PasteCell(rx, ry, cell);
		SetCell(cell->x, cell->y, NULL);
		cell->x = rx;
		cell->y = ry;
		cell->energy -= 1.0;
	}
}

void World::AnimalMake(AnimalCell *cell, Direction direct) {
	if (cell->fill <= 150.0)
		return;
	int rx, ry;
	GetRCoords(cell->x, cell->y, direct, rx, ry);
	if (GetCell(rx, ry).GetType() != Empty)
		return;
	AnimalCell *animal = new AnimalCell(rx, ry, cell->selfanimal);
	SetCell(rx, ry, animal);
	
	cell->selfanimal->cells.push_back(animal);
	
	cell->fill -= 150.0;
}

void World::AnimalReproduct(AnimalCell *cell) {
	
}

void World::AnimalKill(AnimalCell *cell) {
	Animal &deleted = *cell->selfanimal;
	for (size_t i = 0; i < deleted.cells.size(); i++)
		if (cell == deleted.cells[i]) {
			deleted.cells.erase(deleted.cells.begin() + i);
			PasteCell(cell->x, cell->y, NULL);
			return;
		}
}

void World::AnimalAddEnergy(AnimalCell *cell, double n) {
	if (n > cell->fill)
		n = cell->fill;
	cell -> fill -= n;
	cell -> energy += n;
}

void World::AnimalAttack(AnimalCell *cell, Direction direct) {
	int rx, ry;
	GetRCoords(cell->x, cell->y, direct, rx, ry);
	if (GetCell(rx, ry).GetType() != Being)
		return;
	AnimalCell *other = (AnimalCell *) &GetCell(rx, ry);
	other -> energy -= cell -> energy / 5.0;
	cell -> energy -= cell -> energy / 5.0;
	if (other -> energy < 0)
		AnimalKill(other);
}
