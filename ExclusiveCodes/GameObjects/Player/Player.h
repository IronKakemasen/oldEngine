#pragma once
#include "../../Characters/RobotChara/RobotChara.h"
#include "../../../MyLib/Utilities/GameObjectStructures/GameObject/GameObject.h"

class Player :public GameObject
{

public:

    struct CollisionGoal
    {
        Player* player = nullptr;

        bool operator()();
        void Initialize(Player* player_);
    };

    enum STATE {

        IDLE = 0,//アイドリング loop
        MOVE = 1,//移動 loop
        JUMP = 2,//ジャンプ 一度
        FALLING = 3,//落ちる　loop
        DEAD = 4,//死亡時
        ENTER = 5,//入場
        GOAL = 6,//ゴール時

        STATES
    };

    enum DIRECTION {
        LEFT,
        RIGHT
    };

    //プレイヤーがゴールと衝突したときの関数オブジェクト
    CollisionGoal collisionGoal;
    //マップとの衝突に必要な変数
    CollisionMapInfo collisionInfo;
    //当たり判定に使用
    Vector3 prePos = { 0.0f,0.0f,0.0f };
    //乗っかているMovableBlockのdeltaPos
    Vector3 movableBlockDeltaPos = {};


    // 方向
    DIRECTION direction = DIRECTION::LEFT;
    float endTheta = 0.0f;
    float currentTheta = 0.0f;
    float rotateTimer = 0.0f;

    //状態
    STATE state = STATE::ENTER;
    float stateCount[STATES] = { 0.0f };

    //キューブのキャラクタ
    RobotChara chara;
    //物理処理
    Physics3D physicalMaterial;

    //移動スピード
    const float kSpeedX = 0.07f*0.75f;
    const float kJumpSpeedX = 0.075f * 0.625f * 0.65f;
    //ジャンプの音
    int jumpAudioData;
    //キーのゲット
    bool isGetKey = false;
    bool canMove = false;


    //ゲームオブジェクトの更新処理
    virtual void Update(Matrix4 vpMat_)override;
    //ゲームオブジェクトの初期化
    virtual void Initialize()override;
    //ゲームオブジェクトの描画
    virtual void Draw()override;
    //衝突反応関数をセットする
    virtual void SetCollisionBackTable()override;
    //設定した衝突反応関数( collisionBackActivationMap[].func )の更新処理
    virtual void CollisionBackUpdate()override;

    //入力移動処理
    void InputMove();
    //重力処理
    void GravityUpdate();
    //状態を変更する処理
    void StateChang();
    //状態のカウントを初期化する
    void InitStateCount();
    //状態のカウントを更新する
    void UpdateStateCount(STATE& state, float(&stateCount)[STATES],float fps,bool isLoop);
    void UpdateState();

    //旋回処理
    void Rotate();

    std::string GetCurrentStateInString();

    Player();
};

