#pragma once
#include "../../../MyLib/Utilities/SceneStructures/SceneBehavior.h"
#include "../../Camera/GameCamera/GameCamera.h"

#include "../../GameObjects/Player/Player.h"
#include "../../Effect/Particle/Particle.h"
#include "../../Effect/Particle/ParticleEmitter.h"
#include "../../../MyLib/Utilities/PrimitiveStructures/QuadMesh/QuadBehavior/Sprite/Sprite.h"
#include "../../InGameController/InGameController.h"
#include "../../Map/MapIncluder.h"
#include "../../../MyLib/Utilities/CameraStructures/SphericalDebugCamera.h"
#include "../../GameObjects/Key/Key.h"
#include "../../../MyLib/Utilities/MouseStructures/MouseBehavior.h"
#include "../../InGameMouse/InGameMouse.h"
#include "../../GameObjects/Card/CardManager.h"
#include "../../GameObjects/DirectionalArrow/DirectionalArrow.h"
#include "../../GameObjects/DirectionalArrow/ArrowClickAreaManager.h"
#include "../../GameObjects/DirectionalArrow/PredictiveArrow.h"
#include "../../GameObjects/Goal/Goal.h"
#include "../../GameObjects/Card/PredictiveCardUI.h"


class GameScene :public SceneBehavior
{
private:

    std::unique_ptr<GameCamera> mainCamera;
    std::unique_ptr<InGameController> inGameController;
    std::unique_ptr<Player> player;
    std::unique_ptr<Goal> goal;
    std::unique_ptr<Key> key;
    std::unique_ptr<ParticleSystem> particleSystem;
    std::unique_ptr<InGameMouse> inGameMouse;
    std::unique_ptr<DirectionalArrow> directionalArrow;
    std::unique_ptr<MovableField> movableField;
    std::unique_ptr<MovableField> predictiveField;
    std::unique_ptr<CardManager> cardManager;
    std::unique_ptr<ArrowClickAreaManager> arrowClickAreaManager;

    std::unique_ptr<Map0> map0;
    std::unique_ptr<Map1> map1;
    std::unique_ptr<Map2> map2;
    std::unique_ptr<Map3> map3;
    std::unique_ptr<Map4> map4;
    std::unique_ptr<Map5> map5;
    std::unique_ptr<Map6> map6;
    std::unique_ptr<Map7> map7;
    std::unique_ptr<Map8> map8;
    std::unique_ptr<Map9> map9;
    std::unique_ptr<Map10> map10;
    std::unique_ptr<Map11> map11;
    std::unique_ptr<Map12> map12;
    std::unique_ptr<Map13> map13;
    std::unique_ptr<Map14> map14;


    std::unique_ptr<PredictiveArrow> predictiveArrow;
    std::unique_ptr<PredictiveCardUI> predictiveCardUI;

    std::unique_ptr<ParticleEmitter> backGroundEmitter;
    std::unique_ptr<ParticleEmitter> goalEmitter;
    std::unique_ptr<ParticleEmitter> keyEmitter;
    std::unique_ptr<ParticleEmitter> clearEmitter;
    std::unique_ptr<ParticleEmitter> playerIdleEmitter;
    std::unique_ptr<ParticleEmitter> getKeyEmitter;

    float commonScale = 1.0f;
    CounterAndInvTime shrinkCounter;

    //デバッグ用カメラ
    SphericalDebugCamera debugCamera;
    Transform4 debugCameraParent;
    bool cameraChanger = false;

    CameraBehavior* currentCamera = nullptr;

public:

    virtual void Update() override;
    virtual void Render() override;
    virtual void Initialize() override;
    virtual void Debug() override;
    void UpdateCard();
    void UpdateMovableBlock();
    void ActivatePredictCube();
    void ShrinkBlocks();
    void UpdateParticle();

    GameScene(MapTextures* texID_, MapModels* myModels_);
    
    static inline bool cardTargeted = false;
    static inline bool isPerforming = false;
    int audioDataBGM;

    // カードの配置パラメータ
    Vector3 directionalArrowPos = { 1.0f,1.0f,0.0f };
};
