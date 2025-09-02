#pragma once
#include "pch.h"
#include "Hero.h"
#include "Grid.h"
#include "Card.h"
#include "Enemy.h"
//#include "GameLogic.h"

Hero::Hero(int posX, int posY, const char* heroName, int health, int attackSpeed, int damage, int stamina, HeroType type, HeroState state) : posX(posX), posY(posY), heroName(heroName), health(health), attackSpeed(attackSpeed), damage(damage), stamina(stamina), type(type), state(state), grid(&Grid::Instance()) {
	activeCell = &grid->at(posX, posY);
	grid->SetEntity(this, posX, posY);
	attackRange = (type == HeroType::swordman) ? 1 : 2;
}

void Hero::Draw()
{
	DrawCircle(
		activeCell->x + spacing / 2,
		activeCell->y + spacing / 2,
		30,             
		BLUE                 
	);
}

void Hero::Update()
{
	if (IsKeyPressed(KEY_UP)) {
		posY -= 1;
		grid->SetEntity(this, posX, posY);
		stamina -= 10;
	}
	if (IsKeyPressed(KEY_DOWN)) {
		posY += 1;
		grid->SetEntity(this, posX, posY);
		stamina -= 10;
	}
	if (IsKeyPressed(KEY_LEFT)) {
		posX -= 1;
		grid->SetEntity(this, posX, posY);
		stamina -= 10;
	}
	if (IsKeyPressed(KEY_RIGHT)) {
		posX += 1;
		grid->SetEntity(this, posX, posY);
		stamina -= 10;
	}
}

void Hero::UseCard(Card* card) {
	card->hasSelected = true;
	card->Effect(this);
}

void Hero::StaminaUpdate(int value)
{
	stamina += value;
	if (stamina > 100) stamina = 100; // Cap stamina at 100
	if (stamina < 0) stamina = 0; // Prevent negative stamina
}

void Hero::Attack(Enemy* enemy)
{
	if (enemy == nullptr) return;

	state = HeroState::attacking;

	if (state == HeroState::attacking && CheckRange(enemy->activeCell->pos, activeCell->pos)) {
		enemy->health -= damage;
		if (enemy->health <= 0) {
			enemy->isAlive = false;
		}
		StaminaUpdate(-20);
	}
	state = HeroState::idle;
	GameLogic::Instance().ActiveEnemy = nullptr;
}

bool Hero::CheckRange(Vector2 sourcePos, Vector2 endPos)
{
	float distance = sqrt(pow(endPos.x - sourcePos.x, 2) + pow(endPos.y - sourcePos.y, 2));

	if (type == HeroType::swordman) {
		return distance <= attackRange;
	}
	else if (type == HeroType::archer) {
		return distance <= attackRange;
	}
	else return false;
}
