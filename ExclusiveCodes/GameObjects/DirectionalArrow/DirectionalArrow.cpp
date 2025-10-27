#include "DirectionalArrow.h"
#include "../../Map/InGameMapBehavior.h"
#include "../../../MyLib/Utilities/EasingFuctionStructures/EasingFunctions.h"
#include "../../InGameController/InGameController.h"
#include "../../Map/Chip/ChipBehavior.h"
#include "../../Audio/AudioHandle.h"
#include "../../Audio/AudioPlayer.h"

DirectionalArrow::DirectionalArrow()
{
    //ジョイントキューブの初期化
    int tex[6] = { 1,1,1,1,1,1 };
    jointCube.Initialize(0.2f, 0.2f, 0.2f, { -3.0f,0.5f,0.0f }, tex);
    //jointCube.drawMode = DrawMode::kWireFrame;
    jointCube.texHandle = (int)TagTexture::kWhite2x2;
    jointCube.color = { 255,255,255,255 };
    jointCube.SynchronizeTexAndColor();

    //先端のモデルの作成
    sentan.model = MapModels::GetModelMusic(TagModel::kTriangle);
    sentan.trans.parent_trans = &jointCube.trans;
}



void DirectionalArrow::Initialize(MovableField* movableField)
{
    this->movableField = movableField;


    value = 0;
    enabled = false; 
    direction = MovableBlock::ArrowDirection::Up;
    arrowLength = 0.0f;
    stretchAmount = 0.3f;  // 伸ばす量
    easingTime = 0.0f;           // 経過時間
    easingDuration = 0.7f;       // 伸ばす時間
    easingStartLength = 0.0f;    // 開始時の長さ
    easingTargetLength = 10.0f;   // 最大長さ

    hasTriggeredArrowInput = false;

    isEasing = false;
}

void DirectionalArrow::Update(Matrix4 vpMat_)
{
    if (!enabled) return; // 無効なら何もしない

    //=============================追加＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
    //いったんすべてのブロックのインデックスポジションを記録する
    for (auto& block : InGameController::stages[InGameController::currentStageNo]->movableBlocks)
    {
        //IndexMapRecordsに記録する
        InGameController::RecordIndexPosition(block->mainTrans.GetWorldPos(), (int)TagChip::kMovableBlock);
    }
    //=============================追加＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

    arrowLength += stretchAmount;

    if (!isEasing) {
        easingTime = 0.0f;
        easingStartLength = 0.0f;
        arrowLength = 0.0f;
        isEasing = true;

        switch (direction)
        {
        case MovableBlock::ArrowDirection::Up:
        case MovableBlock::ArrowDirection::Down:
            easingTargetLength = static_cast<float>(InGameMapBehavior::kNumBlockVertical - 22.0f) * ChipBehavior::kBlockHeight;
            break;
        case MovableBlock::ArrowDirection::Left:
        case MovableBlock::ArrowDirection::Right:
            easingTargetLength = static_cast<float>(InGameMapBehavior::kNumBlockHorizontal - 2.0f) * ChipBehavior::kBlockWidth;
            break;
        }
    }

    // フレームごとに時間を加算
    easingTime += 1.0f / 60.0f;  

    float t = easingTime / easingDuration;
    if (t > 1.0f) t = 1.0f;

    // Easingを適用
    arrowLength = Easing::EaseOutQuint (easingStartLength, easingTargetLength, t);
    if (t >= 1.0f)
    {
        isEasing = false;
        enabled = false;

        // 矢印の移動処理
        for (auto& block : targetBlocks)
        {
            if (!block->CanMoveByArrow(direction, startPosition))
                continue;

            block->Move(direction, value);
        }

        movableField->fieldLength = 0;
        movableField->enabled = true;
        movableField->isStretch = true;
        movableField->jointCube.color = { 0,0,255,188 };

        // SE再生
        AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::field), false, 60);

        switch (direction) {
        case MovableBlock::ArrowDirection::Up:
        case MovableBlock::ArrowDirection::Down:

            movableField->easingTargetLength = ((float)(InGameMapBehavior::kNumBlockHorizontal - 1) * ChipBehavior::kBlockWidth - jointCube.trans.pos.x);

            break;
        case MovableBlock::ArrowDirection::Left:
        case MovableBlock::ArrowDirection::Right:

            movableField->easingTargetLength = ((float)(InGameMapBehavior::kNumBlockVertical - 1) * ChipBehavior::kBlockHeight - jointCube.trans.pos.y);
            break;
        }

        return;
    }

    // 矢印の先端だけを伸ばす処理
    jointCube.local_surfaces[C_TOP].leftTop.position.y = arrowLength;
    jointCube.local_surfaces[C_TOP].rightTop.position.y = arrowLength;
    jointCube.local_surfaces[C_TOP].rightBottom.position.y = arrowLength;
    jointCube.local_surfaces[C_TOP].leftBottom.position.y = arrowLength;

    sentan.trans.pos.y = arrowLength;

    jointCube.Update(vpMat_);

    //sentan.trans.pos = ;
    //先端の更新処理
    sentan.Update(vpMat_);
}

void DirectionalArrow::SetMoveData(const std::vector<std::unique_ptr<MovableBlock>>& blocks, Vector3 arrowPos)
{
    targetBlocks.clear();
    for (const auto& block : blocks)
    {
        targetBlocks.push_back(block.get());  // raw pointer に変換して格納
    }

    startPosition = arrowPos;
}

void DirectionalArrow::SetPosition(const Vector3& pos)
{
	jointCube.trans.pos = pos;
}

void DirectionalArrow::SetDirection(MovableBlock::ArrowDirection dir)
{
	direction = dir;
}

void DirectionalArrow::Draw()
{
	if (!enabled) return;
	//ジョイントキューブの描画
	jointCube.Draw();
    //先端のモデルの描画
    sentan.Draw();
}