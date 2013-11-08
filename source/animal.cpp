#include <gui.hpp>
#include <world.hpp>
#include <animal.hpp>
#include <main.hpp>

Animal::Animal(int _x, int _y, std::vector<Animal> *_selfvector) {
	x = _x; y = _y;
	fill = 100.0;
	selfvector = _selfvector;
	last = UP;
}

Animal::~Animal() {
}

void Animal::Go(World &world, direction direct) {
	int rx, ry;
	switch (direct) {
		case UP:
			rx = 0, ry = 1;
			break;
		case DOWN:
			rx = 0, ry = -1;
			break;
		case LEFT:
			rx = -1; ry = 0;
			break;
		case RIGHT:
			rx = 1; ry = 0;
			break;
	} 
	if ((world.sells[x+rx][y+ry].type == STONE) || (world.sells[x+rx][y+ry].type == ANIMAL) || (fill <= 0.5))
		return;
	world.sells[x+rx][y+ry].type = ANIMAL;
	world.sells[x+rx][y+ry].animal = world.sells[x][y].animal;
	world.sells[x][y].type = EMPTY;
	world.sells[x][y].animal = NULL;
	x += rx;
	y += ry;
	fill -= 0.5;
}

void Animal::Eat(World &world, direction direct) {
	int rx, ry;
	switch (direct) {
		case UP:
			rx = 0, ry = 1;
			break;
		case DOWN:
			rx = 0, ry = -1;
			break;
		case LEFT:
			rx = -1; ry = 0;
			break;
		case RIGHT:
			rx = 1; ry = 0;
			break;
	} 
	if ((world.sells[x+rx][y+ry].type == STONE) || (world.sells[x+rx][y+ry].type == ANIMAL) || (fill <= 0.5))
		return;
	if ((world.sells[x+rx][y+ry].type == GRASS) ||
		(world.sells[x+rx][y+ry].type == MEAT) ||
		(world.sells[x+rx][y+ry].type == EGG))
		fill += 20.0;
	world.sells[x+rx][y+ry].type = ANIMAL;
	world.sells[x+rx][y+ry].animal = world.sells[x][y].animal;
	world.sells[x][y].type = EMPTY;
	world.sells[x][y].animal = NULL;
	x += rx;
	y += ry;
	fill -= 0.5;
}

void Animal::Kill(World &world) {
	world.sells[x][y].type = EMPTY;
	world.sells[x][y].animal = NULL;
	for (size_t i = 0; i < selfvector->size(); i++)
		if (&((*selfvector)[i]) == this) {
			selfvector->erase(selfvector->begin()+i);
			return;
		}
}

// да родит коровенка теленка
void Animal::Out(World &world) {
	if (fill <= 100)
		return;
	direction direct = NONE;
	int ox, oy;
	if (world.sells[x][y+1].type == EMPTY)
		direct = UP, ox = x, oy = y+1;
	if (world.sells[x][y-1].type == EMPTY)
		direct = DOWN, ox = x, oy = y-1;
	if (world.sells[x+1][y].type == EMPTY)
		direct = RIGHT, ox = x+1, oy = y;
	if (world.sells[x-1][y].type == EMPTY)
		direct = LEFT, ox = x-1, oy = y;
	if (direct == NONE)
		return;
	world.sells[ox][oy].type = ANIMAL;
	animals.push_back(Animal(ox, oy, &animals));
	world.sells[ox][oy].animal = &animals[animals.size()-1];
	fill -= 150.0;
}

void Animal::Step(World &world) {
	fill -= 0.1;
	if (fill <= 0) {
		Kill(world);
		return;
	}
	direction direct = last;
	if (r_rand() % 2)
		direct = (direction) (r_rand() % 4);
	if (world.sells[x][y+1].type == GRASS)
		direct = UP;
	if (world.sells[x][y-1].type == GRASS)
		direct = DOWN;
	if (world.sells[x+1][y].type == GRASS)
		direct = RIGHT;
	if (world.sells[x-1][y].type == GRASS)
		direct = LEFT;
	last = direct;
	Eat(world, direct);
	if (fill >= 300)
		Out(world);
}
