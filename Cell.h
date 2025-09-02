#pragma once

struct Entity;

struct Cell
{
	Vector2 pos;
	int x, y; // Position of the cell
	Entity* owner;
};