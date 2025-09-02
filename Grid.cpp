#include "pch.h"
#include "Grid.h"
#include "rlgl.h"
#include "Enemy.h"
Grid::Grid(int width, int height) : width(width), height(height)
{
	//cells = std::vector<Cell>(width * height);
	cells.resize(height, std::vector<Cell>(width));

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			cells[y][x].x = x * spacing;
			cells[y][x].y = y * spacing;
			cells[y][x].pos = { (float)x, (float)y };
		}
	}
}


Grid& Grid::Instance()
{
	static Grid instance(10, 5);
	return instance;
}


Grid::~Grid()
{
	
}

Cell& Grid::at(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return cells[0][0]; 
	}

	return cells[y][x];
}

Entity* Grid::GetEntity(int x, int y) {
	Cell& cell = at(x, y);
	return cell.owner; // Boşsa nullptr döner
}

void Grid::SetEntity(Entity* entity, int x, int y) {
	if (entity->activeCell != nullptr) {
		entity->activeCell->owner = nullptr;
		entity->activeCell = nullptr;
	}

	Cell& cell = at(x, y);
	cell.owner = entity;
	entity->activeCell = &cell;
}

void Grid::Draw()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			const Cell& cell = cells[y][x];

			DrawRectangleLines(cell.x, cell.y, spacing, spacing, BLACK);
		}
	}
}

std::vector<Enemy*> Grid::GetEnemies()
{
	std::vector<Enemy*> enemies;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Entity* entity = GetEntity(x, y);
			if (Enemy* enemy = dynamic_cast<Enemy*>(entity)) {
				enemies.push_back(enemy);
			}
		}
	}
	return enemies;
}

bool Grid::IsInsideGrid(int x, int y) const {
	return (x >= 0 && x < width && y >= 0 && y < height);
}