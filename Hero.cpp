#pragma once
#include "pch.h"
#include "Hero.h"
#include "Grid.h"
#include "Card.h"
#include "Enemy.h"
//#include "GameLogic.h"

Hero::Hero(int posX, int posY, const char* heroName, int health, int attackSpeed, int damage, int stamina, HeroType type, HeroState state, const char* texturePath) : posX(posX), posY(posY), heroName(heroName), health(health), attackSpeed(attackSpeed), damage(damage), stamina(stamina), type(type), state(state), grid(&Grid::Instance()), texturePath(texturePath) {
	activeCell = &grid->at(posX, posY);
	grid->SetEntity(this, posX, posY);
	attackRange = (type == HeroType::swordman) ? 1 : 2;
	texture = LoadTexture(texturePath);
}

void Hero::Draw()
{
	float scale = 0.035f;

	Rectangle src = { 0, 0, (float)texture.width, (float)texture.height };
	Rectangle dest = {
		(float)activeCell->x + spacing / 2.0f,  
		(float)activeCell->y + spacing / 2.0f, 
		texture.width * scale,
		texture.height * scale
	};
	Vector2 origin = { dest.width / 2.0f, dest.height / 2.0f }; 

	DrawTexturePro(texture, src, dest, origin, 180.0f, WHITE);

	DrawHealthBar();
}

void Hero::Update()
{
	if (IsKeyPressed(KEY_UP)) {
		if (grid->SetEntity(this, posX, posY - 1)) {
			posY -= 1;
			stamina -= 10;
		}
	}
	if (IsKeyPressed(KEY_DOWN)) {
		if (grid->SetEntity(this, posX, posY + 1)) {
			posY += 1;
			stamina -= 10;
		}
	}
	if (IsKeyPressed(KEY_LEFT)) {
		if (grid->SetEntity(this, posX - 1, posY)) {
			posX -= 1;
			stamina -= 10;
		}
	}
	if (IsKeyPressed(KEY_RIGHT)) {
		if (grid->SetEntity(this, posX + 1, posY)) {
			posX += 1;
			stamina -= 10;
		}
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

void Hero::Attack(Enemy* enemy, const Cell* targetCell)
{
	if (enemy == nullptr) return;

	state = HeroState::attacking;

	if (state == HeroState::attacking && CheckRange(targetCell->pos, activeCell->pos)) {
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

void Hero::DrawHealthBar()
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