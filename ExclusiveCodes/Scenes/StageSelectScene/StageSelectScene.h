#pragma once
#include "../../../MyLib/Utilities/SceneStructures/SceneBehavior.h"
#include "../../../MyLib/Utilities/CameraStructures/SphericalDebugCamera.h"
#include "../../Characters/RobotChara/RobotChara.h"
#include "../../InGameMouse/InGameMouse.h"
#include "../../Camera/SampleCamera/SampleCamera.h"
#include "../../GameObjects/Card/Card.h"
#include "../../GameObjects/SelectStar/SelectStar.h"
#include "../../Effect/Particle/Particle.h"
#include "../../Effect/Particle/ParticleEmitter.h"

struct StageSelectScene :public SceneBehavior
{
    //デバッグ用カメラ
    InGameMouse mouse;
    SampleCamera camera;

    enum class Difficulty
    {
        Easy,
        Normal,
       /* Hard*/
    };

    enum class FadeState {
        None,
        FadeIn,
        FadeOut,
        Complete,
    };

public:

    std::vector<std::unique_ptr<Card>> cards;
    std::vector<std::unique_ptr<SelectStar>> selectStars;

    std::unique_ptr<ParticleEmitter> emitter;
    std::unique_ptr<ParticleSystem> particleSystem;
    std::unique_ptr<InGameMouse> inGameMouse;

    JointedCube cursorCube;

    // カードの間隔
    const float cardSpacing = 2.5f;
    // 行（y方向）オフセット
    const float rowOffset = 5.0f;
    int selectedRow = 1;  // 0: Hard, 1: Normal, 2: Easy
    int selectedCol[2] = { 0, 0 };  // 各難易度の現在の選択列（横インデックス）
    int stageCounts[2] = { 4, 5 }; 

    int easyStartID;
    int normalStartID;       
    int hardStartID;

    Card* prevHoveredCard = nullptr;

    virtual void Update() override;
    virtual void Render() override;
    virtual void Initialize() override;
    void AddCards(int count, int row, Difficulty difficulty, int stageID);
    virtual void Debug() override;


    StageSelectScene(MapTextures* texID_, MapModels* myModels_);
    Card* GetSelectedCard();

    FadeState fadeState = FadeState::FadeIn;  
    float fadeAlpha = 1.0f;                   
    float fadeSpeed = 0.13f;

    //スプライト
    Sprite sprite;
    Sprite stageSprite;
};
