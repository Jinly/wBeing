#ifndef ANIMAL_HPP
	#define ANIMAL_HPP

#include <vector>

#include <world.hpp>
#include <main.hpp>

enum direction {
	UP = 0,
	DOWN,
	LEFT,
	RIGHT, 
	NONE
};

class Animal {
	public:
		Animal(int _x, int _y, std::vector<Animal> *selfvector);
		~Animal();
		void Step(World &world);
		void Go(World &world, direction direct);
		void Eat(World &world, direction direct);
		void Kill(World &world);
		void Out(World &world);
		unsigned x, y;
		double fill;
		std::vector<Animal> *selfvector;
		direction last;
};



#endif
