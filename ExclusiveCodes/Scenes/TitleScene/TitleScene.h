
#pragma once
#include "../../../MyLib/Utilities/SceneStructures/SceneBehavior.h"
#include "../../../MyLib/Utilities/CounterStructures/CounterAndTime.h"
#include "../../Camera/SampleCamera/SampleCamera.h"

#include "../../Effect/Particle/Particle.h"
#include "../../Effect/Particle/ParticleEmitter.h"
#include "../../../MyLib/Utilities/PrimitiveStructures/QuadMesh/QuadBehavior/Sprite/Sprite.h"
#include "../../Characters/GoalCharacter/GoalCharacter.h"

#define CARD_MAX 5

class TitleScene :public SceneBehavior
{
private:

    //カメラ
    std::unique_ptr<SampleCamera> mainCamera;
    //パーティクルシステム
    std::unique_ptr<ParticleSystem> particleSystem;
    //エミッター
    ParticleEmitter* emitter;
    //スプライト
    Sprite sprite;


    //ゴールの変数
    enum GoalState
    {
        CLOSE = 0,//閉じる
        UNLOCK = 1,//ロック解除
        OPEN = 2,//開く
        GOAL_STATES
    };

    //閉じている状態
    GoalState goalState = CLOSE;
    GoalCharacter goalCharacter;
    //オブジェクトのトランスフォーム
    Transform4 goalMainTrans;
    //カウンターのぎゃくすう
    CounterAndInvTime counterAndInvTime[GOAL_STATES] = { 0.0f };
    CounterAndInvTime cameraTimer = { 0.0f };


    ModelObject titleModel;
    ModelObject centerGearModel;
    ModelObject aroundGearModel;
    ModelObject keyModel;
    std::vector< std::unique_ptr<Quad>> cards;

    enum class FadeState
    {
        None,
        FadeIn,
        FadeOut,
        Complete,
    };

public:

    virtual void Update() override;
    virtual void Render() override;
    virtual void Initialize() override;
    virtual void Debug() override;

    TitleScene(MapTextures* texID_, MapModels* myModels_);

    void ModelCreate();
    void ModelInitalize();
    void ModelUpdate();
    void ModelDraw();

    int audioData1;

    FadeState fadeState = FadeState::FadeIn;
    float fadeAlpha = 1.0f;
    float fadeSpeed = 0.09f;

    Sprite mouseSprite;
};

