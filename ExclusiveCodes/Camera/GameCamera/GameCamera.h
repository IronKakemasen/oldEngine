#pragma once
#include "../../../MyLib/Utilities/CameraStructures/CameraBehavior.h"
#include <memory> 

class Player;

class GameCamera :public CameraBehavior
{
private:


public:

    enum Status
    {
        kZoomOut,
        kZoomIn,
        kFollow,
    };


    //Constructor
    GameCamera();
    //Destructor
    ~GameCamera() {}
    virtual void Update() override;
    virtual void Initialize() override;
    virtual void Debug()override {};

    //追従
    void FollowUpdate();
    //プレイ中
    void PlayingUpdate();
    //プレイ中初期化
    void InitPlaying();
    //更新処理切り替え
    void InitStartPosAndMoveTimer();
    //移動タイマーの更新処理
    void UpdateMoveTimer();
    //セレクトシーンから進入時はzoomOut
    void ZoomOut();
    //クリア時のズーム
    void ZoomIn();


    float* zoomCounter = nullptr;
    float* zoomInCounter = nullptr;

    static inline Status status = Status::kZoomIn;

    //static inline bool isFollow = true;
    Vector3* targetPosition = nullptr;
    float followSpeed = { 0.01f };
    //遊んでいる時の座標
    Vector3 playingPosition = { 0.0f,0.0f,0.0f };
    Vector3 startPosition = { 0.0f,0.0f,0.0f };
    static inline float moveTimer = 0.0f;
};

