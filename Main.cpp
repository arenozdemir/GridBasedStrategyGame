#include "pch.h"
#include "Grid.h"
#include "Card.h"
#include "Deck.h"
#include "Hero.h"
#include "Enemy.h"

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
    //Hero archer(3, 3, "archer", 100, 2.5, 15, 100, HeroType::archer, HeroState::idle,"null");

	gameLogic.heroes.push_back(&swordMan);
    //gameLogic.heroes.push_back(&archer);
    Spider spider(3, 4, 1, 100, 100, 100, 100, Enemy::EnemyType::Spider, Enemy::EnemyState::idle, "Asssets/spider.png");

	gameLogic.enemies.push_back(&spider);

    deck.AddHero(&swordMan);
    //deck.AddHero(&archer);

    Entity* entities[] = { &deck, &spider};

    int entityCount = sizeof(entities) / sizeof(entities[0]);

    while (!WindowShouldClose())
    {
		float deltaTime = GetFrameTime();

        Vector2 mousePos = GetMousePosition();

        gameLogic.Update(deltaTime);
        for (int i = 0; i < entityCount; i++)
        {
            entities[i]->Update();
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        grid.Draw();
        gameLogic.Draw();
        //archer.Draw();
        swordMan.Draw();

        for (int i = 0; i < entityCount; i++)
        {
            entities[i]->Draw();
        }
        //DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 50, RED);
        EndDrawing();
    }

	CloseWindow(); 

	return 0;
}