#pragma once

#include "Entity.h"
#include "Coroutine.h"

struct Cell;
struct Entity;
struct Grid;
struct GameLogic;
struct BabySpider;

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

	Texture texture;
	int health;
	int attackSpeed;
	int damage;
	int stamina;
	int attackRange;
	int size;
	bool isAlive;
	const char* enemyName;
	const char* texturePath;
	
	Grid* grid;
	GameLogic* gamelogic;

	EnemyType type;
	EnemyState state;

	Coroutine::StepFunc behavior;

	Enemy(int size, int x, int y, int health, int attackSpeed, int damage, int stamina, EnemyType type, EnemyState state, const char* texturePath);

	Hero* SelectRandomHero();
	Hero* SelectNearestHero();
	void Draw() override;
	void DrawHealthBar();
	void Update() override;
	virtual Coroutine::StepFunc Act();
};


struct Spider : Enemy {
	using Enemy::Enemy;
	BabySpider* babies[3] = { nullptr, nullptr, nullptr };
	void Update() override;
	void Draw() override;
	void BornBabySpider(Hero* targetHero);
	void ProduceWeb(Hero* targetHero);
	void CleanupBabies();
	Coroutine::StepFunc Act() override;
};
struct BabySpider : Enemy {
	using Enemy::Enemy;
	void Update() override;
	void Draw() override;
	Coroutine::StepFunc Act() override;
};