#include "Key.h"
#include "../../InGameController/InGameController.h"
#include "../../Audio/AudioHandle.h"
#include "../../Audio/AudioPlayer.h"
#include"../Player/Player.h"

Key::Key() {

    //==============================コライダーの設定===============================
    //Keyとして設定
    SetObjectID(ObjectID::Tag::kKey);
    //コライダーの設定
    cubeCollider.Set(1, 1, 1);

    //==============================コライダーの設定===============================

    //==============================キューブの初期化===============================
    chara.Initialize(&mainTrans);

    //==============================キューブの初期化===============================

}

void Key::SetPlayerAndCollison(Player* player) {

    this->player = player;
    //==============================コライダーの設定===============================
    //衝突関数オブジェクトの初期化
    collisionPlayer.Initialize(player);
}

//ゲームオブジェクトの更新処理
void Key::Update(Matrix4 vpMat_)
{
    if (isPlayable) {

        if (isGravity) {
            physic.AddGravity();


        }

        if (player->isGetKey) {

            //キーと当たった時の処理を記入　このアニメーションが終了後　isActiveがfalseになる
            Rotating();

            Scaling();
        }
    }

    if (collMap.isGround || collMap.isCeiling) {
        physic.velocity = { 0.0f,0.0f,0.0f };
        isGravity = false;
    } else {
        isGravity = true;
    }

    //速度を制限
    float limit = ChipBehavior::kBlockWidth * 0.5f;
    physic.AdjustVelocity({ limit ,limit ,limit });

    //速度の合成
    physic.velocity = physic.velocity +
        movableBlockDeltaPos * collMap.isOnMovableBlock* collMap.isGround;
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
    chara.Update(vpMat_);

    //IndexMapRecordsに記録する
    InGameController::RecordIndexPosition(mainTrans.GetWorldPos(), (int)TagChip::kKey);

}

void Key::Rotating() {

    if (rotateTimer == 1.0f) {
        return;
    }

    rotateTimer += 1.0f / 30.0f;

    if (rotateTimer > 1.0f) {
        rotateTimer = 1.0f;
        scaleTimer = 0.0f;
    }

    mainTrans.defaultQuatenion.y_axis.deltaTheta = Easing::EaseInExpo(0.0f, 720.0f, rotateTimer);


};


void Key::Scaling() {

    if (scaleTimer == 1.0f) {
        return;
    }

    scaleTimer += 1.0f / 60.0f;

    if (scaleTimer > 1.0f) {
        scaleTimer = 1.0f;
        //アクティブフラグをおろす
        isActive = false;
    }


    mainTrans.scale = Easing::EaseOutElastic(Vector3{ 1.0f,1.0f,1.0f }, Vector3{ 0.0f,0.0f,0.0f }, scaleTimer);


}

//ゲームオブジェクトの初期化
void Key::Initialize()
{

    //初期化
    isActive = true;
    //物理処理
    isGravity = false;
    //大きさや回転角を初期化
    mainTrans.scale = { 1.0f,1.0f,1.0f };
    mainTrans.defaultQuatenion.y_axis.deltaTheta = 0.0f;
    //回転タイマーを初期化
    rotateTimer = 0.0f;

    //それぞれの当たり判定
    collMap.isCeiling = false;
    collMap.isGround = false;
    collMap.isWall = false;
    collMap.isOnMovableBlock = false;

    physic.velocity = { 0.0f,0.0f,0.0f };

    rotateTimer = 0.0f;
    //拡縮タイマー
    scaleTimer = 1.0f;

}

//ゲームオブジェクトの描画
void Key::Draw()
{
    //キューブの描画
    chara.Draw();
}

//衝突反応関数をセットする
void Key::SetCollisionBackTable()
{
    collisionBackActivationMap[ObjectID::Tag::KPlayer] = { false, collisionPlayer };
}

//設定した衝突反応関数( collisionBackActivationMap[].func )の更新処理
void Key::CollisionBackUpdate()
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
void Key::CollisionPlayer::Initialize(Player* player)
{
    player_ = player;
}

//ぶつかった時の処理(1フレーム)
bool Key::CollisionPlayer::operator()()
{

    //キーをゲットしていないとき
    if (!player_->isGetKey)
    {
        player_->isGetKey = true;
        // SE再生
        AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::getKey), false, 10);
    }

    return false;

}



Key::~Key() {

}