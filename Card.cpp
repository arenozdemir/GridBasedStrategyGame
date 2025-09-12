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
	float scale = 0.2f;
	Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { (float)x, (float)y, (float)texture.width * scale, (float)texture.height * scale };
	Vector2 origin = { 0.0f, 0.0f };
	DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
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