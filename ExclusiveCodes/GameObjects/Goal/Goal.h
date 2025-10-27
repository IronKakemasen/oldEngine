#pragma once
#include "../../Characters/GoalCharacter/GoalCharacter.h"
#include "../../../MyLib/Utilities/GameObjectStructures/GameObject/GameObject.h"

class Player;

class Goal :public GameObject
{

public:

    struct CollisionPlayer
    {
        Player* player_ = nullptr;
        Goal* goal_ = nullptr;
        bool operator()();
        void Initialize(Player* player,Goal* goal);
    };


    enum STATE
    {
        CLOSE = 0,//閉じる
        UNLOCK = 1,//ロック解除
        OPEN = 2,//開く
        STATES
    };

    //閉じている状態
    STATE state = CLOSE;
    float stateCount[STATES] = { 0.0f};

    //ゴールのキャラクタ
    GoalCharacter chara;
    //プレイヤーとの衝突
    CollisionPlayer collisionPlayer;
    Player* player_ = nullptr;
    //物理挙動
    Physics3D physic;
    //マップとの当たり判定
    CollisionMapInfo collMap;
    Vector3 prePos;
    //物理を使うか
    bool isGravity = false;
    //プレイできるかどうか
    bool isPlayable = false;

    //乗っかているMovableBlockのdeltaPos
    Vector3 movableBlockDeltaPos = {};

    void UpdateStateCount(STATE& state, float(&stateCount)[STATES], float fps, bool isLoop);

    void SetPlayerAndCollison(Player* player);
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
    Goal();

};

