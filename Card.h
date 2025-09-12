#pragma once
#include "Entity.h"
// forward declaration
struct Hero; // forward declaration

struct Card : Entity
{
protected:
    Texture texture;
	const char* cardName = "Card"; // Default card name
	const char* cardDescription = "Default Card Description"; // Default card description
	int staminaCost = 0; 

public:
	float GetTextureWidth() const { return (float)texture.width * 0.2f; }
	float GetTextureHeight() const { return (float)texture.height * 0.2f; }
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
    Rectangle GetBounds() const {
        float scale = 0.2f;
        return { (float)x, (float)y, (float)texture.width * scale, (float)texture.height * scale };
    }
};

struct AttackRangeCard : Card {
    AttackRangeCard() { staminaCost = -20; cardName = "AttackRangeCard"; cardDescription = "Use this card to increase attack range for 2 cells"; texture = LoadTexture("Asssets/AttackRangeCard.png");
    }
    void Effect(Hero* hero) override;
};

struct HealCard : Card {
	HealCard() { staminaCost = -30; cardName = "HealCard"; cardDescription = "Use this card to heal 20 health points"; texture = LoadTexture("Asssets/HealtCard.png");
    }
    void Effect(Hero* hero) override;
};

struct StaminaCard : Card {
	StaminaCard() { cardName = "StaminaCard"; cardDescription = "Use this card to restore 40 stamina points"; texture = LoadTexture("Asssets/StaminaCard.png");
    }
    void Effect(Hero* hero) override;
};