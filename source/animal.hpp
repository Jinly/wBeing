#ifndef ANIMAL_HPP
	#define ANIMAL_HPP

#include <vector>

#include <world.hpp>
#include <gui.hpp>
#include <main.hpp>

enum direction {
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
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
		std::vector<Animal> *selfvector;
		Color &GetColor(void);
	private:
		Color color;
		direction last;
		double fill;
};



#endif
