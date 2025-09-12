#pragma once
#include "Entity.h"

struct Grid;
struct Card;
struct Enemy;

enum HeroType {
	swordman = 0,
	archer
};

enum HeroState {
	idle = 0,
	moving,
	attacking,
};

struct Hero : Entity
{
	Texture texture;
	int posX, posY;
	int health;
	int attackSpeed;
	int damage;
	int stamina;
	int attackRange;
	const char* heroName;
	const char* texturePath;
	
	Grid* grid;

	HeroType type;
	HeroState state;

	Hero(int posX, int posY, const char* heroName, int healt, int attackSpeed, int damage,int stamina, HeroType type, HeroState state, const char* texturePath);

	void Draw() override;
	void Update() override;
	void UseCard(Card* card);
	void StaminaUpdate(int value);
	void Attack(Enemy* enemy, const Cell* targetCell);
	void DrawHealthBar();
	void DrawStaminaBar();
	bool CheckRange(Vector2 sourcePos, Vector2 endPos);
};

