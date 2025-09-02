#pragma once
#include "Entity.h"
// forward declaration
struct Hero; // forward declaration

struct Card : Entity
{
protected:
	const char* cardName = "Card"; // Default card name
	const char* cardDescription = "Default Card Description"; // Default card description
	int staminaCost = 0; 

public:
    int x = 0, y = 0;
    int baseY = 0;
    int width = 100;
    int height = 150;
    bool hasSelected = false;
    Card() {}
    void Draw() override;
    bool IsMouseOver(int mouseX, int mouseY) const;
    bool CardSelected(int mouseX, int mouseY, int button) const;
    virtual void Effect(Hero* hero);

};

struct AttackRangeCard : Card {
    AttackRangeCard() { staminaCost = -20; cardName = "AttackRangeCard"; cardDescription = "Use this card to increase attack range for 2 cells"; }
    void Effect(Hero* hero) override;
};

struct HealCard : Card {
	HealCard() { staminaCost = -30; cardName = "HealCard"; cardDescription = "Use this card to heal 20 health points"; }
    void Effect(Hero* hero) override;
};

struct StaminaCard : Card {
	StaminaCard() { cardName = "StaminaCard"; cardDescription = "Use this card to restore 40 stamina points"; }
    void Effect(Hero* hero) override;
};