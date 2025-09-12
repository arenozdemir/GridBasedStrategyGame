#include "pch.h"
#include "GameLogic.h"
#include "Hero.h"
#include "Enemy.h"
#include "Grid.h"
#include <queue>

bool playerTurn = true;
float timer = 0;
Rectangle attackButton;
Cell* targetCell = nullptr;

std::queue<Coroutine::StepFunc> enemyQueue;
Coroutine::StepFunc currentEnemyCoroutine = nullptr;

GameLogic& GameLogic::Instance() {
    static GameLogic instance;
    return instance;
}

void GameLogic::Update(float deltaTime)
{
    if (ActiveHero != NULL && playerTurn && ActiveHero->stamina > 0)
        ActiveHero->Update();

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
    
    //enemy turn
    if (!playerTurn) {
        if (!currentEnemyCoroutine && !enemyQueue.empty()) {
            currentEnemyCoroutine = enemyQueue.front();
            enemyQueue.pop();
            timer = 0;
        }

        if (currentEnemyCoroutine) {
            bool running = currentEnemyCoroutine(deltaTime);
            if (!running) {
                currentEnemyCoroutine = nullptr;
            }
        }

        if (!currentEnemyCoroutine && enemyQueue.empty()) {
            EndTurn();
        }
    }
}

void GameLogic::Draw()
{
    Timer();
    DrawAttackButton();
    DrawEndTourButton();
}

void GameLogic::SelectEntityFromCell()
{
    if (IsMouseButtonPressed(0)) {
        Vector2 mousePos = GetMousePosition();
        int x = mousePos.x;
        int y = mousePos.y;

        if (CheckCollisionPointRec(mousePos, attackButton) && playerTurn) {
            if (IsMouseButtonPressed(0)) {
                ActiveHero->Attack(ActiveEnemy, targetCell);
            }
            return;
        }

        if (!Grid::Instance().IsInsideGrid(x, y)) return;

        Cell* c = &Grid::Instance().GetCellAtMousePos(x, y);
        Entity* entity = c->owner;

        Hero* hero = dynamic_cast<Hero*>(entity);
        Enemy* enemy = dynamic_cast<Enemy*>(entity);

        if (hero) {
            ActiveHero = hero;
            printf("Active hero set: %s\n", ActiveHero->heroName);
        }
        else if (enemy) {
            targetCell = &Grid::Instance().GetCellAtMousePos(x, y);
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
    Vector2 center = { screenWidth - 1600, screenHeight - 100 };
    float rotationSpeed = 12.0;
    float endAngle = fmod(timer * rotationSpeed, 360.0f);
    DrawCircleSector(center, 30, 0, endAngle, 32, RED);
}

void GameLogic::DrawAttackButton()
{
    attackButton = { screenWidth - 500, screenHeight / 2 - 25, 150, 50 };

    if (ActiveEnemy != nullptr && ActiveHero != nullptr) {
        DrawRectangleRec(attackButton, LIGHTGRAY);
        DrawText("Attack", attackButton.x + 40, attackButton.y + 10, 20, BLACK);
    }
}

void GameLogic::DrawEndTourButton()
{
    if (!playerTurn) return;
    endTurnButton = { screenWidth - 300, screenHeight - 100, 150, 50 };

    DrawRectangleRec(endTurnButton, LIGHTGRAY);
    DrawText("End Turn", endTurnButton.x + 30, endTurnButton.y + 10, 20, BLACK);

    if (IsMouseButtonPressed(0) && playerTurn) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, endTurnButton)) {
            EndTurn();
        }
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

void GameLogic::AddEntity(Entity* entity)
{
	if (Hero* hero = dynamic_cast<Hero*>(entity)) {
		heroes.push_back(hero);
	}
	else if (Enemy* enemy = dynamic_cast<Enemy*>(entity)) {
		enemies.push_back(enemy);
	}

	entities.push_back(entity);
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

    if (!playerTurn) {
        while (!enemyQueue.empty()) enemyQueue.pop();
        currentEnemyCoroutine = nullptr;
        for (Enemy* enemy : enemies) {
            if (enemy->isAlive)
                enemyQueue.push(enemy->Act());
        }
    }
}