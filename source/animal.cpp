#include <animal.hpp>
#include <gui.hpp>
#include <world.hpp>
#include <main.hpp>

AnimalSell::AnimalSell(int _x, int _y, std::vector<AnimalSell> *_selfvector) {
	x = _x; y = _y;
	fill = 10.0;
	energy = 50.0;
	selfvector = _selfvector;
	last = UP;
	color.alpha = 255;
	color.blue = r_rand() % 128 + 128;
	color.red = 64 + r_rand() % 128;
	color.green = 64 + r_rand() % 128;
}

AnimalSell::~AnimalSell() {
}

void AnimalSell::Go(World &world, direction direct) {
	int rx, ry;
	world.GetPoint(x, y, direct, rx, ry);
	if ((world.sells[rx][ry].type == Cell::STONE) || (world.sells[rx][ry].type == Cell::ANIMAL) || (energy <= 0.005*fill))
		return;
	world.sells[rx][ry].type = Cell::ANIMAL;
	world.sells[rx][ry].animal = world.sells[x][y].animal;
	world.sells[x][y].type = Cell::EMPTY;
	world.sells[x][y].animal = NULL;
	x = rx;
	y = ry;
	energy -= 0.005 * fill;
}

void AnimalSell::Eat(World &world, direction direct) {
	int rx, ry;
	world.GetPoint(x, y, direct, rx, ry);
	if ((world.sells[rx][ry].type == Cell::STONE) || (world.sells[rx][ry].type == Cell::ANIMAL) || (energy <= 0.01*fill))
		return;
	if ((world.sells[rx][ry].type == Cell::GRASS) ||
		(world.sells[rx][ry].type == Cell::EGG))
		fill += 20.0;
	world.sells[rx][ry].type = Cell::ANIMAL;
	world.sells[rx][ry].animal = world.sells[x][y].animal;
	world.sells[x][y].type = Cell::EMPTY;
	world.sells[x][y].animal = NULL;
	x = rx;
	y = ry;
	energy -= 0.01*fill;
}

void AnimalSell::Kill(World &world) {
	world.sells[x][y].type = Cell::EMPTY;
	world.sells[x][y].animal = NULL;
	for (size_t i = 0; i < selfvector->size(); i++)
		if (&((*selfvector)[i]) == this) {
			selfvector->erase(selfvector->begin()+i);
			return;
		}
}

void AnimalSell::Make(World &world, direction direct) {
	if (fill <= 150)
		return;
	int rx, ry;
	world.GetPoint(x, y, direct, rx, ry);
	if (world.sells[rx][ry].type != Cell::EMPTY)
		return;
	world.sells[rx][ry].type = Cell::ANIMAL;
	animals.push_back(AnimalSell(rx, ry, &animals));
	world.sells[rx][ry].animal = &animals[animals.size() - 1];
	world.sells[rx][ry].animal -> color = color;
	fill -= 150.0;
}

void AnimalSell::Step(World &world) {
	energy -= 0.1;
	if (energy < 5.0)
		AddEnergy(10.0);
	if (energy <= 0.0) {
		Kill(world);
		return;
	}
	direction direct = last;
	if (r_rand() % 3)
		direct = (direction) (r_rand() % 4);
	if (world.sells[x][y+1].type == Cell::GRASS)
		direct = UP;
	if (world.sells[x][y-1].type == Cell::GRASS)
		direct = DOWN;
	if (world.sells[x+1][y].type == Cell::GRASS)
		direct = RIGHT;
	if (world.sells[x-1][y].type == Cell::GRASS)
		direct = LEFT;
	last = direct;
	Eat(world, direct);
	if (fill >= 200)
		Make(world, (direction) (r_rand() % 4));
}

void AnimalSell::AddEnergy(double _fill) {
	if (_fill >= fill)
		_fill = fill;
	fill -= _fill;
	energy += _fill;
}

Color &AnimalSell::GetColor(void) {
	return color;
}

Animal::Animal(AnimalSell *sell) {
	sells.push_back(sell);
}

Animal::~Animal() {
	
}

void Animal::Step(World *world) {
	return;
}
