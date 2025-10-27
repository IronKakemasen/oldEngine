#pragma once

#include "../../Characters/KeyCharacter/KeyCharacter.h"
#include "../../../MyLib/Utilities/GameObjectStructures/GameObject/GameObject.h"
class Player;

class Key :public GameObject
{

public:


    struct CollisionPlayer
    {
        Player* player_ = nullptr;
        bool operator()();
        void Initialize(Player* player);
    };


    CollisionPlayer collisionPlayer;
    //プレイヤーのポインタ
    Player* player;
    //キューブのキャラクタ
    KeyCharacter chara;
    //物理を使うか
    bool isGravity = false;
    Physics3D physic;
    CollisionMapInfo collMap;
    Vector3 prePos = { 0.0f,0.0f,0.0f };
    //遊べる時
    bool isPlayable = false;
    //回転タイマー
    float rotateTimer = 0.0f;
    //拡縮タイマー
    float scaleTimer = 1.0f;

    //乗っかているMovableBlockのdeltaPos
    Vector3 movableBlockDeltaPos = {};

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
    void Rotating();
    void Scaling();
    void SetPlayerAndCollison(Player* player);
    Key();
    ~Key();
};

