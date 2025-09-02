#include "pch.h"
#include "Grid.h"
#include "Card.h"
#include "Deck.h"
#include "Hero.h"
//#include "GameLogic.h"
#include "Enemy.h"

int main() {

	InitWindow(screenWidth, screenHeight, "Grid game");

	SetTargetFPS(60);
    GameLogic& gameLogic = GameLogic::Instance();
    Grid& grid = Grid::Instance();

    Deck deck;

    HealCard healCard;
    StaminaCard staminaCard;
    AttackRangeCard attackRangeCard;

	deck.AddCard(&healCard);
	deck.AddCard(&staminaCard);
	deck.AddCard(&attackRangeCard);

    Hero swordMan(5,4,"swordman",100, 1, 20,100, HeroType::swordman, HeroState::idle);
    Hero archer(3, 3, "archer", 100, 2.5, 15, 100, HeroType::archer, HeroState::idle);

	gameLogic.heroes.push_back(&swordMan);
    gameLogic.heroes.push_back(&archer);

    Spider spider(4, 1, 100, 100, 100, 100, Enemy::EnemyType::Spider, Enemy::EnemyState::idle);

    deck.AddHero(&swordMan);
    deck.AddHero(&archer);

    Entity* entities[] = { &deck, &spider };

    int entityCount = sizeof(entities) / sizeof(entities[0]);

    while (!WindowShouldClose())
    {
		float deltaTime = GetFrameTime();

        Vector2 mousePos = GetMousePosition();

        int cellX = (int)(mousePos.x / spacing);
        int cellY = (int)(mousePos.y / spacing);

        gameLogic.Update(deltaTime);
        for (int i = 0; i < entityCount; i++)
        {
            entities[i]->Update();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (cellX >= 0 && cellX < grid.width && cellY >= 0 && cellY < grid.height)
        {
            Cell& cell = grid.at(cellX, cellY);

            DrawText(TextFormat("X: %d, Y: %d, Value: %d", cellX, cellY),
                10, 10, 20, BLACK);
        }

        else
        {
            DrawText("Mouse outside grid", 10, 10, 20, RED);
        }

		gameLogic.Draw();
        grid.Draw();
        archer.Draw();
        swordMan.Draw();

        for (int i = 0; i < entityCount; i++)
        {
            entities[i]->Draw();
        }

        EndDrawing();
    }

	CloseWindow(); 

	return 0;
}