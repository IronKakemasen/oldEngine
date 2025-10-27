#pragma once
#include "../../../MyLib/Utilities/BaseClassStructures/Transform/Transform.h"
#include "../../../MyLib/Utilities/PrimitiveStructures/QuadMesh/QuadBehavior/Cube/JointedCube/JointedCube.h"
#include "../../Utilities/SoheStructures.h"
#include "../../Utilities/SoheEasing.h"

#include <wrl.h>
#include <d3d12.h>
#include <vector>
#include <array>
#include <fstream>

class ParticleEmitter;

enum class ParticleType : uint32_t
{
    None,
    Dust,
    SelectSceneBackGround,
    GameSceneBackGround,
    Goal,
    Key,
    Clear,
    PlayerIdle,
	PlayerJump,
    GetKey,

    Count
};

struct ParticleState
{
    Matrix4 worldMatrix;
    Vector4<float> color;
    ParticleType type;
    uint32_t textureIndex;
    float temperature;
    float lifetime;
    float density;
    Vector3 velocity;
    float age = 0.0f;
    bool hasLifetime;
    float rotationSpeed;

    bool hasExisted_;
    bool isEmit_;
    int frameCount_;
    int appearInterval_;
    Vector3 emitterRange_;
    Vector3 prePos_ = {};
    Vector3 acceleration_;
    Vector3 startPos_;
    float speed_;
    int spawnFrame_ = 0;
    float thetaVel_ = 0;
    int amount = 0;
    Vector3 startScale = { 0.0f,0.0f,0.0f };
    Vector3 endScale = { 1.0f,1.0f,1.0f };
    bool isExist_;
    unsigned int startColor_;     
    unsigned int endColor_;
    std::unique_ptr<SoheEasing> fadeOutEase_;
    std::unique_ptr<SoheEasing> toCenterEase_;
    std::unique_ptr<SoheEasing> scaleEase_;
    float theta_;

    std::unique_ptr<JointedCube> cube;

    ParticleState()
        : cube(std::make_unique<JointedCube>())
    {
        int tex[6] = { 0,0,0,0,0,0 };
        cube->Initialize(1.0f, 1.0f, 1.0f, { 0,0,0 }, tex);

        cube->texHandle = (int)TagTexture::kTestGoal;
        cube->color = { 255,0,0,255 };
        cube->SynchronizeTexAndColor();

        // デフォルト値で初期化
        isExist_ = false;
        hasExisted_ = false;
        frameCount_ = 0;
        appearInterval_ = 4;
        startColor_ = 0xffffffff;
        endColor_ = 0xffffff00;
        speed_ = 1.0f;
        emitterRange_ = { 20, 20, 20 };
        fadeOutEase_ = std::make_unique<SoheEasing>();
        toCenterEase_ = std::make_unique<SoheEasing>();
        scaleEase_ = std::make_unique<SoheEasing>();
        isEmit_ = false;
        
    }
};

struct ParticleConfig
{
    ParticleType type;
    float gravity;
    float drag;
    float decayRate;
    float maxLifetime;
    uint32_t textureIndex;
    bool reactsToFire;
    bool emitsLight;
    float particleRadius;
    Vector4<float> baseColor;
};


class ParticleSystem
{
public:
    void Initialize();
    void SpawnParticle(Transform4& transform, ParticleType type, float lifetime, int amount, const Matrix4& vpMat_);
    void Update(const Matrix4& vpMat_);
    void AddEmitter(ParticleEmitter* emitter);
    void LoadParticleDefinitionsFromJson(const std::string& filepath);
    void Draw();

private:

public:
    std::vector<ParticleEmitter*> emitters_;  // エミッターのリスト
    std::vector<ParticleState> particles_;
    std::array<ParticleConfig, static_cast<size_t>(ParticleType::Count)> particleConfigs_;

    bool enableKeyParticles_ = true;
    bool enableClearParticles_ = true;
    bool enableGoalParticles_ = true;
    bool enableGetKeyParticles_ = true;

    int getKeyParticleTimer_ = 0; 
    const int kGetKeyParticleDuration = 120;
};


