#include "pch.h"
#include "Hero.h"
#include "Deck.h"
//include "GameLogic.h"
#include "Config.h"
void Deck::AddCard(Card* card)
{
    cards.push_back(card);
}

void Deck::AddHero(Hero* hero) {
    heroes.push_back(hero);
}

void Deck::Update() {
    Vector2 mousePos = GetMousePosition();
    for (auto& card : cards) {
        if (card->CardSelected((int)mousePos.x, (int)mousePos.y, 0) && GameLogic::Instance().ActiveHero->stamina >= 0) {
            GameLogic::Instance().ActiveHero->UseCard(card);
			cards.erase(std::remove(cards.begin(), cards.end(), card), cards.end());
        }
    }
}

void Deck::Draw()
{
    if (cards.empty()) return;

    int totalWidth = cards.size() * cardWidth + (cards.size() - 1) * spacing;
    int startX = (screenWidth - totalWidth) / 2;
    int baseY = screenHeight - cardHeight - 20;

    Vector2 mousePos = GetMousePosition();
    int index = 0;

    for (auto& card : cards) {
        int x = startX + index * (cardWidth + spacing);
        card->x = x;
        card->baseY = baseY;

        if (card->IsMouseOver((int)mousePos.x, (int)mousePos.y))
            card->y = baseY - 15;
        else
            card->y = baseY;

        card->Draw();

        index++;
    }
}