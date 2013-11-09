#ifndef WORLD_HPP
	#define WORLD_HPP

#include <vector>

#include <gui.hpp>

class Animal;

enum cellType {
	EMPTY = 0,
	GRASS,
	STONE,
	ANIMAL,
	EGG
};

class Cell {
	public:
		cellType type;
		Animal *animal;
};

class World {
	public:
		World();
		void DrawCell(int x, int y, GdkPixbuf *pixbuf);
		void Draw(GdkPixbuf *pixbuf);
		void AddGrass(int n);
		Cell sells[256][256];		
};

#endif
