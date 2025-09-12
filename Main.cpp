#include "pch.h"
#include "Entities.h"
#include "Grid.h"
#include "Card.h"
#include "Deck.h"
#include "Hero.h"
#include "Enemy.h"

std::vector<Entity*> entities;

int main() {

	InitWindow(1920, 1080, "Grid game");
    
    ToggleFullscreen();
	
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

    Hero swordMan(5,4,"swordman",100, 1, 20,100, HeroType::swordman, HeroState::idle,"Asssets/Swordman.png");

	gameLogic.heroes.push_back(&swordMan);

    Spider spider(3, 4, 1, 100, 100, 100, 100, Enemy::EnemyType::Spider, Enemy::EnemyState::idle, "Asssets/spider.png");

	gameLogic.enemies.push_back(&spider);

    deck.AddHero(&swordMan);

	entities.push_back(&deck);
	entities.push_back(&spider);

    while (!WindowShouldClose())
    {
		float deltaTime = GetFrameTime();

        Vector2 mousePos = GetMousePosition();

        gameLogic.Update(deltaTime);
        for (int i = 0; i < entities.size(); i++)
        {
            entities[i]->Update();
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        grid.Draw();
        gameLogic.Draw();
        swordMan.Draw();

        for (int i = 0; i < entities.size(); i++)
        {
            entities[i]->Draw();
        }
        EndDrawing();
    }

	CloseWindow(); 

	return 0;
}