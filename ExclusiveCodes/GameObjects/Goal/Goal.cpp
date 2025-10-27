#include "Goal.h"
#include "../../InGameController/InGameController.h"
#include"../Player/Player.h"

//状態ごとのアニメーションカウントを追加する
void Goal::UpdateStateCount(STATE& state, float(&stateCount)[STATES], float fps, bool isLoop) {

    stateCount[state] += 1.0f / fps;

    if (stateCount[state] > 1.0f) {

        if (isLoop) {
            stateCount[state] = 0.0f;
        } else {

            stateCount[state] = 1.0f;
        }

    }

};

Goal::Goal() {

    //==============================コライダーの設定===============================
    //Goalとして設定
    SetObjectID(ObjectID::Tag::kGoal);
    //コライダーの設定
    cubeCollider.Set(1, 1, 1);
    //==============================コライダーの設定===============================

     //キューブの初期化
    chara.Initialize(&mainTrans);
}

void Goal::SetPlayerAndCollison(Player* player) {

    player_ = player;
    //==============================コライダーの設定===============================
    //衝突関数オブジェクトの初期化
    collisionPlayer.Initialize(player, this);
}

//ゲームオブジェクトの更新処理
void Goal::Update(Matrix4 vpMat_)
{
    if (isPlayable) {

        if (isGravity) {
            physic.AddGravity();
        }

    }

    if (collMap.isGround || collMap.isCeiling) {
        physic.velocity = { 0.0f,0.0f,0.0f };
        isGravity = false;
    } else {
        isGravity = true;
    }


    switch (state)
    {
    case Goal::CLOSE:
        UpdateStateCount(state, stateCount, 60.0f, true);

        break;
    case Goal::UNLOCK:
        UpdateStateCount(state, stateCount, 60.0f, false);

        if (stateCount[UNLOCK] == 1.0f) {
            state = OPEN;
            stateCount[OPEN] = 0.0f;
        }
        break;
    case Goal::OPEN:
        UpdateStateCount(state, stateCount, 60, false);

        break;

    }


    //速度を制限
    float limit = ChipBehavior::kBlockWidth * 0.5f;
    physic.AdjustVelocity({ limit ,limit ,limit });

    //速度の合成
    physic.velocity = physic.velocity +
        movableBlockDeltaPos * collMap.isOnMovableBlock * collMap.isGround;

    if (collMap.isWall)
    {
        physic.velocity.x = 0.0f;
    }

    //移動量を代入
    mainTrans.pos = mainTrans.pos + physic.velocity;


    //==================================プレイヤーとマップの当たり判定を行う====================================================================
    //！！！！座標を動かした後！！！！
    collMap.isCeiling = false;
    collMap.isGround = false;
    collMap.isWall = false;
    collMap.isOnMovableBlock = false;


    //普通のブロック
    for (auto itr_block = InGameController::stages[InGameController::currentStageNo]->blocks.begin();
        itr_block != InGameController::stages[InGameController::currentStageNo]->blocks.end(); ++itr_block)
    {
        IndexedPosition pIndex = InGameMapBehavior::GetIndexedPositionBasedInPosition(mainTrans.GetWorldPos());

        IndexedPosition blockIndex =
            InGameMapBehavior::GetIndexedPositionBasedInPosition((*itr_block)->trans.GetWorldPos());

        if (InGameMapBehavior::IsAround(&pIndex, &blockIndex, 1.0f))
        {
            CollisionDetections::Col2D::FixThisObjPosWhenCollided(&mainTrans.pos, prePos,
                cubeCollider.width, cubeCollider.height, physic.velocity,
                collMap, &(*itr_block)->trans.pos, (*itr_block)->trans.pos,
                ChipBehavior::kBlockWidth, ChipBehavior::kBlockHeight);
        }
    }

    //ムーバブルブロック
    for (auto itr_movable = InGameController::stages[InGameController::currentStageNo]->movableBlocks.begin();
        itr_movable != InGameController::stages[InGameController::currentStageNo]->movableBlocks.end(); ++itr_movable)
    {
        IndexedPosition pIndex = InGameMapBehavior::GetIndexedPositionBasedInPosition(mainTrans.GetWorldPos());

        IndexedPosition blockIndex =
            InGameMapBehavior::GetIndexedPositionBasedInPosition((*itr_movable)->mainTrans.GetWorldPos());

        if (InGameMapBehavior::IsAround(&pIndex, &blockIndex, 1.0f))
        {
            CollisionDetections::Col2D::FixThisObjPosWhenCollided(&mainTrans.pos, prePos,
                cubeCollider.width, cubeCollider.height, physic.velocity,
                collMap, &(*itr_movable)->mainTrans.pos, (*itr_movable)->prePos,
                ChipBehavior::kBlockWidth, ChipBehavior::kBlockHeight, true);

            movableBlockDeltaPos = (*itr_movable)->mainTrans.pos - (*itr_movable)->prePos;

        }
    }

    //普通のブロック
    for (auto itr_block = InGameController::stages[InGameController::currentStageNo]->blocks.begin();
        itr_block != InGameController::stages[InGameController::currentStageNo]->blocks.end(); ++itr_block)
    {
        IndexedPosition pIndex = InGameMapBehavior::GetIndexedPositionBasedInPosition(mainTrans.GetWorldPos());

        IndexedPosition blockIndex =
            InGameMapBehavior::GetIndexedPositionBasedInPosition((*itr_block)->trans.GetWorldPos());

        if (InGameMapBehavior::IsAround(&pIndex, &blockIndex, 1.0f))
        {
            CollisionDetections::Col2D::FixThisObjPosWhenCollided(&mainTrans.pos, prePos,
                cubeCollider.width, cubeCollider.height, physic.velocity,
                collMap, &(*itr_block)->trans.pos, (*itr_block)->trans.pos,
                ChipBehavior::kBlockWidth, ChipBehavior::kBlockHeight);
        }
    }


    //プレポズを測定
    prePos = mainTrans.pos;
    //===============================マップとの当たり判定処理==================================


    mainTrans.SetWVP_Mat(vpMat_);
    mainTrans.SetWorldMatBasedQuaternion();

    //キューブのキャラクタ
    chara.Update(vpMat_, state, stateCount[state]);

    //IndexMapRecordsに記録する
    InGameController::RecordIndexPosition(mainTrans.GetWorldPos(), (int)TagChip::kGoal);

}

//ゲームオブジェクトの初期化
void Goal::Initialize()
{

    //初期化
    isActive = true;
    //物理処理
    isGravity = false;
    //それぞれの当たり判定
    collMap.isCeiling = false;
    collMap.isGround = false;
    collMap.isWall = false;
    collMap.isOnMovableBlock = false;

    physic.velocity = { 0.0f,0.0f,0.0f };

    state = CLOSE;
    for (int i = 0; i < STATES; ++i) {
        stateCount[i] = 0.0f;
    }
}

//ゲームオブジェクトの描画
void Goal::Draw()
{
    //キューブの描画
    chara.Draw();
}

//衝突反応関数をセットする
void Goal::SetCollisionBackTable()
{
    collisionBackActivationMap[ObjectID::Tag::KPlayer] = { false, collisionPlayer };

}

//設定した衝突反応関数( collisionBackActivationMap[].func )の更新処理
void Goal::CollisionBackUpdate()
{
    for (auto& [key, value] : collisionBackActivationMap)
    {
        if (!value.func) continue;

        if (value.isActive)
        {
            value.isActive = value.func();
        }
    }
}

//ゴールと衝突したときの反応関数の初期化
void Goal::CollisionPlayer::Initialize(Player* player, Goal* goal)
{
    player_ = player;
    goal_ = goal;
}

//ぶつかった時の処理(1フレーム)
bool Goal::CollisionPlayer::operator()()
{

    //キーをゲットしている時
    if (player_->isGetKey)
    {
        if (goal_->state == CLOSE) {
            goal_->state = UNLOCK;

            goal_->stateCount[UNLOCK] = 0.0f;

        }


    }

    return false;

}


