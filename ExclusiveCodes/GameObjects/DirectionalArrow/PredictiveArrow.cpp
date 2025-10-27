#include "PredictiveArrow.h"
#include "../../Map/InGameMapBehavior.h"

PredictiveArrow::PredictiveArrow()
{
    //ジョイントキューブの初期化
    int tex[6] = { 1,1,1,1,1,1 };
    jointCube.Initialize(0.2f, 0.2f, 0.2f, { -3.0f,0.5f,-0.7f }, tex);
    //jointCube.drawMode = DrawMode::kWireFrame;
    jointCube.texHandle = (int)TagTexture::kWhite2x2;
    jointCube.color = { 255, 255, 255, 255 };
    jointCube.SynchronizeTexAndColor();
    direction = MovableBlock::ArrowDirection::None;

    //先端のモデルの作成
    sentan.model = MapModels::GetModelMusic(TagModel::kTriangle);
    sentan.trans.scale.z = 1.0f;

    sentan.trans.parent_trans = &jointCube.trans;
}

void PredictiveArrow::Initialize()
{
    jointCube.local_surfaces[C_TOP].leftTop.position.y = 0;
    jointCube.local_surfaces[C_TOP].rightTop.position.y = 0;
    jointCube.local_surfaces[C_TOP].rightBottom.position.y = 0;
    jointCube.local_surfaces[C_TOP].leftBottom.position.y = 0;
    arrowLength = 0.0f;
    easingTime = 0.0f;
    visible = false;
    direction = MovableBlock::ArrowDirection::None;

}

void PredictiveArrow::Update(Matrix4 vpMat)
{
    if (!visible) return;

    jointCube.Update(vpMat);
    sentan.Update(vpMat);
}

void PredictiveArrow::Draw() 
{
    if (!visible) return;
    jointCube.Draw(); 
    sentan.Draw();
}

void PredictiveArrow::SetDirectionAndPosition(MovableBlock::ArrowDirection dir, Vector3 startPos)
{
    direction = dir;
    position = startPos;
    visible = true;

    // 方向ごとの長さと角度設定
    switch (direction)
    {
    case MovableBlock::ArrowDirection::Up:
    case MovableBlock::ArrowDirection::Down:
        easingTargetLength = static_cast<float>(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
        break;
    case MovableBlock::ArrowDirection::Left:
    case MovableBlock::ArrowDirection::Right:
        easingTargetLength = static_cast<float>(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;
        break;
    }

    // JointCubeなどの描画情報の初期化
    jointCube.trans.pos = startPos;
    jointCube.trans.pos.z = -0.7f;
    // 各方向に応じて角度変更
    switch (direction)
    {
    case MovableBlock::ArrowDirection::Up:
        jointCube.trans.defaultQuatenion.z_axis.deltaTheta = 0.0f;

        break;
    case MovableBlock::ArrowDirection::Down:
        jointCube.trans.defaultQuatenion.z_axis.deltaTheta = 180.0f;
        break;
    case MovableBlock::ArrowDirection::Left:
        jointCube.trans.defaultQuatenion.z_axis.deltaTheta = 90.0f;
        break;
    case MovableBlock::ArrowDirection::Right:
        jointCube.trans.defaultQuatenion.z_axis.deltaTheta = -90.0f;
        break;
    }

    easingTime += 1.0f / 60.0f;
    if (easingTime > easingDuration) {
        easingTime -= easingDuration;
    }
    float t = easingTime / easingDuration;

    arrowLength = Easing::EaseOutCubic(easingStartLength, easingTargetLength, t);

    // Surfaceの設定
    jointCube.local_surfaces[C_TOP].leftTop.position.y = arrowLength;
    jointCube.local_surfaces[C_TOP].rightTop.position.y = arrowLength;
    jointCube.local_surfaces[C_TOP].rightBottom.position.y = arrowLength;
    jointCube.local_surfaces[C_TOP].leftBottom.position.y = arrowLength;

    sentan.trans.pos.y = arrowLength ;
}