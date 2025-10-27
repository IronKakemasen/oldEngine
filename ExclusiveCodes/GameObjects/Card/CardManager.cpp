#include "CardManager.h"
#include "../../Audio/AudioHandle.h"
#include "../../Audio/AudioPlayer.h"
#include "../../InGameController/InGameController.h"

CardManager::CardManager()
{
	spacingAngle = 32.0f;
}

void CardManager::Initialize(const std::vector<float>& moveValues, float centerX, float centerY) 
{
    cards.clear();

    int cardCount = (int)moveValues.size();

    for (int i = 0; i < cardCount; ++i)
    {
        auto newCard = std::make_unique<Card>();
        newCard->Initialize(moveValues[i]);
        newCard->SetMoveValue(moveValues[i]);

        // 扇状配置
        int reverseIndex = (cardCount - 1) - i;
        float offsetIndex = reverseIndex - (cardCount - 1) / 2.0f;
        float angleDeg = offsetIndex * spacingAngle;
        float angleRad = angleDeg * (3.14159265f / 180.0f);

        float radius = 3.3f;
        float x = centerX + radius * std::sin(-angleRad);
        float y = centerY - radius * (1.0f - std::cos(-angleRad));
        float z = -0.1f * i - 1.5f;

        Vector3 pos = { x, y, z };

        // jointCube
        newCard->jointCube.trans.pos = pos;
        newCard->jointCube.trans.defaultQuatenion.z_axis.deltaTheta = angleDeg;
        newCard->jointCube.trans.defaultQuatenion.y_axis.deltaTheta = -60.0f;

        // AuraJointCube
        newCard->AuraJointCube.trans.pos = pos;
        newCard->AuraJointCube.trans.defaultQuatenion.z_axis.deltaTheta = angleDeg;
        newCard->AuraJointCube.trans.defaultQuatenion.y_axis.deltaTheta = -60.0f;

        newCard->originalPos = pos;
        newCard->currentPos = pos;
        newCard->targetPos = pos;

        Vector3 centerPos = { centerX, centerY, z };
        Vector3 fromCenter = (pos - centerPos).Normalize();
        newCard->hoverOffsetDir = fromCenter;

        newCard->baseRotationY = -40.0f;
        newCard->baseRotationZ = angleDeg;

        cards.push_back(std::move(newCard));
    }
}

void CardManager::UpdateAll(const Matrix4& vpMat)
{
    for (auto& card : cards)
    {
        card->Update(vpMat);
    }
}

void CardManager::DrawAll()
{
    for (auto& card : cards)
    {
        card->Draw();
    }
}

void CardManager::UseSelectedCard()
{
    for (auto& card : cards) 
    {
        if (card->isSelected && !card->isUsed) 
        {
            card->isUsed = true;
            card->isSelected = false;
            break;
        }
    }
}

bool CardManager::HasSelectedCard() const
{
    for (const auto& card : cards)
    {
        if (!card->isUsed && card->isSelected)
        {
            return true;
        }
    }
    return false;
}

void CardManager::HandleClick(InGameMouse* inGameMouse, const Matrix4& vpMat, float clickRadius, float moveValueOnClick)
{
    if (!(NoviceLike::mouseState.rgbButtons[0] && !NoviceLike::preMouseState.rgbButtons[0])) return;

    int clickedCardIndex = -1;

    for (int i = 0; i < cards.size(); ++i)
    {
        if (inGameMouse->CollisionWithMouse3D(cards[i]->jointCube.trans.pos, clickRadius, vpMat)) 
        {
            clickedCardIndex = i;
            break;
        }
    }

    if (clickedCardIndex == -1) return;

    if (cards[clickedCardIndex]->isSelected) 
    {
        cards[clickedCardIndex]->isSelected = false;
        if (!cards[clickedCardIndex]->isUsed)
        {
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::disSelectCard), false, 50);
        }
        else
        {
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::usedCardSelect), false, 50);
        }
    }
    else 
    {
        for (auto& card : cards)
        {
            card->isSelected = false;
        }
        cards[clickedCardIndex]->isSelected = true;
        if (!cards[clickedCardIndex]->isUsed)
        {
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::cardChoose), false, 50);
        }
        else
        {
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::usedCardSelect), false, 50);
        }

        // 動かすブロックの移動値を外部に通知したい場合は、コールバックか外部への値設定が必要
        if (onCardSelected) 
        {
            onCardSelected(cards[clickedCardIndex]->GetMoveValue());
        }
    }
}

void CardManager::HandleHover(InGameMouse* inGameMouse, const Matrix4& vpMat, float hoverRadius, float hoverOffsetY)
{
    Card* hoveredCard = nullptr;

    // 初期化
    for (auto& card : cards)
    {
        card->targetPos = card->originalPos;
    }

    // 現在のホバー対象を探す
    for (auto& card : cards)
    {
		if (card->isUsed) continue; // 使用済みカードはホバー対象外
        if (inGameMouse->CollisionWithMouse3D(Vector3(card->originalPos.x, card->originalPos.y + 0.2f, card->originalPos.z), hoverRadius, vpMat))
        {
            hoveredCard = card.get();
            break;
        }
    }

    // ホバーが変化したときだけSEを再生
    if (hoveredCard != lastHoveredCard_)
    {
        if (hoveredCard != nullptr)
        {
            // SE再生
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::cardTouch), false, 70);
            // ここでホバーしたカードの回転角度を1回転分増やす
            hoveredCard->targetRotationY += 360.0f;
        }

        // ホバー状態更新
        lastHoveredCard_ = hoveredCard;
    }

    // ターゲット位置の設定
    if (hoveredCard)
    {
        hoveredCard->targetPos = hoveredCard->originalPos + Vector3{ 0.0f, hoverOffsetY, 0.0f };
    }

    // 色とテクスチャの更新
    for (auto& card : cards)
    {
        if (card->isUsed)
        {
            card->jointCube.color = { 128, 128, 128, 255 };
			card->isAura = false;
        }
        else if (card->isSelected)
        {
            card->jointCube.color = { 255, 255, 0, 255 };
			card->AuraJointCube.color = { 255, 255, 0, 60 };
        }
        else if (card.get() == hoveredCard)
        {
            card->jointCube.color = { 0, 50, 255, 255 };
            card->AuraJointCube.color = { 0, 255, 255, 60 };
        }
        else
        {
            card->jointCube.color = { 0, 50, 255, 255 };
            card->AuraJointCube.color = { 0, 255, 255, 60 };
        }

        card->jointCube.SynchronizeTexAndColor();
    }
}

Card* CardManager::GetSelectedCard() const
{
    for (const auto& card : cards) 
    {
        if (card->isSelected && !card->isUsed)
        {
            return card.get();
        }
    }
    return nullptr;
}