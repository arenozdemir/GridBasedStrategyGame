#include "pch.h"
#include "Grid.h"
#include "rlgl.h"
#include "Enemy.h"

Texture deck;

Grid::Grid(int width, int height) : width(width), height(height)
{
	deck = LoadTexture("Asssets/Tahtamap.png");

	cells.resize(height, std::vector<Cell>(width));

	int offsetX = (GetScreenWidth() - (width * spacing)) / 2 - 30;
	int offsetY = (GetScreenHeight() - (height * spacing)) / 2 - 5;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			cells[y][x].x = offsetX + (x * spacing);
			cells[y][x].y = offsetY + (y * spacing);
			cells[y][x].pos = { (float)x, (float)y };
		}
	}
}


Grid& Grid::Instance()
{
	static Grid instance(8, 8);
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
	return cell.owner;
}

Cell& Grid::GetCellAtMousePos(int x, int y) {
	x = (x - (GetScreenWidth() - (width * spacing)) / 2 + 30) / spacing;
	y = (y - (GetScreenHeight() - (height * spacing)) / 2 + 5) / spacing;

	return at(x, y);
}

bool Grid::SetEntity(Entity* entity, int x, int y) {
	Cell& cell = at(x, y);

	if (cell.owner != nullptr && cell.owner != entity) {
		return false;
	}

	if (entity->activeCell != nullptr) {
		entity->activeCell->owner = nullptr;
		entity->activeCell = nullptr;
	}

	cell.owner = entity;
	entity->activeCell = &cell;

	//cellin kendine ait bir özelliği varsa burada yapacak
	if (cell.UpdateSomething) {
		cell.UpdateSomething(cell);
	}

	return true;
}

void Grid::Draw()
{
	DrawTexture(deck, 0, 0, WHITE);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			const Cell& cell = cells[y][x];

			DrawRectangleLines(cell.x, cell.y, spacing, spacing, BLACK);
		}
	}
	
	for (std::vector<Cell>& row : cells) {
		for (Cell& cell : row) {
			if (cell.DrawSomething)
				cell.DrawSomething(cell);
		}
	}
}

bool Grid::IsInsideGrid(int x, int y) const {
	int offsetX = (GetScreenWidth() - (width * spacing)) / 2 - 30;
	int offsetY = (GetScreenHeight() - (height * spacing)) / 2 - 5;

	return (x >= offsetX && x < offsetX + width * spacing && y >= offsetY && y < offsetY + height * spacing);
}