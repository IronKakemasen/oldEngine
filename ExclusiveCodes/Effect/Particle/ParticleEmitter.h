#pragma once
#include "../../../MyLib/Utilities/BaseClassStructures/Transform/Transform.h"
#include "Particle.h"

class ParticleEmitter
{
public:

    void Initialize(ParticleType type, const Vector3& position, float spawnInterval, float lifetime, int amount);

    void Update(float deltaTime, ParticleSystem& particleSystem, const Matrix4& vpMat_);

    // Emitterの位置設定
    void SetPosition(const Vector3& position) { position_ = position; }

    // Emitterのタイプ設定
    void SetType(ParticleType type) { type_ = type; }

    ParticleType type_;
    Vector3 position_;
    float spawnInterval_;
    float lifetime_;
    float timeSinceLastSpawn_;
    int amount_;
private:

};

