#include "SelectStar.h"
#include "../../../MyLib/Utilities/EasingFuctionStructures/EasingFunctions.h"

SelectStar::SelectStar()
{
    //ジョイントキューブの初期化
    int tex[6] = { 1,1,1,1,1,1 };
    // ジョイントキューブの初期化
    jointCube.Initialize(1.5f, 1.5f, 0.0001f, { -3.0f,0.5f,-2.0f }, tex);
    jointCube.texHandle = (int)TagTexture::kSelectStar;
    jointCube.color = { 255,255,86,255 };
    jointCube.SynchronizeTexAndColor();

    //オーラジョイントキューブの初期化
    auraJointCube.Initialize(1.5f, 1.5f, 0.0001f, { -3.0f,0.5f,-2.0f }, tex);
    //カードのUVを変化させる
    auraJointCube.texHandle = (int)TagTexture::kSelectStar;
    auraJointCube.color = { 255, 255, 0, 60 };
    auraJointCube.SynchronizeTexAndColor();
}

void SelectStar::Initialize()
{

}

void SelectStar::Update(Matrix4 vpMat_)
{
    jointCube.Update(vpMat_);

    // オーラの時間更新
    easingTime += 1.0f / 60.0f;
    if (easingTime > easingDuration) {
        easingTime -= easingDuration;
    }

    float t = easingTime / easingDuration;

    for (int i = 0; i < 5; ++i)
    {
        float offset = i * 0.8f;
        float loopTime = fmodf(t + offset, 1.0f);

        float scaleXY = Easing::EaseOutCubic(1.0f, 1.25f, loopTime);
        float alpha = Easing::Lerp(0.3f, 0.0f, loopTime);

        auraJointCube.trans.scale = { scaleXY, scaleXY, 0.2f };
        auraJointCube.trans.pos = jointCube.trans.pos;
        auraJointCube.color.w = alpha * 255;
        auraJointCube.SynchronizeColor();
        auraJointCube.Update(vpMat_);
    }
}

void SelectStar::Draw()
{
    NoviceLike::SetBlendMode(kBlendModeAdd);

    // オーラ描画
    for (int i = 0; i < 5; ++i)
    {
        auraJointCube.Draw();
    }

    NoviceLike::SetBlendMode(kBlendModeNormal);
    
    jointCube.Draw(); 
}