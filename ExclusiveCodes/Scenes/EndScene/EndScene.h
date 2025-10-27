#pragma once


#pragma once
#include "../../../MyLib/Utilities/SceneStructures/SceneBehavior.h"
#include "../../Camera/SampleCamera/SampleCamera.h"

#include "../../Effect/Particle/Particle.h"
#include "../../Effect/Particle/ParticleEmitter.h"
#include "../../../MyLib/Utilities/PrimitiveStructures/QuadMesh/QuadBehavior/Sprite/Sprite.h"


class EndScene :public SceneBehavior
{
private:

    //カメラ
    std::unique_ptr<SampleCamera> mainCamera;
    //パーティクルシステム
    std::unique_ptr<ParticleSystem> particleSystem;
    //エミッター
    ParticleEmitter* emitter;
    //サンプルスプライト
    Sprite sampleSprite;

public:

    virtual void Update() override;
    virtual void Render() override;
    virtual void Initialize() override;
    virtual void Debug() override;

    EndScene(MapTextures* texID_, MapModels* myModels_);

    int audioData1;
};

