#pragma once

#include "Cell.h"
#include "Config.h"
#include "Entity.h"

struct Grid;

struct Grid
{
	//int spacing = 20;
	int width, height;

	std::vector<std::vector<Cell>> cells;

	Grid(int width, int height);
	
	static Grid& Instance();

	~Grid();

	Cell& at(int x, int y);

	Entity* GetEntity(int x, int y);

	void SetEntity(Entity* entity, int x, int y);

	void Draw();

	// Returns a vector of all entities in the grid
	std::vector<Enemy*> GetEnemies();

	bool IsInsideGrid(int x, int y) const; // <-- burası
};

