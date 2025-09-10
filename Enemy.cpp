#include "pch.h"
#include "Enemy.h"
#include "Config.h"
#include "Cell.h"
#include "Grid.h"
#include "Hero.h"

Enemy::Enemy(int size, int x, int y, int health, int attackSpeed, int damage, int stamina, EnemyType type, EnemyState state, const char* texturePath)
    : size(size),
    health(health),
    attackSpeed(attackSpeed),
    damage(damage),
    stamina(stamina),
    type(type),
    state(state),
    grid(&Grid::Instance()),
    gamelogic(&GameLogic::Instance()),
    texturePath(texturePath)
{
    posX = x;
    posY = y;
	isAlive = true;

    if (size == 1) {
        activeCell = &grid->at(posX, posY);
        activeCells.push_back(activeCell);
        activeCell->owner = this;
    }
    else if (size > 1) {
        activeCells.reserve(size * size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                Cell* cell = &grid->at(posX + i, posY + j);
                activeCells.push_back(cell);
                cell->owner = this;
            }
        }
        activeCell = activeCells[0]; // merkez / referans hücre
    }
    else {
		printf("Invalid enemy size\n");
    }

    enemyName = "Enemy";
    texture = LoadTexture(texturePath);
}

void Enemy::Draw()
{
	DrawTextureEx(texture, { (float)activeCell->x, (float)activeCell->y }, 0.0f, 0.2f, WHITE);
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

Coroutine::StepFunc Enemy::Act() {
    float timer = 0;
    int state = 0;

    return [=](float dt) mutable -> bool {
        switch (state) {
        case 0:
            printf("Enemy attacks!\n");
            state = 1;
            break;
        case 1:
            timer += dt;
            if (timer >= 2.0f) {
                printf("Damage applied!\n");
                return false; // coroutine bitti
            }
            break;
        }
        return true; // devam et
        };
}

void Spider::Update()
{
	
}

Coroutine::StepFunc Spider::Act() {
    if (gamelogic->heroes.empty())
        return [](float) { return false; };

    int randomIndex = rand() % gamelogic->heroes.size();
    Hero* targetHero = gamelogic->heroes[randomIndex];
    if (targetHero->activeCell == nullptr)
        return [](float) { return false; };

    float timer = 0;
    int state = 0;
	
    float scale = 0.05f;

	Texture webTexture = LoadTexture("Asssets/SpiderWeb1.png");

	Rectangle source = { 0.0f, 0.0f, (float)webTexture.width, (float)webTexture.height };
	Rectangle dest = { (float)targetHero->activeCell->x, (float)targetHero->activeCell->y, (float)webTexture.width * scale, (float)webTexture.height * scale };
	Vector2 origin = { 0.0f, 0.0f };

	DrawTexturePro(webTexture, source, dest, origin, 0.0f, WHITE);

    targetHero->activeCell->DrawSomething = [webTexture, source, dest, origin](Cell& cell) {
        DrawTexturePro(webTexture, source, dest, origin, 0.0f, WHITE);
        };


    targetHero->activeCell->UpdateSomething = [](Cell& cell) {
        if (cell.owner != NULL) {
            GameLogic::Instance().ActiveHero->StaminaUpdate(-50);
			//cell.DrawSomething = nullptr;
			//cell.UpdateSomething = nullptr;
        }
    };
	
    return [=](float dt) mutable -> bool {
        switch (state) {
        case 0:
            printf("Spider attacks!\n");
            state = 1;
            break;
        case 1:
            timer += dt;
            if (timer >= 2.0f) {
                printf("Spider damage applied!\n");
                return false;
            }
            break;
        }
        return true; // coroutine devam ediyor
        };
}