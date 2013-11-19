#ifndef WORLD_HPP
	#define WORLD_HPP

#include <vector>

#include <gui.hpp>

class Animal;

enum CellType {
	Empty,
	Grass,
	Being,
	Egg
};

enum Direction {
	Up,
	Down,
	Left,
	Right
};

class EmptyCell {
	public:
		EmptyCell();
		virtual ~EmptyCell();
		virtual CellType GetType(void);
		virtual Color GetColor(void);
};

class GrassCell : public EmptyCell {
	public:
		GrassCell();
		virtual ~GrassCell();
		virtual CellType GetType(void);
		virtual Color GetColor(void);
};

class AnimalCell : public EmptyCell {
	public:
		AnimalCell(int _x, int _y, Animal *_selfanimal);
		virtual ~AnimalCell();
		virtual CellType GetType(void);
		virtual Color GetColor(void);
		double fill, energy;
		Animal *selfanimal;
		int x, y;
};

class World {
	public:
		World(int x, int y);
		~World() {};
	// methods
		void Draw(GdkPixbuf *pixbuf);
		void AddGrass(unsigned n);
		void GrowGrass(void);
		void Step(void);
	// work with cells
		EmptyCell &GetCell(int x, int y);
		void SetCell(int x, int y, EmptyCell *cell);
		void PasteCell(int x, int y, EmptyCell *cell);
	// work with animals
		void GetRCoords(int x, int y, Direction direct, int &rx, int &ry);
		void AnimalGo(AnimalCell *cell, Direction direct);
		void AnimalEat(AnimalCell *cell, Direction direct);
		void AnimalMake(AnimalCell *cell, Direction direct);
		void AnimalReproduct(AnimalCell *cell);
		void AnimalAttack(AnimalCell *cell, Direction direct);
		void AnimalKill(AnimalCell *cell);
		void AnimalAddEnergy(AnimalCell *cell, double n);
	// width and h
		int w, h;
		std::vector<Animal *> animals;
	private:
		EmptyCell **cells;
};

#endif
