#include "Player.h"
#include "../../Map/InGameMapBehavior.h"
#include "../../InGameController/InGameController.h"
#include"../../Audio/AudioPlayer.h"
#include"../../Audio/AudioHandle.h"
#include "../../Scenes/GameScene/GameScene.h"
#include "../MovableField/MovableField.h"


Player::Player()
{
    //==============================コライダーの設定===============================
    //Playerとして設定
    SetObjectID(ObjectID::Tag::KPlayer);
    //コライダーの設定
    cubeCollider.Set(1.0f, 1.0f, 1.0f);
    //衝突関数オブジェクトの初期化
    collisionGoal.Initialize(this);
    //==============================コライダーの設定===============================

    //音のデータをセットする
    jumpAudioData = AudioHandle::Get(AudioID::jump);

    //キューブの初期化
    chara.Initialize(&mainTrans);
}

//ゲームオブジェクトの更新処理
void Player::Update(Matrix4 vpMat_)
{
    //速度の初期化
    physicalMaterial.velocity.x = 0.0f;

    //地面にいる時と天井にいる時に速度を0にする
    if (collisionInfo.isGround || collisionInfo.isCeiling) {
        physicalMaterial.velocity.y = 0.0f;
    }

    //状態の変更処理
    StateChang();

    if (state != ENTER)
    {
        if (state != Player::STATE::GOAL)
        {
            //キー入力移動と状態変更

            if (!GameScene::isPerforming && canMove)

            {
                InputMove();
            }
        }
    }

    //旋回処理
    Rotate();


 
        //状態ごとにカウントを上げる
        UpdateState();
    


    //死亡時以外の時
    if (state != DEAD) {

        //重力処理
        GravityUpdate();

        //速度の合成
        physicalMaterial.velocity = physicalMaterial.velocity +
            movableBlockDeltaPos * collisionInfo.isOnMovableBlock * collisionInfo.isGround;

        //if (collisionInfo.isWall)
        //{
        //    physicalMaterial.velocity.x = 0.0f;
        //}

        //速度を制限
        float limit = ChipBehavior::kBlockWidth * 0.5f;
        physicalMaterial.AdjustVelocity({ limit ,limit ,limit });

        //移動量を代入
        mainTrans.pos = mainTrans.pos + physicalMaterial.velocity;

    }

    //==================================プレイヤーとマップの当たり判定を行う====================================================================
    //！！！！座標を動かした後！！！！
    collisionInfo.isCeiling = false;
    collisionInfo.isGround = false;
    collisionInfo.isWall = false;
    collisionInfo.isOnMovableBlock = false;


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
                cubeCollider.width, cubeCollider.height, physicalMaterial.velocity,
                collisionInfo, &(*itr_block)->trans.pos, (*itr_block)->trans.pos,
                ChipBehavior::kBlockWidth, ChipBehavior::kBlockHeight);
        }
    }

    canMove = true;
    int count = 0;
    //ムーバブルブロック
    for (auto itr_movable = InGameController::stages[InGameController::currentStageNo]->movableBlocks.begin();
        itr_movable != InGameController::stages[InGameController::currentStageNo]->movableBlocks.end(); ++itr_movable)
    {
        if ((*itr_movable)->isMoving) count++;

        IndexedPosition pIndex = InGameMapBehavior::GetIndexedPositionBasedInPosition(mainTrans.GetWorldPos());

        IndexedPosition blockIndex =
            InGameMapBehavior::GetIndexedPositionBasedInPosition((*itr_movable)->mainTrans.GetWorldPos());

        if (InGameMapBehavior::IsAround(&pIndex, &blockIndex, 1.0f))
        {
            CollisionDetections::Col2D::FixThisObjPosWhenCollided(&mainTrans.pos, prePos,
                cubeCollider.width, cubeCollider.height, physicalMaterial.velocity,
                collisionInfo, &(*itr_movable)->mainTrans.pos, (*itr_movable)->prePos,
                ChipBehavior::kBlockWidth, ChipBehavior::kBlockHeight, true);

            movableBlockDeltaPos = (*itr_movable)->mainTrans.pos - (*itr_movable)->prePos;

            int debug = 0;
            if (movableBlockDeltaPos.GetMagnitutde() > 0.0f)
            {
                debug = 0;
            }
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
                cubeCollider.width, cubeCollider.height, physicalMaterial.velocity,
                collisionInfo, &(*itr_block)->trans.pos, (*itr_block)->trans.pos,
                ChipBehavior::kBlockWidth, ChipBehavior::kBlockHeight);
        }
    }

    if (count > 0)
    {
        canMove = false;
    }

    //プレポズを測定
    prePos = mainTrans.pos;

    //==================================プレイヤーとマップの当たり判定を行う====================================================================

    //行列計算
    //！！！マップチップとの当たり判定を行った後！！！
    mainTrans.SetWVP_Mat(vpMat_);
    mainTrans.SetWorldMatBasedQuaternion();



    //キューブのキャラクタ

    chara.Update(vpMat_, state, stateCount[state]);




    //IndexMapRecordsに記録する
    int const rectIndexNo = 9;
    InGameController::RecordIndexPosition(mainTrans.GetWorldPos() +
        Vector3{ cubeCollider.width * 0.5f,0.0f,0.0f }, rectIndexNo);
    InGameController::RecordIndexPosition(mainTrans.GetWorldPos() +
        Vector3{ -cubeCollider.width * 0.5f,0.0f,0.0f }, rectIndexNo);

    InGameController::RecordIndexPosition(mainTrans.GetWorldPos(), (int)TagChip::kPlayer);

}

//ゲームオブジェクトの初期化
void Player::Initialize()
{
    //初期化
    isActive = true;

    isGetKey = false;
    //入場時の状態
    state = STATE::ENTER;
    //方向
    direction = DIRECTION::LEFT;
    //回転用の角度とタイマー
    endTheta = 0.0f;
    currentTheta = 0.0f;
    rotateTimer = 0.0f;

    //それぞれの状態のカウントを初期化する
    for (uint32_t i = 0; i < STATES; ++i) {
        stateCount[i] = 0.0f;
    }

    //速度を初期化する
    physicalMaterial.velocity = { 0.0f,0.0f,0.0f };

    for (int i = 0; i < (int)STATE::STATES; ++i)
    {
        stateCount[i] = 0.0f;
    }

}

//ゲームオブジェクトの描画
void Player::Draw()
{
    //キューブの描画
    chara.Draw();
}

//衝突反応関数をセットする
void Player::SetCollisionBackTable()
{
    collisionBackActivationMap[ObjectID::Tag::kGoal] = { false, collisionGoal };
}

//設定した衝突反応関数( collisionBackActivationMap[].func )の更新処理
void Player::CollisionBackUpdate()
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


void Player::GravityUpdate()
{

    ////重力を加算
    //if (!collisionInfo.isGround)
    //{
    //    physicalMaterial.AddGravity();
    //}

    physicalMaterial.AddGravity();

};


//状態のカウントを初期化する
void Player::InitStateCount() {
    stateCount[state] = 0.0f;
};


void Player::UpdateState() {

    switch (state)
    {
    case Player::IDLE:
        UpdateStateCount(state, stateCount, 120.0f, true);
        break;
    case Player::MOVE:
        UpdateStateCount(state, stateCount, 120.0f, true);
        break;
    case Player::JUMP:
        UpdateStateCount(state, stateCount, 30.0f, false);
        break;
    case Player::FALLING:
        UpdateStateCount(state, stateCount, 30.0f, false);
        break;
    case Player::DEAD:
        UpdateStateCount(state, stateCount, 120.0f, false);
        break;
    case Player::ENTER:
        UpdateStateCount(state, stateCount, 60.0f, false);
        break;
    case Player::GOAL:
        UpdateStateCount(state, stateCount, 120.0f, false);
        break;
    default:
        break;
    }

};

//状態ごとのアニメーションカウントを追加する
void Player::UpdateStateCount(STATE& state, float(&stateCount)[STATES], float fps, bool isLoop) {


    stateCount[state] += 1.0f / fps;

    if (stateCount[state] > 1.0f) {


        if (isLoop) {
            InitStateCount();
        } else {
            //ジャンプ中の時だけ特殊
            if (state == JUMP) {
                state = FALLING;
                stateCount[FALLING] = 0.0f;
            } else {

                if (state == ENTER) {
                    state = IDLE;
                }
                stateCount[state] = 1.0f;
            }

        }

    }



};

void Player::StateChang() {

    //Playableな時
    if (state != GOAL && state != DEAD && state != ENTER) {
        //地面についた時にアイドル状態にする
        if (state == JUMP || state == FALLING) {

            if (collisionInfo.isGround) {
                state = IDLE;
                InitStateCount();
            }

        } else {
            state = IDLE;
        }

        //一旦座標が一定ライン以下になったら状態を死亡にする
        Vector3 deathPos = InGameMap::GetPositionBasedInIndex(12, 0);
        if (mainTrans.pos.y <= deathPos.y-0.6f) {
            state = DEAD;
            InitStateCount();

        }

    }

}
void Player::Rotate() {

    if (rotateTimer == 1.0f) {
        return;
    }

    rotateTimer += 1.0f / 30.0f;

    if (rotateTimer > 1.0f) {
        rotateTimer = 1.0f;
    }
    mainTrans.defaultQuatenion.y_axis.deltaTheta = Easing::EaseOutCubic(currentTheta, endTheta, rotateTimer);

};


void Player::InputMove()
{
    //ゴールしていないとき

    if (NoviceLike::IsKeyPress(DIK_D)) {

        //ジャンプしているとき
        if (state == JUMP || state == FALLING) {
            physicalMaterial.velocity.x = kJumpSpeedX;
        } else {
            physicalMaterial.velocity.x = kSpeedX;
        }

        //アイドル状態の時
        if (state == IDLE) {
            state = MOVE;
            stateCount[IDLE] = 0.0f;
        }

        //方向を変化させる
        if (direction != RIGHT) {
            stateCount[MOVE] = 0.0f;
            currentTheta = mainTrans.defaultQuatenion.y_axis.deltaTheta;
            endTheta = 0.0f;
            rotateTimer = 0.0f;
            direction = RIGHT;
        }


    }

    if (NoviceLike::IsKeyPress(DIK_A)) {
        //ジャンプしているとき
        if (state == JUMP || state == FALLING) {
            physicalMaterial.velocity.x = -kJumpSpeedX;
        } else {
            physicalMaterial.velocity.x = -kSpeedX;
        }
        //アイドル状態の時
        if (state == IDLE) {
            state = MOVE;
            stateCount[IDLE] = 0.0f;
        }
        //方向を変化させる
        if (direction != LEFT) {
            stateCount[MOVE] = 0.0f;
            currentTheta = mainTrans.defaultQuatenion.y_axis.deltaTheta;
            endTheta = 180;
            rotateTimer = 0.0f;
            direction = LEFT;

        }
    }

    if (NoviceLike::IsKeyPress(DIK_W)) {
        // 地面にいる時
        if (collisionInfo.isGround && state != JUMP) {
            //ジャンプ音を鳴らす
            AudioPlayer::GetInstance().PlayAudio(jumpAudioData, false, 100);
            //ちからを加算する
            Vector3 const jumpForce = { 0.0f,0.08f,0.0f };
            physicalMaterial.AddForce(jumpForce);
            //状態をジャンプにする
            state = JUMP;
            //状態アニメーションカウントを初期化
            InitStateCount();

        }
    }


#ifdef _DEBUG

    if (NoviceLike::IsKeyPress(DIK_Q)) {

        //状態を死亡にする
        state = DEAD;
        //状態アニメーションカウントを初期化
        InitStateCount();

    }

#endif // _DEBUG



}

//ぶつかった時の処理(1フレーム)
bool Player::CollisionGoal::operator()()
{
    //プレイヤーがキーをゲットしていたら
    if (player->isGetKey) {
        if (player->state != Player::STATE::GOAL)
        {
            player->state = Player::STATE::GOAL;
        }

    }

    return false;
}

//ゴールと衝突したときの反応関数の初期化
void Player::CollisionGoal::Initialize(Player* player_)
{
    player = player_;

}

std::string Player::GetCurrentStateInString()
{
    std::string ret_state[(int)STATE::STATES]
    {
        "IDLE","MOVE","JUMP","FALLING","DEAD","ENTER","GOAL"
    };

    return ret_state[(int)state];
}
