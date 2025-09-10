#pragma once
#include <functional>
struct Entity;

struct Cell
{
	Vector2 pos;
	int x, y; // Position of the cell
	Entity* owner;
	Texture texture;
	std::function<void(Cell&)> DrawSomething;
	std::function<void(Cell&)> UpdateSomething;
};