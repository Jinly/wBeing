#include <animal.hpp>
#include <gui.hpp>

Animal::Animal(World &world, unsigned x, unsigned y) {
	// родился белорусом, стал цыганом
	color.red = r_rand() % 128 + 64;
	color.green = r_rand() % 128 + 64;
	color.blue = r_rand() % 128 + 128;
	color.alpha = 255;
	
	AnimalCell *cell = new AnimalCell(x, y, this);
	world.SetCell(x, y, cell);
	cells.push_back(cell);
	
	minx = maxx = x;
	miny = maxy = y;
	
}

Animal::~Animal() {

}

void Animal::Step(World &world) {
	world.AnimalEat(cells[r_rand() % cells.size()], (Direction)(r_rand() % 4));
	if (cells[r_rand() % cells.size()] -> fill > 300)
		world.AnimalMake(cells[r_rand() % cells.size()], (Direction)(r_rand() % 4));
	
	for (size_t i = 0; i < cells.size(); i++)
		if (cells[i]->energy < 5.0)
			world.AnimalAddEnergy(cells[i], 10.0);
	
	for (size_t i = 0; i < cells.size(); i++)
		cells[i] -> energy -= 0.1;
	
	for (size_t i = 0; i < cells.size(); i++)
		if (cells[i]->energy < 0.0)
			world.AnimalKill(cells[i]);
	if (cells.size() == 0)
		Kill(world);
	return;
}

void Animal::Kill(World &world) {
	for (size_t i = 0; i < world.animals.size(); i++)
		if (world.animals[i] == this) {
			world.animals.erase(world.animals.begin()+i);
			break;
		}
	delete this;
}

bool Animal::CellGo(World &world, AnimalCell *cells) {
	
}

bool Animal::CellEat(World &world, AnimalCell *cells) {
	
}

bool Animal::CellMake(World &world, AnimalCell *cells) {
	
}
