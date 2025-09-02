#include "pch.h"
#include "Enemy.h"
#include "Config.h"
#include "Cell.h"
#include "Grid.h"
//#include "GameLogic.h"

Enemy::Enemy(int x, int y, int health, int attackSpeed, int damage, int stamina, EnemyType type, EnemyState state)
	: health(health),
	attackSpeed(attackSpeed),
	damage(damage),
	stamina(stamina),
	type(type),
	state(state),
	grid(&Grid::Instance()),
	gamelogic(&GameLogic::Instance())  // <-- ayrı ayrı başlatılmalı
{
	posX = x;
	posY = y;
	activeCell = &grid->at(posX, posY);
	grid->SetEntity(this, posX, posY);
	enemyName = "Enemy";
}

void Enemy::Draw()
{
	DrawCircle(
		activeCell->x + spacing / 2,
		activeCell->y + spacing / 2,
		30,
		RED
	);
	DrawHealthBar();
}

void Enemy::DrawHealthBar()
{
	int barWidth = 50;
	int barHeight = 10;
	int healthBarX = activeCell->x + spacing / 2 - barWidth / 2;
	int healthBarY = activeCell->y + spacing / 2 - 20;

	DrawRectangle(healthBarX, healthBarY, barWidth, barHeight, DARKGRAY);

	float healthPercentage = static_cast<float>(health) / 100.0f; // Assuming max health is 100
	int healthWidth = static_cast<int>(barWidth * healthPercentage);

	DrawRectangle(healthBarX, healthBarY, healthWidth, barHeight, GREEN);
}

void Enemy::Update()
{

}

void Enemy::Act()
{
}

void Spider::Update()
{
	
}

void Spider::Act()
{

}
