#include "ParticleEmitter.h"

void ParticleEmitter::Initialize(ParticleType type, const Vector3& position, float spawnInterval, float lifetime, int amount)
{
    type_ = type; 
    position_ = position; 
    spawnInterval_ = spawnInterval; 
    lifetime_ = lifetime; 
    amount_ = amount;
    timeSinceLastSpawn_ = 0.0f;
}

void ParticleEmitter::Update(float deltaTime, ParticleSystem& particleSystem, const Matrix4& vpMat_)
{
    timeSinceLastSpawn_ += deltaTime;

    // 定期的にパーティクルを生成
    while (timeSinceLastSpawn_ >= spawnInterval_)
    {
        Transform4 wt;
        wt.pos = position_;

        particleSystem.SpawnParticle(wt, type_, lifetime_, amount_, vpMat_);
        timeSinceLastSpawn_ -= spawnInterval_;
    }
}