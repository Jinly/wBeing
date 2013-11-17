#ifndef ANIMAL_HPP
	#define ANIMAL_HPP

#include <world.hpp>

class Animal {
	public:
		Animal(World &world, unsigned x, unsigned y);
		~Animal();
		Color GetColor(void) { return color; }
		void Step(World &world);
		void Kill(World &world);
	//	
	//	data
		std::vector<AnimalCell *> cells;
	private:
		Color color;
		int maxx, maxy;
		int minx, miny;
		// NULL
};

#endif
