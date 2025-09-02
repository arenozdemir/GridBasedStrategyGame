#pragma once

#include "Entity.h"

struct Cell;
struct Entity;
struct Grid;
struct GameLogic;

struct Enemy : Entity {
	
	enum EnemyType {
		swordsman = 0,
		archer,
		Spider
	};

	enum EnemyState {
		idle = 0,
		moving,
		attacking
	};

	int health;
	int attackSpeed;
	int damage;
	int stamina;
	int attackRange;
	bool isAlive;
	const char* enemyName;
	
	Grid* grid;
	GameLogic* gamelogic;

	EnemyType type;
	EnemyState state;

	Enemy(int x, int y, int health, int attackSpeed, int damage, int stamina, EnemyType type, EnemyState state);

	void Draw() override;
	void DrawHealthBar();
	void Update() override;
	virtual void Act();
};

struct Spider : Enemy {
	using Enemy::Enemy;
	void Update() override;
	void Act() override;
};