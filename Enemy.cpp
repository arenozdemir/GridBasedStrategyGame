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
    
}

void Enemy::DrawHealthBar()
{
	int barWidth = 50;
	int barHeight = 10;
	int healthBarX = activeCell->x + spacing / 2 - barWidth / 2;
	int healthBarY = activeCell->y + spacing / 2 - 20;

	DrawRectangle(healthBarX, healthBarY, barWidth, barHeight, DARKGRAY);

	float healthPercentage = static_cast<float>(health) / 100.0f;
	int healthWidth = static_cast<int>(barWidth * healthPercentage);

	DrawRectangle(healthBarX, healthBarY, healthWidth, barHeight, GREEN);
}

void Enemy::Update()
{

}

Hero* Enemy::SelectRandomHero()
{
	if (gamelogic->heroes.empty())
		return nullptr;

	int randomIndex = rand() % gamelogic->heroes.size();
	return gamelogic->heroes[randomIndex];
}

Hero* Enemy::SelectNearestHero()
{
	if (gamelogic->heroes.empty())
		return nullptr;

	Hero* nearestHero = nullptr;
	float minDistance = std::numeric_limits<float>::max();

	for (Hero* hero : gamelogic->heroes) {
		float distance = sqrt(pow(hero->posX - posX, 2) + pow(hero->posY - posY, 2));
		if (distance < minDistance) {
			minDistance = distance;
			nearestHero = hero;
		}
	}

	return nearestHero;
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

void Spider::Draw()
{
    DrawTextureEx(texture, { (float)activeCell->x, (float)activeCell->y }, 0.0f, 0.2f, WHITE);
    DrawHealthBar();
}

void Spider::CleanupBabies() {
    for (int i = 0; i < 3; i++) {
        if (babies[i] && !babies[i]->isAlive) {
            delete babies[i];
            babies[i] = nullptr;
        }
    }
}

void Spider::BornBabySpider(Hero* targetHero) {
    CleanupBabies();

    int slot = -1;
    for (int i = 0; i < 3; i++) {
        if (!babies[i]) {
            slot = i;
            break;
        }
    }

    if (slot == -1) return;

    int posX = targetHero->posX;
    int posY = targetHero->posY - 1;

    if (grid->GetEntity(posX, posY)) {
        return;
    };

    BabySpider* baby = new BabySpider(1, posX, posY, 30, 1, 5, 20, Enemy::EnemyType::Spider, Enemy::EnemyState::idle, "Asssets/BabySpider.png");

    babies[slot] = baby;  
    gamelogic->AddEntity(baby);
}

void Spider::ProduceWeb(Hero* targetHero) {
    float scale = 0.05f;

    Texture webTexture = LoadTexture("Asssets/SpiderWeb1.png");

    Rectangle source = { 0.0f, 0.0f, (float)webTexture.width, (float)webTexture.height };
    Rectangle dest = { (float)targetHero->activeCell->x, (float)targetHero->activeCell->y, (float)webTexture.width * scale, (float)webTexture.height * scale };
    Vector2 origin = { 0.0f, 0.0f };

    targetHero->activeCell->DrawSomething = [webTexture, source, dest, origin](Cell& cell) {
        DrawTexturePro(webTexture, source, dest, origin, 0.0f, WHITE);
        };

    targetHero->activeCell->UpdateSomething = [](Cell& cell) {
        if (cell.owner != NULL) {
            GameLogic::Instance().ActiveHero->StaminaUpdate(-50);
        }
        };
}

Coroutine::StepFunc Spider::Act() {
	Hero* targetHero = SelectRandomHero();

    float timer = 0;
    int state = 0;
	
    ProduceWeb(targetHero);
	
    return [=](float dt) mutable -> bool {
        switch (state) {
        case 0:
            state = 1;
            break;
        case 1:
            timer += dt;
            if (timer >= 2.0f) {
				BornBabySpider(targetHero);
                return false;
            }
            break;
        }
        return true;
        };
}

void BabySpider::Draw()
{
    float scale = 0.5f;

    Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle dest = { (float)activeCell->x, (float)activeCell->y, (float)texture.width * scale, (float)texture.height * scale };
    Vector2 origin = { 0.0f, 0.0f };

    DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);

    DrawHealthBar();
}

void BabySpider::Update()
{
    if (health <= 0) {
		isAlive = false;
    }
}

Coroutine::StepFunc BabySpider::Act()
{
    Hero* targetHero = SelectNearestHero();
    if (!targetHero || !targetHero->activeCell)
        return [](float) { return false; };

    float timer = 0;
    int state = 0;

    return [=](float dt) mutable -> bool {
        switch (state) {
        case 0: 
            timer += dt;
            if (timer >= 2.0f) {
                timer = 0;
                state = 1; 
            }
            break;

        case 1: {
            float distance = sqrt(pow(targetHero->posX - posX, 2) + pow(targetHero->posY - posY, 2));
            if (distance <= sqrt(2)) {
                targetHero->health -= damage;
            }
            else {
                int dx = targetHero->posX - posX;
                int dy = targetHero->posY - posY;

                int stepX = (abs(dx) >= abs(dy)) ? ((dx > 0) ? 1 : -1) : 0;
                int stepY = (abs(dy) > abs(dx)) ? ((dy > 0) ? 1 : -1) : 0;

                posX += stepX;
                posY += stepY;
                Grid::Instance().SetEntity(this, posX, posY);
            }
            state = 2;
            break;
        }

        case 2: 
            timer += dt;
            if (timer >= 2.0f) {
                return false; 
            }
            break;
        }

        return true;
        };
}