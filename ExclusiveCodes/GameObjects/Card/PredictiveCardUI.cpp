#include "PredictiveCardUI.h"
#include "../../../MyLib/Utilities/EasingFuctionStructures/EasingFunctions.h"

PredictiveCardUI::PredictiveCardUI()
{
    //ジョイントキューブの初期化
    int tex[6] = { 1,1,1,1,1,1 };
    jointCube.Initialize(1.0f, 1.0f, 0.1f, { -3.0f,0.5f,-5.0f }, tex);
    direction = MovableBlock::ArrowDirection::None;

}

void PredictiveCardUI::Initialize()
{
    moveValue = 0;
    isVisible = false;

    easingStartLength = -1.0f;
    easingTargetLength = 1.0f;
    easingDuration = 1.0f;
    easingTime = 0.0f;

    //カードの表面を変更
    jointCube.texHandle = (int)TagTexture::kNumbers;
    // スプライトシートUV
    float numSprites = 10.0f; // スプライトの数（カードの数）
    float numTexcoordMove = 1.0f / numSprites;
    float numDefault = numTexcoordMove * 0.5f;

    jointCube.local_surfaces[C_FRONT].AdjustForSpriteSheet(numSprites);
    jointCube.local_surfaces[C_FRONT].trans.uvPos.u = moveValue * numTexcoordMove + numDefault;

    jointCube.color = { 255,255,255,160 };
    jointCube.SynchronizeTexAndColor();

    jointCube.local_surfaces[C_BEYOND].texHandle = (int)TagTexture::kCardBack;
    jointCube.local_surfaces[C_LEFT].texHandle = (int)TagTexture::kWhite2x2;
    jointCube.local_surfaces[C_RIGHT].texHandle = (int)TagTexture::kWhite2x2;
    jointCube.local_surfaces[C_TOP].texHandle = (int)TagTexture::kWhite2x2;
    jointCube.local_surfaces[C_BOTTOM].texHandle = (int)TagTexture::kWhite2x2;

	jointCube.trans.scale.z = 0.01f;

    direction = MovableBlock::ArrowDirection::None;
}

void PredictiveCardUI::Set(int value, const Vector3& pos, MovableBlock::ArrowDirection dir)
{
    moveValue = value;
    position = pos;
	direction = dir;
    isVisible = true;

    jointCube.trans.pos = position;

    // スプライトシートUV更新
    float numSprites = 10.0f; // スプライトの数（カードの数）
    float numTexcoordMove = 1.0f / numSprites;
    float numDefault = numTexcoordMove * 0.5f;

    jointCube.local_surfaces[C_FRONT].AdjustForSpriteSheet(numSprites);
    jointCube.local_surfaces[C_FRONT].trans.uvPos.u = moveValue * numTexcoordMove + numDefault;

    jointCube.SynchronizeTexAndColor();
}

void PredictiveCardUI::Update(Matrix4 vpMat_)
{
    if (!isVisible) return;

	basePos = jointCube.trans.pos;

    // イージング時間の更新
    easingTime += 1.0f / 60.0f;  
    if (easingTime > easingDuration) {
        easingTime -= easingDuration; 
    }

    // イージング補間値の計算
    float t = easingTime / easingDuration;
    float arrowLength = Easing::EaseOutCubic(easingStartLength, easingTargetLength, t);
    float arrowScale = Easing::EaseOutCubic(1.0f, 2.0f, t);

	jointCube.trans.scale = { arrowScale, arrowScale, 0.01f };

    switch (direction)
    {
    case MovableBlock::ArrowDirection::Up:
		moveDirection = { 0.0f, 1.0f, 0.0f };
        break;
    case MovableBlock::ArrowDirection::Down:
		moveDirection = { 0.0f, -1.0f, 0.0f };
        break;
    case MovableBlock::ArrowDirection::Left:
		moveDirection = { -1.0f, 0.0f, 0.0f };
        break;
    case MovableBlock::ArrowDirection::Right:
		moveDirection = { 1.0f, 0.0f, 0.0f };
        break;
    }

    // 方向に応じて位置を動かす
    Vector3 offset = moveDirection * arrowLength;
    jointCube.trans.pos = basePos + offset;


    jointCube.Update(vpMat_);
}

void PredictiveCardUI::Draw()
{
    if (!isVisible) return;

    jointCube.Draw();
}