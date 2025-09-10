#pragma once

struct Cell;

struct Entity
{
protected:
	int posX, posY;
public:
	virtual ~Entity() = default;
	virtual void Update();
	virtual void Draw();
	virtual Vector2 GetPos();
	Cell* activeCell;
	std::vector<Cell*> activeCells;
};