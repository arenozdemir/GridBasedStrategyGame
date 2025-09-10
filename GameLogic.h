#pragma once

#include "Coroutine.h"

struct Hero;
struct Enemy;
struct Grid;

struct GameLogic
{
    static GameLogic& Instance();
    
    GameLogic() = default;

    float turnDuration = 30.0f;
    float timeLeft = turnDuration;

    int staminaRegenPerTurn = 30;

    Rectangle endTurnButton = { 900, 650, 150, 50 };

    Hero* ActiveHero = NULL;

	Enemy* ActiveEnemy = NULL;

	std::vector<Hero*> heroes;

	std::vector<Enemy*> enemies;

    void Update(float deltaTime);

    void Draw();

    void SelectEntityFromCell();

    void EndTurn();

    void Timer();

    void DrawAttackButton();

    void DrawEndTourButton();

	bool CheckStamina();

    void DrawStamina();
    
    CoroutineManager coroutineManager;
};