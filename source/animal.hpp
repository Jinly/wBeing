#ifndef ANIMAL_HPP
	#define ANIMAL_HPP

#include <vector>

#include <world.hpp>
#include <gui.hpp>
//#include <main.hpp>

class Animal;

class AnimalSell {
	public:
		AnimalSell(int _x, int _y, std::vector<AnimalSell> *selfvector);
		~AnimalSell();
		void Step(World &world);
		void Go(World &world, direction direct);
		void Eat(World &world, direction direct);
		void Kill(World &world);
		void Make(World &world, direction direct);
		void AddEnergy(double _fill);
		unsigned x, y;
		std::vector<AnimalSell> *selfvector;
		Animal *selfanimal;
		Color &GetColor(void);
	private:
		Color color;
		direction last;
		double energy;
		double fill;
};

class Animal {
	public:
		Animal(AnimalSell *sell);
		~Animal();
		void Step(World *world);
		//data
		std::vector<AnimalSell *> sells;
};

#endif
