#include "pch.h"
#include "Card.h"
#include "Hero.h"
#include "Entity.h"

bool Card::IsMouseOver(int mouseX, int mouseY) const
{
    return mouseX >= x && mouseX <= x + width &&
        mouseY >= y && mouseY <= y + height;
}

bool Card::CardSelected(int mouseX, int mouseY, int button) const
{
	return IsMouseOver(mouseX, mouseY) && IsMouseButtonPressed((MouseButton)button);
}

void Card::Effect(Hero* hero)
{

}

void Card::Draw()
{
    DrawRectangle(x, y, width, height, DARKGRAY);

    int segmentHeight = height / 4;
    DrawLine(x, y + segmentHeight, x + width, y + segmentHeight, BLACK);
    DrawLine(x, y + 2 * segmentHeight, x + width, y + 2 * segmentHeight, BLACK);
    DrawLine(x, y + 3 * segmentHeight, x + width, y + 3 * segmentHeight, BLACK);

    DrawText(cardName, x + 5, y + 5, 10, WHITE);
    DrawText(cardDescription, x + 5, y + segmentHeight + 5, 10, WHITE);
    DrawText(TextFormat("Stamina: %d", staminaCost), x + 5, y + 2 * segmentHeight + 5, 10, WHITE);
}

void HealCard::Effect(Hero* hero)
{
	hero->health += 20;
	hero->StaminaUpdate(HealCard::staminaCost);
	if (hero->health > 100) {
		hero->health = 100; 
	}
}

void StaminaCard::Effect(Hero* hero)
{
	hero->StaminaUpdate(40);
}

void AttackRangeCard::Effect(Hero* hero)
{
	hero->attackRange += 1;
	hero->StaminaUpdate(AttackRangeCard::staminaCost);
}