#include "MovableBlock.h"
#include "../../InGameController/InGameController.h"
#include "../../Map/InGameMapBehavior.h"
#include "../../Audio/AudioHandle.h"
#include "../../Audio/AudioPlayer.h"

MovableBlock::MovableBlock()
{
    //オブジェクトの設定
    SetObjectID(ObjectID::Tag::kMovable);

    chara.Initialize(&mainTrans);
    chara.cube.color = { 255, 255, 255, 255 };
    chara.cube.texHandle = (int)TagTexture::kMovableBlock;
    chara.cube.SynchronizeTexAndColor();

}

void MovableBlock::Initialize()
{
	//初期化
	isActive = true;
    moveSpeed = 3.5f;
}

void MovableBlock::Update(Matrix4 vpMat_)
{
    // プレポジ登録
    prePos = mainTrans.pos;

    if (isMoving)
    {
        easingTime += 1.0f / 60.0f; // 60FPS前提
        float t = easingTime / easingDuration;
        if (t > 1.0f) t = 1.0f;

        // イージング適用（EaseOutQuint）
        mainTrans.pos = Easing::Lerp(startPosition, targetPosition, t);

        if (t >= 1.0f)
        {
            isMoving = false;
            // SE再生
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::movableMove), false, 60);
        }
    }

    // キャラ更新
    chara.Update(vpMat_);
    mainTrans.SetWorldMatBasedQuaternion();

    // IndexMapRecordsに記録
    InGameController::RecordIndexPosition(mainTrans.GetWorldPos(), (int)TagChip::kMovableBlock);

}

void MovableBlock::Draw()
{
	//キューブの描画
	chara.Draw();
}

void MovableBlock::Move(ArrowDirection dir, float distance)
{
    Vector3 currentPos = mainTrans.pos;

    switch (dir)
    {
    case ArrowDirection::Up:
    {
        float numSpace = 0;

        IndexedPosition indexPos = InGameMapBehavior::GetIndexedPositionBasedInPosition(currentPos);

        int nextTag;
        int next = -1;

        while (1)
        {
            if (indexPos.Y + next >= 0 && indexPos.Y + next < InGameMapBehavior::kNumBlockVertical - 1 &&
                indexPos.X >= 0 && indexPos.X < InGameMapBehavior::kNumBlockHorizontal - 1)
            {
                nextTag = InGameController::currentIndexPosRecords[indexPos.Y + next][indexPos.X];

                if (nextTag == (int)TagChip::kSpace && (nextTag != 9))
                {
                    numSpace++;
                }
                else if (nextTag == (int)TagChip::kBlock)
                {
                    break;
                }


                next--;
            }

            else
            {
                break;
            }

        }

        Max(numSpace, distance);

        targetPosition = currentPos + Vector3(0.0f, numSpace * ChipBehavior::kBlockWidth, 0.0f);

        break;
    }

    case ArrowDirection::Down:
    {
        float numSpace = 0;
        IndexedPosition indexPos = InGameMapBehavior::GetIndexedPositionBasedInPosition(currentPos);

        int nextTag;
        int next = 1;

        while (1)
        {
            if ((indexPos.Y + next) >= 0 && (indexPos.Y + next) < InGameMapBehavior::kNumBlockVertical - 1 &&
                indexPos.X >= 0 && indexPos.X < InGameMapBehavior::kNumBlockHorizontal-1)
            {
                nextTag = InGameController::currentIndexPosRecords[indexPos.Y + next][indexPos.X];

                if(nextTag == (int)TagChip::kSpace && (nextTag != 9))
                {
                    numSpace++;
                }
                else if (nextTag == (int)TagChip::kBlock)
                {
                    break;
                }

                next++;

            }
            else
            {
                break;
            }


        }


        Max(numSpace, distance);

        targetPosition = currentPos + Vector3(0.0f, -numSpace * ChipBehavior::kBlockHeight, 0.0f);

        break;
    }

    case ArrowDirection::Left:
    {
        float numSpace = 0;

        IndexedPosition indexPos = InGameMapBehavior::GetIndexedPositionBasedInPosition(currentPos);

        int nextTag;
        int next = -1;

        float numObj = 0;
        int nextUnderTagEqualSpace = 0;
        int nextUnderTag;

        while (1)
        {
            if (indexPos.Y >= 0 && indexPos.Y < InGameMapBehavior::kNumBlockVertical - 1 &&
                indexPos.X + next >= 0 && indexPos.X + next < InGameMapBehavior::kNumBlockHorizontal - 1)
            {
                nextTag = InGameController::currentIndexPosRecords[indexPos.Y ][indexPos.X + next];
                nextUnderTag = InGameController::currentIndexPosRecords[indexPos.Y + 1][indexPos.X + next];


                if ((nextUnderTag != (int)TagChip::kBlock) && nextUnderTag != (int)TagChip::kMovableBlock)
                {
                    nextUnderTagEqualSpace++;
                }

                if ((nextTag == (int)TagChip::kSpace) || (nextTag == 9))
                {
                    numSpace++;
                }

                else if (nextTag == (int)TagChip::kGoal || nextTag == (int)TagChip::kPlayer || nextTag == (int)TagChip::kKey)
                {
                    numObj++;
                }

                else if (nextTag == (int)TagChip::kBlock)
                {
                    if (numObj > 0.0f && nextUnderTagEqualSpace > 0)
                    {
                        numSpace++;
                    }


                    break;
                }


                next--;

            }
            else
            {
                if (numObj > 0.0f && nextUnderTagEqualSpace > 0)
                {
                    numSpace++;
                }


                break;
            }


        }
       
        Max(numSpace, distance);

        targetPosition = currentPos + Vector3(-numSpace * ChipBehavior::kBlockHeight, 0.0f, 0.0f);

        break;
    }

    case ArrowDirection::Right:
    {
        float numSpace = 0;

        IndexedPosition indexPos = InGameMapBehavior::GetIndexedPositionBasedInPosition(currentPos);

        int nextTag;
        int next = 1;
        float numObj = 0;
        int nextUnderTagEqualSpace = 0;
        int nextUnderTag;

        while (1)
        {
            if (indexPos.Y >= 0 && indexPos.Y < InGameMapBehavior::kNumBlockVertical - 1 &&
                indexPos.X + next >= 0 && indexPos.X + next < InGameMapBehavior::kNumBlockHorizontal - 1)
            {
                nextTag = InGameController::currentIndexPosRecords[indexPos.Y][indexPos.X + next];
                nextUnderTag = InGameController::currentIndexPosRecords[indexPos.Y + 1][indexPos.X + next];

                if ((nextUnderTag != (int)TagChip::kBlock)&& nextUnderTag != (int)TagChip::kMovableBlock)
                {
                    nextUnderTagEqualSpace++;
                }

                if (nextTag == (int)TagChip::kSpace || (nextTag == 9))
                {
                    numSpace++;

                }
                else if (nextTag == (int)TagChip::kGoal || nextTag == (int)TagChip::kPlayer || nextTag == (int)TagChip::kKey)
                {
                    numObj++;
                }

                else if (nextTag == (int)TagChip::kBlock)
                {
                    if (numObj > 0.0f && nextUnderTagEqualSpace > 0)
                    {
                        numSpace++;
                    }

                    break;
                }


                next++;

            }
            else
            {
                if (numObj > 0.0f && nextUnderTagEqualSpace > 0)
                {
                    numSpace++;
                }


                break;
            }

        }


        Max(numSpace, distance);

        targetPosition = currentPos + Vector3(numSpace * ChipBehavior::kBlockWidth, 0.0f, 0.0f);

        break;

    }

    }

    // 目標位置の設定
    isMoving = true;
    startPosition = mainTrans.pos;
    easingTime = 0.0f;

    // 距離に応じて時間を変える
    float easeDistance = (targetPosition - startPosition).Length();

    easingDuration = easeDistance / moveSpeed;

    // 最低時間を設定
    if (easingDuration < 0.1f) easingDuration = 0.1f;
    //移動後を登録
    //InGameController::RecordIndexPosition(mainTrans.GetWorldPos(), (int)TagChip::kMovableBlock);

}



bool MovableBlock::CanMoveByArrow(ArrowDirection dir, Vector3 value) const
{
    if (dir == ArrowDirection::Right || dir == ArrowDirection::Left) 
    {
        return mainTrans.pos.y > value.y;
    }
    else if (dir == ArrowDirection::Up || dir == ArrowDirection::Down) {
        return mainTrans.pos.x > value.x;
    }
    return true;
}

//衝突反応関数をセットする
void MovableBlock::SetCollisionBackTable()
{

}

//設定した衝突反応関数( collisionBackActivationMap[].func )の更新処理
void MovableBlock::CollisionBackUpdate()
{
	//キューブの描画
	chara.Draw();
}
