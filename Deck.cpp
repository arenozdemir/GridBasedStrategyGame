#include "pch.h"
#include "Hero.h"
#include "Deck.h"
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

    for (auto it = cards.begin(); it != cards.end(); ) {
        Card* card = *it;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(mousePos, card->GetBounds()) &&
            GameLogic::Instance().ActiveHero->stamina >= 0)
        {
            GameLogic::Instance().ActiveHero->UseCard(card);
            it = cards.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Deck::Draw()
{
    if (cards.empty()) return;

    int totalWidth = cards.size() * cards[0]->GetTextureWidth() + (cards.size() - 1) * spacing;
    int startX = (screenWidth - totalWidth) / 2;
    int baseY = screenHeight - cards[0]->GetTextureHeight() + 170;

    Vector2 mousePos = GetMousePosition();
    int index = 0;

    for (auto& card : cards) {
        int x = startX + index * (card->GetTextureWidth() + spacing);
        card->x = x;
        card->baseY = baseY;

        if (CheckCollisionPointRec(mousePos, card->GetBounds()))
            card->y = baseY - 180;
        else
            card->y = baseY;

        card->Draw();
        index++;
    }
}