#include "Card.h"
#include "../../../MyLib/Utilities/EasingFuctionStructures/EasingFunctions.h"

Card::Card(float moveValue)
{
    this->moveValue = moveValue;


    //ジョイントキューブの初期化
    int tex[6] = { 1,1,1,1,1,1 };
    jointCube.Initialize(1.5f, 2.0f, 0.1f, { -3.0f,0.5f,-2.0f }, tex);

    //カードの表面を変更
    jointCube.texHandle = (int)TagTexture::kCardNumbers;
    jointCube.SynchronizeTexAndColor();


    jointCube.local_surfaces[C_BEYOND].texHandle = (int)TagTexture::kCardBack;
    jointCube.local_surfaces[C_LEFT].texHandle = (int)TagTexture::kWhite2x2;
    jointCube.local_surfaces[C_RIGHT].texHandle = (int)TagTexture::kWhite2x2;
    jointCube.local_surfaces[C_TOP].texHandle = (int)TagTexture::kWhite2x2;
    jointCube.local_surfaces[C_BOTTOM].texHandle = (int)TagTexture::kWhite2x2;

    //オーラジョイントキューブの初期化
    AuraJointCube.Initialize(1.5f, 2.0f, 0.01f, { -3.0f,0.5f,-2.0f }, tex);
    //カードのUVを変化させる
    AuraJointCube.texHandle = (int)TagTexture::kWhite2x2;

    AuraJointCube.SynchronizeTexAndColor();
}

void Card::Initialize(float moveVal)
{
  
    isSelected = false;

    // moveValue に応じてテクスチャを選択
    moveValue = moveVal;

    //カードのUVを変化させる
    //スプライトシートのスプライトの数
    float  numSprites = 9;
    //ローカルサーフェイスのuv座標を調整
    jointCube.local_surfaces[C_FRONT].AdjustForSpriteSheet(numSprites);
    float const numTexcoordMove = 1.0f / numSprites;
    float const numDefault = numTexcoordMove * 0.5f;
    jointCube.local_surfaces[C_FRONT].trans.uvPos.u = (moveValue - 1) * numTexcoordMove + numDefault;

    jointCube.local_surfaces[C_BEYOND].AdjustForSpriteSheet(numSprites);
    jointCube.local_surfaces[C_BEYOND].trans.uvPos.u = (moveValue - 1) * numTexcoordMove + numDefault;

    jointCube.color = { 255,0,0,255 };
    jointCube.SynchronizeTexAndColor();


    AuraJointCube.color = { 0, 255, 255, 60 };
    AuraJointCube.SynchronizeTexAndColor();

    isSelected = false;
    isUsed = false;
	isAura = true;
}

void Card::Update(Matrix4 vpMat_)
{
    // Lerpによる滑らか移動
    float lerpSpeed = 0.1f;
    // 位置の補間
    currentPos = Easing::Lerp(currentPos, targetPos, lerpSpeed);

    // 回転角度の補間
    currentRotationY = Easing::Lerp(currentRotationY, targetRotationY, lerpSpeed);

    // 位置反映
    jointCube.trans.pos = currentPos;
    AuraJointCube.trans.pos = currentPos;
	// 回転反映
    float totalRotationY = baseRotationY + currentRotationY;
    float totalRotationZ = baseRotationZ;

    jointCube.trans.defaultQuatenion.y_axis.deltaTheta = totalRotationY;
    jointCube.trans.defaultQuatenion.z_axis.deltaTheta = totalRotationZ;

    AuraJointCube.trans.defaultQuatenion.y_axis.deltaTheta = totalRotationY;
    AuraJointCube.trans.defaultQuatenion.z_axis.deltaTheta = totalRotationZ;

    // 更新
    jointCube.Update(vpMat_);
    AuraJointCube.Update(vpMat_);

    vpMat = vpMat_;
}

void Card::Draw()
{
    // 元の状態を保存
    float originalZ = originalPos.z;

    // 時間更新
    easingTime += 1.0f / 60.0f;
    if (easingTime > easingDuration) {
        easingTime -= easingDuration;
    }
    float t = easingTime / easingDuration;

    NoviceLike::SetBlendMode(kBlendModeAdd);

    if (isAura)
    {
        for (int i = 0; i < 5; ++i)
        {
            float offset = i * 0.4f;
            float loopTime = fmodf(t + offset, 1.0f);

            float scaleXY = Easing::EaseOutCubic(1.0f, 1.25f, loopTime);
            float alpha = Easing::Lerp(0.3f, 0.0f, loopTime);

            AuraJointCube.trans.scale = { scaleXY, scaleXY, 0.2f };

            AuraJointCube.color.w = alpha * 255;
            AuraJointCube.SynchronizeColor();

            AuraJointCube.Update(vpMat);
            AuraJointCube.Draw();
        }
    }

    NoviceLike::SetBlendMode(kBlendModeNormal);

    jointCube.Draw(); // 通常のカード
}