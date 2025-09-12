#pragma once
#include "Entity.h"
#include "Card.h"
struct GameLogic;
struct Hero;
struct Deck;

struct Deck : Entity
{
    std::vector<Card*> cards;
    //for heroes who use cards
    std::vector<Hero*> heroes;

    void AddCard(Card* card);

    void AddHero(Hero* hero);

    void Draw() override;

    void Update() override;

	bool HandleInput();
};