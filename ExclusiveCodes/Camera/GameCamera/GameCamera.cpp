#include "GameCamera.h"
#include "../../GameObjects/Player/Player.h"
#include "../../Map/InGameMapBehavior.h"
#include "../../InGameController/InGameController.h"


GameCamera::GameCamera()
{
    Initialize();
    //仮置き座標

    playingPosition = { 6.0f,31.0f,-9.0f };
    trans.pos = playingPosition;
    trans.SetWorldMatBasedQuaternion();
    //viewProjectionMat = CreateViewProjectionMatrix();
    SendCameraInfomation();

}

//移動タイマーの更新処理
void  GameCamera::UpdateMoveTimer() {

    if (moveTimer == 1.0f) {
        return;
    }

    moveTimer += 1.0f / 30.0f;

    if (moveTimer > 1.0f) {

        moveTimer = 1.0f;

    }

};

void GameCamera::Update()
{
    switch (status)
    {
    case GameCamera::kZoomOut:
        ZoomOut();

        break;
    case GameCamera::kZoomIn:
        ZoomIn();

        break;
    case GameCamera::kFollow:
        //追尾する
        FollowUpdate();

        break;
    }


    trans.SetWorldMatBasedQuaternion();
    viewProjectionMat = CreateViewProjectionMatrix();

    SendCameraInfomation();
}

void GameCamera::PlayingUpdate() {

    UpdateMoveTimer();

    trans.pos = Easing::EaseOutExpo(startPosition, playingPosition, moveTimer);
    /*  trans.defaultQuatenion.x_axis.deltaTheta = Easing::EaseOutExpo(0.0f, 0.0f, moveTimer);*/

};

//更新処理切り替え
void  GameCamera::InitStartPosAndMoveTimer() {

    //始点に現在の位置を入れる
    startPosition = trans.pos;
    //タイマーを0にする
    moveTimer = 0.0f;

};


void GameCamera::InitPlaying() {

    InitStartPosAndMoveTimer();

}

void GameCamera::FollowUpdate() {


    Vector3 cameraPos = {
       playingPosition.x - 1.6f + targetPosition->x / 4.0f,
       playingPosition.y,
       playingPosition.z 
    };

    trans.pos = Easing::EaseOutCubic(trans.pos, cameraPos, followSpeed);

};

void GameCamera::Initialize()
{
    trans.defaultQuatenion.x_axis.deltaTheta = 0.0f;
}

void GameCamera::ZoomOut()
{
    Vector3 playerPos;
    float const farNum = -5.0f;

    int y = 0;

    for (auto& row : InGameController::stages[InGameController::currentStageNo]->tagData)
    {
        int x = 0;
        for (auto& element : row)
        {
            if (element == (int)TagChip::kPlayer)
            {
                playerPos = InGameMap::GetPositionBasedInIndex(y, x);
            }

            ++x;
        }

        y++;
    }

    playerPos.z += farNum;

    if (zoomCounter)
    {
        if (*zoomCounter <= 0.75f)
        {
            float n = 6.28f*(*zoomCounter*1.33333333f) *2.0f ;

            trans.pos.x = playerPos.x + sinf(n) * 0.02f;
            trans.pos.y = playerPos.y + cosf(n*0.75f ) * 0.02f;
            trans.pos.z = playerPos.z;

            
        }

        else
        {
            float n = (*zoomCounter - 0.75f) * 4.0f;

            trans.pos = Easing::EaseOutBack(playerPos, playingPosition, n);
        }
    }
}

void GameCamera::ZoomIn()
{
    Vector3 goalPos;
    float const farNum = -5.0f;

    int y = 0;

    for (uint32_t i = 0; i < InGameMap::kNumBlockVertical; ++i)
    {

        for (int k = 0; k < InGameMap::kNumBlockHorizontal; ++k)
        {
            if (InGameController::currentIndexPosRecords[i][k] == (int)TagChip::kGoal)
            {
                goalPos = InGameMap::GetPositionBasedInIndex(i, k);
            }

        }

    }

    //for (auto& row : InGameController::stages[InGameController::currentStageNo]->tagData)
    //{
    //    int x = 0;
    //    for (auto& element : row)
    //    {
    //        if (element == (int)TagChip::kGoal)
    //        {
    //            goalPos = InGameMap::GetPositionBasedInIndex(y, x);
    //        }

    //        ++x;
    //    }

    //    y++;
    //}

    goalPos.z += farNum;

    if (zoomInCounter)
    {
        if (*zoomInCounter <= 0.3f)
        {
            float n = *zoomInCounter * 3.333333333333333f;
            trans.pos = Easing::EaseOutBack(playingPosition, goalPos, n);            
        }

        else
        {
            float n = 6.28f*(*zoomInCounter - 0.3f) *2.0f * 1.25f;
            trans.pos.x = goalPos.x + sinf(n) * 0.04f;
            trans.pos.y = goalPos.y + cosf(n*0.5f)* 0.02f;

        }
    }

}