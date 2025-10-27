#pragma once
#include "Card.h"
#include "../../InGameMouse/InGameMouse.h"

class CardManager
{
public:
    CardManager();
    void Initialize(const std::vector<float>& moveValues, float centerX, float centerY);
    void UpdateAll(const Matrix4& vpMat);
    void DrawAll();

    std::vector<std::unique_ptr<Card>>& GetCards() { return cards; }

    void UseSelectedCard();
    bool HasSelectedCard() const;
    void HandleClick(InGameMouse* inGameMouse, const Matrix4& vpMat, float clickRadius, float moveValueOnClick);
    void HandleHover(InGameMouse* inGameMouse, const Matrix4& vpMat, float hoverRadius, float hoverOffsetY);
    Card* GetSelectedCard() const;

    std::vector<std::unique_ptr<Card>> cards;
    Card* lastHoveredCard_ = nullptr;
    std::function<void(float)> onCardSelected;
    float spacingAngle;
};

