#ifndef WORLD_HPP
	#define WORLD_HPP

#include <vector>

#include <gui.hpp>

class AnimalSell;

class Cell {
	public:
		enum cellType {
			EMPTY = 0,
			GRASS,
			STONE,
			ANIMAL,
			EGG
		};
		cellType type;
		AnimalSell *animal;
};

class World {
	public:
		World();
		void DrawCell(int x, int y, GdkPixbuf *pixbuf);
		void Draw(GdkPixbuf *pixbuf);
		void AddGrass(int n);
		void GrowGrass(void);
		void GetPoint(int x, int y, direction direct, int &ox, int &oy);
		Cell sells[256][256];	
};

#endif
