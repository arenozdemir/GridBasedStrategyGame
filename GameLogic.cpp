#include "pch.h"
#include "GameLogic.h"
#include "Hero.h"
#include "Enemy.h"
#include "Grid.h"

bool playerTurn = true;
float timer = 0;

GameLogic& GameLogic::Instance() {
	static GameLogic instance;
	return instance;
}

void GameLogic::Update(float deltaTime)
{
    if (ActiveHero != NULL && playerTurn && ActiveHero->stamina > 0) ActiveHero->Update();

	if (!playerTurn) {
		for (Enemy* enemy : Grid::Instance().GetEnemies()) {
			if (enemy->isAlive) {
				enemy->Act();
			}
		}
	}

    SelectEntityFromCell();
    
    if (ActiveHero != NULL && CheckStamina()) {
        EndTurn();
    }

    if (timer <= tourTime) {
        timer += deltaTime;
        if (timer >= 30) {
            EndTurn();
        };
    }
}

void GameLogic::Draw()
{
    Timer();
    DrawAttackButton();
    DrawStamina();
}

void GameLogic::SelectEntityFromCell()
{
    
    if (IsMouseButtonPressed(0)) {
        Vector2 mousePos = GetMousePosition();
        int x = (int)(mousePos.x / spacing);
        int y = (int)(mousePos.y / spacing);

        if (!Grid::Instance().IsInsideGrid(x, y)) return;

        Entity* entity = Grid::Instance().GetEntity(x, y);

        Hero* hero = dynamic_cast<Hero*>(entity);
        Enemy* enemy = dynamic_cast<Enemy*>(entity);

        if (hero) {
            ActiveHero = hero;
            printf("Active hero set: %s\n", ActiveHero->heroName);
        }
        else if (enemy) {
            ActiveEnemy = enemy;
            printf("Active enemy set: %s\n", ActiveEnemy->enemyName);
        }
        else
        {
			ActiveHero = NULL;
			ActiveEnemy = NULL;
        }
    }
}

void GameLogic::Timer() {
    Vector2 center = { 50,600 };
    float rotationSpeed = 12;
    float endAngle = fmod(timer * rotationSpeed, 360.0f);
    DrawCircleSector(center, 30, 0, endAngle, 32, RED);
}

void GameLogic::DrawAttackButton()
{
    Vector2 mousePos = GetMousePosition();
    int x = (int)(mousePos.x / spacing);
    int y = (int)(mousePos.y / spacing);


    Rectangle attackButton = { 930, 600, 150, 50 };

    if (CheckCollisionPointRec(mousePos, attackButton) && playerTurn) {
        if (IsMouseButtonPressed(0)) {
            ActiveHero->Attack(ActiveEnemy);
        }
    }
    if (ActiveEnemy != nullptr) {
		DrawRectangleRec(attackButton, LIGHTGRAY);
        DrawText("Attack", attackButton.x + 40, attackButton.y + 10, 20, BLACK);
    }
}

bool GameLogic::CheckStamina()
{
    for (Hero* hero : heroes) {
		if (hero->stamina > 0) {
			return false;
		}
    }
	return true;
}

void GameLogic::DrawStamina()
{
    if (ActiveHero != nullptr) {
        int staminaPosX = screenWidth / 2 - (ActiveHero->stamina * 10 / 2);
        int staminaPosY = screenHeight - 10;

        DrawRectangle(staminaPosX, staminaPosY, ActiveHero->stamina * 10, 10, BLUE);
    }
}

void GameLogic::EndTurn()
{
	printf("End turn. Player turn: %s\n", playerTurn ? "true" : "false");
    for (Hero* hero : heroes) {
        if (!playerTurn) continue;
        hero->StaminaUpdate(30);
    }
    timer = 0;
    playerTurn = !playerTurn;
}

