#include "MovableField.h"
#include "../../Map/InGameMapBehavior.h"
#include "../../Map/Chip/ChipBehavior.h"

MovableField::MovableField()
{
    //ジョイントキューブの初期化
    int tex[6] = { 1,1,1,1,1,1 };
    jointCube.Initialize(1.0f, 1.0f, 1.2f, { -3.0f,0.5f,.0f }, tex);
    //jointCube.drawMode = DrawMode::kWireFrame;
    jointCube.texHandle = (int)TagTexture::kWhite2x2;
    jointCube.SynchronizeTexAndColor();
}

void MovableField::Initialize()
{
    jointCube.color = { 0,0,255,188 };
    jointCube.SynchronizeTexAndColor();
    fieldLength = 0.0f;

    enabled = false; // 領域が有効なときだけ判定
    isStretch = false;

    isFading = false;
    fadeWaitTimer = 0.0f;
    fadeOutTimer = 0.0f;
    kFadeWaitDuration = 0.1f;      // 待機時間
    kFadeOutDuration = 1.0f;       // フェード時間

    easingTime = 0.0f;           // 経過時間
    easingDuration = 0.7f;       // 伸ばす時間
    easingStartLength = 0.0f;    // 開始時の長さ
    easingTargetLength = 10.0f;   // 最大長さ
    isEasing = false;
    stretchAmount = 0.2f;  // 伸ばす量

    palseTimer = 0.0f;
}

void MovableField::Update(Matrix4 vpMat_)
{
    if (!enabled) return;

    bool justFinished = false;
    if (isEasing) 
    {
        easingTime += 1.0f / 60.0f;
        float t = easingTime / easingDuration;
        if (t > 1.0f) t = 1.0f;

        jointCube.color.w = Easing::EaseOutQuint((float)0, (float)188, t);
        jointCube.SynchronizeColor();

        if (t >= 1.0f) {
            isEasing = false;
            justFinished = true;  // イージング終了フラグ
        }
    }
    else
    {
        // フェード処理
        if (!isFading) {
            fadeWaitTimer += 1.0f / 60.0f;
            if (fadeWaitTimer >= kFadeWaitDuration) {
                isFading = true;
                fadeWaitTimer = 0.0f;
            }
        }
        else {
            fadeOutTimer += 1.0f / 60.0f;
            float t = fadeOutTimer / kFadeOutDuration;
            t = std::clamp(t, 0.0f, 1.0f);

            jointCube.color.w = static_cast<uint8_t>(188 * (1.0f - t));
            jointCube.SynchronizeTexAndColor();

            if (t >= 1.0f) {
                enabled = false;
                isFading = false;
                fadeOutTimer = 0.0f;
            }
        }
    }

    // 伸びる方向に合わせて面を動かす
    jointCube.local_surfaces[C_TOP].leftTop.position.y = easingTargetLength;
    jointCube.local_surfaces[C_TOP].rightTop.position.y = easingTargetLength;
    jointCube.local_surfaces[C_TOP].rightBottom.position.y = easingTargetLength;
    jointCube.local_surfaces[C_TOP].leftBottom.position.y = easingTargetLength;

    jointCube.Update(vpMat_);

    // イージング完了時のコールバック
    if (justFinished && onEasingFinished) {
        onEasingFinished();
    }
}

void MovableField::Activate(MovableBlock::ArrowDirection dir, const Vector3& pos)
{


    direction = dir;
    SetPosition(pos);

    fieldLength = 0.0f;

    // フェード状態をリセット
    isFading = false;
    fadeWaitTimer = 0.0f;
    fadeOutTimer = 0.0f;

    easingTime = 0.0f;
    easingStartLength = 0.0f;
    isEasing = true;

    // アルファ値を元に戻す
    jointCube.color = { 0, 0, 255, 188 };
    jointCube.SynchronizeTexAndColor();

    // 頂点のリセット
    for (int i = 0; i < 2; ++i)
    {
        jointCube.local_surfaces[i].leftTop.position.y = 0.0f;
        jointCube.local_surfaces[i].rightTop.position.y = 0.0f;
        jointCube.local_surfaces[i].leftBottom.position.y = 0.0f;
        jointCube.local_surfaces[i].rightBottom.position.y = 0.0f;
        jointCube.local_surfaces[i].leftTop.position.x = 0.0f;
        jointCube.local_surfaces[i].rightTop.position.x = 0.0f;
        jointCube.local_surfaces[i].leftBottom.position.x = 0.0f;
        jointCube.local_surfaces[i].rightBottom.position.x = 0.0f;
    }

}

void MovableField::ActivatePredictive(MovableBlock::ArrowDirection dir, const Vector3& pos, Matrix4 vpMat_)
{
    direction = dir;
    
    fieldLength = 0.0f;

    // 頂点のリセット
    for (int i = 0; i < 2; ++i)
    {
        jointCube.local_surfaces[i].leftTop.position.y = 0.0f;
        jointCube.local_surfaces[i].rightTop.position.y = 0.0f;
        jointCube.local_surfaces[i].leftBottom.position.y = 0.0f;
        jointCube.local_surfaces[i].rightBottom.position.y = 0.0f;
        jointCube.local_surfaces[i].leftTop.position.x = 0.0f;
        jointCube.local_surfaces[i].rightTop.position.x = 0.0f;
        jointCube.local_surfaces[i].leftBottom.position.x = 0.0f;
        jointCube.local_surfaces[i].rightBottom.position.x = 0.0f;
    }

    switch (direction) 
    {
    case MovableBlock::ArrowDirection::Up:
        easingTargetLength = ((float)(InGameMapBehavior::kNumBlockHorizontal - 1) * ChipBehavior::kBlockWidth - jointCube.trans.pos.x);
        jointCube.trans.defaultQuatenion.z_axis.deltaTheta = -90.0f;
        jointCube.local_surfaces[C_TOP].leftTop.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
        jointCube.local_surfaces[C_TOP].leftBottom.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
        jointCube.local_surfaces[C_BOTTOM].leftTop.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
        jointCube.local_surfaces[C_BOTTOM].leftBottom.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
        SetPosition({ pos.x , ((float)InGameMapBehavior::kNumBlockVertical - 11.0f) * ChipBehavior::kBlockHeight, -0.7f });
        break;
    case MovableBlock::ArrowDirection::Down:
        easingTargetLength = ((float)(InGameMapBehavior::kNumBlockHorizontal - 1) * ChipBehavior::kBlockWidth - jointCube.trans.pos.x);
        jointCube.trans.defaultQuatenion.z_axis.deltaTheta = -90.0f;
        jointCube.local_surfaces[C_TOP].leftTop.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
        jointCube.local_surfaces[C_TOP].leftBottom.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
        jointCube.local_surfaces[C_BOTTOM].leftTop.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
        jointCube.local_surfaces[C_BOTTOM].leftBottom.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
        SetPosition({ pos.x , ((float)InGameMapBehavior::kNumBlockVertical - 11.0f) * ChipBehavior::kBlockHeight, -0.7f });
        break;
    case MovableBlock::ArrowDirection::Left:
        jointCube.trans.defaultQuatenion.z_axis.deltaTheta = 0.0f;

        easingTargetLength = ((float)(InGameMapBehavior::kNumBlockVertical - 1) * ChipBehavior::kBlockHeight - jointCube.trans.pos.y);
        jointCube.local_surfaces[C_TOP].rightTop.position.x = ((float)(InGameMapBehavior::kNumBlockHorizontal - 2)) * ChipBehavior::kBlockWidth;
        jointCube.local_surfaces[C_TOP].rightBottom.position.x = (float)(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;
        jointCube.local_surfaces[C_BOTTOM].rightTop.position.x = (float)(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;
        jointCube.local_surfaces[C_BOTTOM].rightBottom.position.x = (float)(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;
        SetPosition({ 1.0f, pos.y, -0.7f });
        break;
    case MovableBlock::ArrowDirection::Right:
        jointCube.trans.defaultQuatenion.z_axis.deltaTheta = 0.0f;
        
        easingTargetLength = ((float)(InGameMapBehavior::kNumBlockVertical - 1) * ChipBehavior::kBlockHeight - jointCube.trans.pos.y);
        jointCube.local_surfaces[C_TOP].rightTop.position.x = ((float)(InGameMapBehavior::kNumBlockHorizontal - 2)) * ChipBehavior::kBlockWidth;
        jointCube.local_surfaces[C_TOP].rightBottom.position.x = (float)(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;
        jointCube.local_surfaces[C_BOTTOM].rightTop.position.x = (float)(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;
        jointCube.local_surfaces[C_BOTTOM].rightBottom.position.x = (float)(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;
        SetPosition({ ChipBehavior::kBlockWidth, pos.y, -0.7f });
        break;
    }

    easingTime = 0.0f;
    easingStartLength = 0.0f;
 
    isEasing = true;
    enabled = true;

    jointCube.color = { 0, 255, 255, 30};
	jointCube.texHandle = (int)TagTexture::kWhite2x2;

    if (enabled) {
        float pulse = (std::sin(palseTimer * 2.0f * 3.1415f) + 1.0f) * 0.5f; // 0〜1 の範囲
        jointCube.color.w = static_cast<uint8_t>(20+ 40 * pulse); // α: 80〜120
        palseTimer += 1.0f / 60.0f;
    }
    jointCube.SynchronizeTexAndColor();

    // 頂点
    jointCube.local_surfaces[C_TOP].leftTop.position.y = easingTargetLength;
    jointCube.local_surfaces[C_TOP].rightTop.position.y = easingTargetLength;
    jointCube.local_surfaces[C_TOP].rightBottom.position.y = easingTargetLength;
    jointCube.local_surfaces[C_TOP].leftBottom.position.y = easingTargetLength;

    jointCube.Update(vpMat_);
}

void MovableField::SetPosition(const Vector3& pos)
{
    jointCube.trans.pos = pos;
}

void MovableField::SetDirection(MovableBlock::ArrowDirection dir)
{
    direction = dir;
}

void MovableField::Draw()
{
    if (!enabled) return;
    //ジョイントキューブの描画
    jointCube.Draw();
}