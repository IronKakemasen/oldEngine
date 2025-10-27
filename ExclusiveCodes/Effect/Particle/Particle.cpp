#define _USE_MATH_DEFINES

#include "Particle.h"
#include "ParticleEmitter.h"
#include "../../../External/nlohmann/json.hpp"
#include "../../../MyLib/NoviceLike/NoviceLike.h"
#include "../../Utilities/SoheStructures.h"

#include <iostream>

// ParticleType文字列をenumに変換するヘルパー関数
ParticleType StringToParticleType(const std::string& typeStr)
{
    if (typeStr == "None") return ParticleType::None;
    if (typeStr == "Dust") return ParticleType::Dust;
    if (typeStr == "SelectSceneBackGround") return ParticleType::SelectSceneBackGround;
    if (typeStr == "GameSceneBackGround") return ParticleType::GameSceneBackGround;
    // 他のタイプも追加
    return ParticleType::None;
}

void ParticleSystem::Initialize()
{
    // JSONから読み込む
    LoadParticleDefinitionsFromJson("Game/Data/particles.json");

    // JSONに色情報がない場合、ここでデフォルト色を設定する
    particleConfigs_[static_cast<size_t>(ParticleType::None)].baseColor = Uint32ToColorVector(0xFFFFFFff);

    // パーティクルインスタンスの初期化
    for (auto& p : particles_)
    {

    }
}

Matrix4 UpdateTranslation(const Matrix4& mat, const Vector3& delta)
{
    Matrix4 result = mat;
    result.m[3][0] += delta.x;
    result.m[3][1] += delta.y;
    result.m[3][2] += delta.z;
    return result;
}

void ParticleSystem::SpawnParticle(Transform4& transform, ParticleType type, float lifetime, int amount, const Matrix4& vpMat_)
{
    const ParticleConfig* config = &particleConfigs_[static_cast<size_t>(type)];

    ParticleState particle;
    //particle.worldMatrix = Matrix4::MakeAffine(transform.scale_, transform.rotation_, transform.translation_);
   /* particle.worldMatrix = transform.mat.World;*/
    particle.cube->trans.pos = transform.pos;
    particle.color = config->baseColor;
    particle.type = type;
	particle.amount = amount;
    particle.textureIndex = config->textureIndex;
    particle.lifetime = lifetime;
    particle.age = 0.0f;
    particle.hasLifetime = true;
    // タイプごとの初期値
    particle.velocity = { 0.0f, -2.0f, 0.0f }; // すべてのパーティクルが静止

    if (type == ParticleType::SelectSceneBackGround)
    {
        // ランダム位置
        particle.cube->trans.pos.z = 3.0f;

        // 回転速度
        particle.rotationSpeed = (rand() % 2 == 0 ? 1.0f : -1.0f) * GetRand(3.0f, 8.0f);

        particle.hasLifetime = true;

        particle.scaleEase_->interval = 0.006f;
        particle.fadeOutEase_->interval = 0.003f;

        particle.cube->trans.scale = { 1.0f,1.0f,1.0f };
        particle.acceleration_ = { 0,0.3f, 0.0f };
        particle.cube->color = { 255,255,255,10 };
        particle.startColor_ = 0xffffff44;
        particle.endColor_ = 0xffffff00;
        particle.isExist_ = false;
        particle.hasExisted_ = false;
        particle.speed_ = 0.2f;
        particle.frameCount_ = 0;
		particle.cube->texHandle = (int)TagTexture::kMovableBlock;

        particle.cube->SynchronizeTexAndColor();

        particle.scaleEase_->SetEasing(EasingType::EaseInSine);
        particle.fadeOutEase_->SetEasing(EasingType::EaseInSine);
    }
    if (type == ParticleType::GameSceneBackGround)
    {
        particle.cube->trans.pos.z = 3.0f;
        particle.appearInterval_ = 30;
        particle.fadeOutEase_->interval = 0.01f;
        particle.cube->trans.scale = { 0.0f,0.0f,0.0f };
        particle.cube->color = { 0.0f, 118.0f, 73.0f, 136.0f };
        particle.startColor_ = 0x00764988;
        particle.endColor_ = 0x00764900;
        particle.cube->texHandle = (int)TagTexture::kParticle0;
        particle.cube->SynchronizeTexAndColor();
        particle.isExist_ = false;
        particle.hasExisted_ = false;
        particle.speed_ = 0.2f;
        particle.frameCount_ = 0;
        particle.fadeOutEase_->SetEasing(EasingType::EaseInSine);
    }
    if (type == ParticleType::Goal)
    {
        particle.appearInterval_ = 4;
        particle.emitterRange_ = { 0.45f, 0, 0 };
        particle.fadeOutEase_->interval = 0.015f;
        particle.cube->color = (256.0f, 222.0f, 0.0f, 256.0f);
        particle.cube->trans.scale.x = 0.3f;
        particle.cube->trans.scale.y = 0.3f;
        particle.cube->trans.scale.z = 0.3f;
        particle.startColor_ = 0xFFDE00ff;
        particle.endColor_ = 0xFFDE0000;
        particle.cube->texHandle = (int)TagTexture::kWhite2x2;
        particle.cube->SynchronizeTexAndColor();
        particle.isExist_ = true;
        particle.hasExisted_ = false;
        particle.frameCount_ = 0;
        particle.isEmit_ = false;
        particle.speed_ = 1.0f;
        particle.fadeOutEase_->SetEasing(EasingType::EaseOutCirc);
    }
    if (type == ParticleType::Key)
    {
        particle.appearInterval_ = 4;
        particle.emitterRange_ = { 0.5f, 1,0 };
        particle.fadeOutEase_->interval = 0.01f;
        particle.cube->color = { 127.0f,255.0f,0.0f,255.0f };
        particle.startColor_ = 0xffa500ff;
        particle.endColor_ = 0xffa50000;
        particle.cube->texHandle = (int)TagTexture::kWhite2x2;
        particle.cube->SynchronizeTexAndColor();
		particle.cube->trans.scale.z = -0.3f;
        particle.isExist_ = true;
        particle.hasExisted_ = false;
        particle.frameCount_ = 0;
        particle.isEmit_ = false;
        particle.speed_ = 0.01f;
        particle.fadeOutEase_->SetEasing(EasingType::EaseOutCirc);
        particle.scaleEase_->SetEasing(EasingType::EaseLinear);
        particle.fadeOutEase_->interval = 0.04f;
        particle.scaleEase_->interval = 0.04f;
    }
    if (type == ParticleType::Clear)
    {
        particle.appearInterval_ = 1;
        particle.emitterRange_ = { 6, 6, 0 };
        particle.fadeOutEase_->interval = 0.03f;
        particle.toCenterEase_->interval = 0.03f;
        particle.spawnFrame_ = 5;
        particle.scaleEase_->interval = 0.03f;
        particle.cube->color = { 157.0f,0.0f,255.0f,136.0f };
		particle.cube->trans.scale = { 0.0f,0.0f,0.0f };
        particle.startColor_ = 0xFF2A0088;
        particle.endColor_ = 0xFF2A0000;
        particle.cube->texHandle = (int)TagTexture::kParticle0;
        particle.cube->SynchronizeTexAndColor();
        particle.isExist_ = false;
        particle.hasExisted_ = false;
        particle.speed_ = 0.2f;
        particle.frameCount_ = 0;
        particle.startPos_ = transform.pos;
        particle.fadeOutEase_->SetEasing(EasingType::EaseInSine);
        particle.toCenterEase_->SetEasing(EasingType::EaseInSine);
        particle.scaleEase_->SetEasing(EasingType::EaseInSine);
    }
    if (type == ParticleType::PlayerIdle)
    {
        particle.appearInterval_ = 4;
        particle.emitterRange_ = { 0.45f, 0, 0 };
        particle.fadeOutEase_->interval = 0.015f;
        particle.cube->color = (255.0f, 0.0f, 0.0f, 255.0f);
        particle.cube->trans.scale.x = 1.0f;
        particle.cube->trans.scale.y = 1.0f;
        particle.cube->trans.scale.z = 0.01f;
        particle.startColor_ = 0xff0000ff;
        particle.endColor_ = 0xff000000;
        particle.cube->texHandle = (int)TagTexture::kParticle0;
        particle.cube->SynchronizeTexAndColor();
        particle.isExist_ = true;
        particle.hasExisted_ = false;
        particle.frameCount_ = 0;
        particle.startPos_ = transform.pos;
        particle.isEmit_ = false;
        particle.speed_ = 1.0f;
        particle.fadeOutEase_->SetEasing(EasingType::EaseOutCirc);
    }
    if (type == ParticleType::GetKey)
    {
        particle.appearInterval_ = 0;
        particle.emitterRange_ = { 30, 30,0.0f };
        particle.fadeOutEase_->interval = 0.01f;
        particle.cube->color = { 235,228,0,100 };
        particle.startColor_ = 0xEBE40064;
        particle.endColor_ = 0xEBE40000;
        particle.cube->trans.scale.x = 0.4f;
        particle.cube->trans.scale.y = 0.4f;
        particle.cube->trans.scale.z = 0.01f;
        particle.cube->texHandle = (int)TagTexture::kWhite2x2;
        particle.cube->SynchronizeTexAndColor();
        particle.isExist_ = false;
        particle.hasExisted_ = false;
        particle.frameCount_ = 0;
        particle.isEmit_ = false;
        particle.speed_ = 0.03f;
        particle.startPos_ = transform.pos;
        particle.fadeOutEase_->SetEasing(EasingType::EaseOutCirc);
        particle.scaleEase_->SetEasing(EasingType::EaseLinear);
        particle.fadeOutEase_->interval = 0.02f;
        particle.scaleEase_->interval = 0.02f;
    }

    particle.cube->Update(vpMat_);

    particles_.push_back(std::move(particle));
}


void ParticleSystem::LoadParticleDefinitionsFromJson(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        // エラー処理: ファイルが開けなかったことをログに出力するなど
        return;
    }

    nlohmann::json j;
    try {
        file >> j;
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return;
    }

    for (const auto& particle_json : j)
    {
        ParticleConfig config = {};
        config.type = StringToParticleType(particle_json["type"].get<std::string>());
        config.gravity = particle_json["gravity"].get<float>();
        config.drag = particle_json["drag"].get<float>();
        config.decayRate = particle_json["decayRate"].get<float>();
        config.maxLifetime = particle_json["maxLifetime"].get<float>();
        config.textureIndex = particle_json["textureIndex"].get<uint32_t>();

        particleConfigs_[static_cast<size_t>(config.type)] = config;
    }
}

void ParticleSystem::Update(const Matrix4& vpMat_)
{
    float deltaTime = 1.0f / 60.0f;

    for (auto& emitter : emitters_)
    {
        emitter->Update(deltaTime, *this, vpMat_);
    }

    // パーティクル更新処理
    for (auto it = particles_.begin(); it != particles_.end(); )
    {
        ParticleType type = it->type;

        if (type == ParticleType::Dust)
        {
            const ParticleConfig& config = particleConfigs_[static_cast<size_t>(type)];

            // 重力の影響
            it->velocity.y -= config.gravity * deltaTime;

            // ドラッグ
            it->velocity = it->velocity * (1.0f - config.drag);

            // 位置の更新
            it->cube->trans.pos = it->cube->trans.pos + (it->velocity * deltaTime);
        }
        else if (type == ParticleType::SelectSceneBackGround)
        {
            if (it->frameCount_ >= it->amount * it->appearInterval_)
            {
                it->hasExisted_ = false;
                it->frameCount_ = 0;
            }
            if (!it->hasExisted_)
            {
                if (it->frameCount_ >= it->spawnFrame_)
                {
                    if (!it->isExist_)
                    {
                        // ランダムで角度を設定
                        it->theta_ = static_cast<float>(rand()) / RAND_MAX * 2.0f * float(M_PI);
                        it->cube->trans.pos.x = static_cast<float>(it->cube->trans.pos.x) + float(GetRand(-12, 26));
                        it->cube->trans.pos.y = static_cast<float>(it->cube->trans.pos.y) - 8.0f;
                        it->cube->trans.scale = { 0.0f,0.0f,0.0f };
                        it->endScale.x = float(GetRand(10, 50));
                        it->endScale.y = it->endScale.x;
                        it->endScale.z = float(GetRand(5, 10));
                        // 初期速度を設定
                        it->velocity.x = float(GetRand(0.0f, 0.0f));
                        it->velocity.y = float(GetRand(0.05f, 0.08f));
                        if (rand() % 2 == 0)
                        {
                            it->cube->texHandle = (int)TagTexture::kMovableBlock;
                            it->cube->SynchronizeTexAndColor();
                        }
                        else
                        {
                            it->cube->texHandle = (int)TagTexture::kNormalBlock;
                            it->cube->SynchronizeTexAndColor();
                        }

                        if (rand() % 2 == 0)
                        {
                            it->thetaVel_ = float(GetRand(3, 8));
                        }
                        else
                        {
                            it->thetaVel_ = -float(GetRand(3, 8));
                        }

                        // パーティクルの存在を設定
                        it->isExist_ = true;
                        it->hasExisted_ = true;
                        it->fadeOutEase_->isEase = true;
                        it->scaleEase_->isEase = true;
                    }
                }
            }
            it->frameCount_++;
            // パーティクルの移動
            if (it->isExist_) {

                it->cube->trans.pos.y += (it->velocity.y);

                it->cube->trans.defaultQuatenion.z_axis.deltaTheta += it->thetaVel_ / 5.0f;

                // イージングが終了したら存在しなくなる

                // スケールの補間
               /* fadeOutEase_->CountEaseLinear(startColor_, endColor_, color_);*/
                if (it->scaleEase_->isEase)
                {
                    it->scaleEase_->CountEaseLinear({ 0.0f,0.0f,0.0f },
                        { it->endScale.x / 10,it->endScale.y / 10,it->endScale.z / 10 }, it->cube->trans.scale);
                }
            }
        }
        else if (type == ParticleType::GameSceneBackGround)
        {
            if (it->frameCount_ >= it->amount * it->appearInterval_)
            {
                it->hasExisted_ = false;
                it->frameCount_ = 0;
            }
            if (!it->hasExisted_)
            {
                if (it->frameCount_ >= it->spawnFrame_)
                {
                    if (!it->isExist_)
                    {
                        // ランダムで角度を設定
                        it->theta_ = static_cast<float>(rand()) / RAND_MAX * 2.0f * float(M_PI);
                        it->cube->trans.pos.x = float(GetRand(-19, 35));
                        it->cube->trans.pos.y = float(GetRand(18, 45));

                        // 初期速度を設定
                        it->velocity.x = float(GetRand(-0.005f, 0.005f));
                        it->velocity.y = float(GetRand(-0.005f, 0.005f));

                        if (GetRand(0, 1) == 0)
                        {
                            it->cube->color = { 0.0f, 89.0f, 155.0f, 60.0f };
                            it->startColor_ = 0x00599B88;
                            it->endColor_ = 0x00599B00;
                        }
                        else if (GetRand(0, 1) == 1)
                        {
                            it->cube->color = { 0.0f, 118.0f, 73.0f, 60.0f };
                            it->startColor_ = 0x00764988;
                            it->endColor_ = 0x00764900;
                        }
                      
                        it->endScale.x = float(GetRand(1.0f, 5.0f));
                        it->endScale.y = it->endScale.x;
                        it->endScale.z = 0.001f;

                        it->cube->SynchronizeColor();

                        if (rand() % 2 == 0)
                        {
                            it->thetaVel_ = float(GetRand(3, 8));
                        }
                        else
                        {
                            it->thetaVel_ = -float(GetRand(3, 8));
                        }

                        // パーティクルの存在を設定
                        it->isExist_ = true;
                        it->hasExisted_ = true;
                        it->fadeOutEase_->isEase = true;
                    }
                }
            }
            it->frameCount_++;
            // パーティクルの移動
            if (it->isExist_) {

                it->cube->trans.pos.x += it->velocity.x;
                it->cube->trans.pos.y += it->velocity.y;

                it->cube->trans.defaultQuatenion.z_axis.deltaTheta += it->thetaVel_ / 4.0f;

                // イージングが終了したら存在しなくなる
                if (!it->fadeOutEase_->isEase) {
                    it->isExist_ = false;
                }
                // スケールの補間
                it->fadeOutEase_->OnceReverseEaseLinear({ 0.0f,0.0f,0.0f }, it->endScale, it->cube->trans.scale);
            }
        }
        else if (type == ParticleType::Goal)
        {
            // フレームごとに新しいパーティクルを生成
            if (!it->hasExisted_)
            {
                if (it->frameCount_ >= it->spawnFrame_)
                {
                    if (!it->isExist_)
                    {
                        it->theta_ = static_cast<float>(rand()) / RAND_MAX * 2.0f * float(M_PI);
                        it->cube->trans.pos.x = static_cast<float>(it->cube->trans.pos.x) + GetRand(-(float)it->emitterRange_.x, (float)it->emitterRange_.x) ;
                        it->cube->trans.pos.y = static_cast<float>(it->cube->trans.pos.y);
                        it->velocity.x = 0;
                        it->velocity.y = (float)GetRand(0.02f, 0.04f);
                        it->isExist_ = true;
                        it->fadeOutEase_->isEase = true;
                        if (rand() % 3 == 0)
                        {
                            it->cube->color = { 225.0f,185.0f,0.0f,255.0f };
                            it->startColor_ = 0xFFB900ff;
                            it->endColor_ = 0xFFB90000;
                        }
                        else if (rand() % 3 == 1)
                        {
                            it->cube->color = { 12.0f,174.0f,112.0f,255.0f };
                            it->startColor_ = 0xCAE700ff;
                            it->endColor_ = 0xCAE70000;
                        }
                        else if (rand() % 3 == 2)
                        {
                            it->cube->color = { 255.0f,222.0f,0,255.0f };
                            it->startColor_ = 0xFFDE00ff;
                            it->endColor_ = 0xFFDE0000;
                        }

                        it->cube->SynchronizeColor();

                        if (GetRand(0, 1) == 0)
                        {
                            it->thetaVel_ = float(GetRand(10, 15));
                        }
                        else
                        {
                            it->thetaVel_ = -float(GetRand(10, 15));
                        }

                    }
                    it->frameCount_ = 0;
                }
            }
            it->frameCount_++;
            if (it->isExist_)
            {
                it->cube->trans.pos.y += it->velocity.y;

                it->cube->trans.defaultQuatenion.z_axis.deltaTheta += it->thetaVel_ / 20.0f;

                unsigned int currentColor = (unsigned int)ColorVectorToUint32(it->cube->color);
                it->fadeOutEase_->CountEaseLinear(it->startColor_, it->endColor_, currentColor);
                it->cube->color = Uint32ToColorVector(currentColor);
                it->cube->color = { it->cube->color.x * 255.0f,it->cube->color.y * 255.0f ,it->cube->color.z * 255.0f, it->cube->color.w * 255.0f };
                it->cube->SynchronizeColor();

                if (!it->fadeOutEase_->isEase)
                {
                    it->isExist_ = false;
                }
              
            }
        }
        else if (type == ParticleType::Key)
        {


            if (it->frameCount_ >= it->amount * it->appearInterval_)
            {
                it->hasExisted_ = false;
                it->frameCount_ = 0;
            }
            // フレームごとに新しいパーティクルを生成
            if (!it->hasExisted_)
            {
                if (it->frameCount_ >= it->spawnFrame_)
                {
                    if (!it->isExist_)
                    {
                        // ランダムで角度を設定
                        it->theta_ = static_cast<float>(rand()) / RAND_MAX * 2.0f * float(M_PI);

                        // 半径をランダムに生成 (0～emitterRange_ の範囲)
                        float radius = static_cast<float>(GetRand(0.05f, static_cast<float>(it->emitterRange_.x)));

                        // 極座標 -> 直交座標
                        it->cube->trans.pos.x = static_cast<float>(it->cube->trans.pos.x) + radius * cos(it->theta_);
                        it->cube->trans.pos.y = static_cast<float>(it->cube->trans.pos.y +0.5f) + radius * sin(it->theta_);
                        it->velocity.x = it->speed_ * cosf(it->theta_);
                        it->velocity.y = it->speed_ * sinf(it->theta_);
                        it->isExist_ = true;
                        it->fadeOutEase_->isEase = true;
                        it->hasExisted_ = true;
                        if (rand() % 1 == 0)
                        {
                            it->cube->color = { 225.0f,185.0f,0.0f,255.0f };
                            it->startColor_ = 0xFFB900ff;
                            it->endColor_ = 0xFFB90000;
                        }
                        else if (rand() % 1 == 1)
                        {
                            it->cube->color = { 12.0f,174.0f,112.0f,255.0f };
                            it->startColor_ = 0xCAE700ff;
                            it->endColor_ = 0xCAE70000;
                        }
                       
                        it->cube->SynchronizeColor();
                        if (rand() % 2 == 0)
                        {
                            it->thetaVel_ = float(rand() % 6 + 3);
                        }
                        else
                        {
                            it->thetaVel_ = -float(rand() % 6 + 3);
                        }

                    }
                    it->frameCount_ = 0;
                }
            }
            it->frameCount_++;
            if (it->isExist_)
            {
                it->cube->trans.pos.x += it->velocity.x;
                it->cube->trans.pos.y += it->velocity.y;

                it->cube->trans.defaultQuatenion.z_axis.deltaTheta += it->thetaVel_;

                if (!it->fadeOutEase_->isEase)
                {
                    it->isExist_ = false;
                }
                unsigned int currentColor = (unsigned int)ColorVectorToUint32(it->cube->color);
                it->fadeOutEase_->CountEaseLinear(it->startColor_, it->endColor_, currentColor);
                it->cube->color = Uint32ToColorVector(currentColor);
                it->cube->color = { it->cube->color.x * 255.0f,it->cube->color.y * 255.0f ,it->cube->color.z * 255.0f, it->cube->color.w * 255.0f };
                it->scaleEase_->OnceReverseEaseLinear({ 0.0f,0.0f, 0.0f }, { 0.7f,0.7f,0.1f }, it->cube->trans.scale);
                it->cube->SynchronizeColor();
            }
        }
        else if (type == ParticleType::Clear)
        {


            if (it->frameCount_ >= it->amount * it->appearInterval_)
            {
                it->hasExisted_ = false;
                it->frameCount_ = 0;
            }
            if (!it->hasExisted_)
            {
                if (it->frameCount_ >= it->spawnFrame_)
                {
                    if (!it->isExist_)
                    {
                     
                        // ちゃんと中心からオフセットして上書きする
                        Vector3 center = it->startPos_;

                        it->theta_ = static_cast<float>(rand()) / RAND_MAX * 2.0f * float(M_PI);
                        float radius = static_cast<float>(GetRand(1.0f, static_cast<float>(it->emitterRange_.x)));

                        float offsetX = radius * cos(it->theta_);
                        float offsetY = radius * sin(it->theta_);

                        it->cube->trans.pos = {
                            center.x + offsetX,
                            center.y + offsetY,
                            center.z
                        };

                        // prePos_ も放射方向に設定（補間用）
                        it->prePos_ = it->cube->trans.pos;

                        // 初期速度を設定
                      /*  it->velocity.x = float(GetRand(5, 10));
                        it->velocity.y = float(GetRand(-4, -2));*/

                        if (GetRand(0, 0) == 0)
                        {
                            it->cube->color = { 210.0f,0.0f,255.0f,136.0f };
                            it->startColor_ = 0xD800FF88;
                            it->endColor_ = 0xD800FF00;
                        }
                        it->cube->SynchronizeColor();
                        if (rand() % 2 == 0)
                        {
                            it->thetaVel_ = float(GetRand(3, 6));
                        }
                        else
                        {
                            it->thetaVel_ = -float(GetRand(3, 6));
                        }

                        // パーティクルの存在を設定
                        it->isExist_ = true;
                        it->hasExisted_ = true;
                        it->fadeOutEase_->isEase = true;
                        it->toCenterEase_->isEase = true;
                    }
                }
            }

            it->frameCount_++;

            // パーティクルの移動
            if (it->isExist_) {
                it->cube->trans.defaultQuatenion.z_axis.deltaTheta  += it->thetaVel_ / 10.0f;

                // イージングが終了したら存在しなくなる
                if (!it->toCenterEase_->isEase) {
                    it->isExist_ = false;
                }
                // スケールの補間
                it->toCenterEase_->CountEaseLinear(it->prePos_, it->startPos_, it->cube->trans.pos);
                it->scaleEase_->CountEaseLinear({ 0.0f,0.0f,0.0f }, { 1.6f,1.6f,1.6f }, it->cube->trans.scale);
                unsigned int currentColor = (unsigned int)ColorVectorToUint32(it->cube->color);
                it->fadeOutEase_->CountEaseLinear(it->startColor_, it->endColor_, currentColor);
                it->cube->color = Uint32ToColorVector(currentColor);
                it->cube->color = { it->cube->color.x * 255.0f,it->cube->color.y * 255.0f ,it->cube->color.z * 255.0f, it->cube->color.w * 255.0f };
               /* it->cube->SynchronizeColor();*/
            }
        }
        else if (type == ParticleType::PlayerIdle)
        {
            // フレームごとに新しいパーティクルを生成
            if (!it->hasExisted_)
            {
                if (it->frameCount_ >= it->spawnFrame_)
                {
                    if (!it->isExist_)
                    {
                        it->theta_ = static_cast<float>(rand()) / RAND_MAX * 2.0f * float(M_PI);
                        it->cube->trans.pos.x = static_cast<float>(it->cube->trans.pos.x) + GetRand(-(float)it->emitterRange_.x, (float)it->emitterRange_.x);
                        it->cube->trans.pos.y = static_cast<float>(it->cube->trans.pos.y) + GetRand(-(float)it->emitterRange_.x, (float)it->emitterRange_.x);
                        it->velocity.x = 0;
                        it->velocity.y = (float)GetRand(0.0001f, 0.00042f);
                        it->isExist_ = true;
                        it->fadeOutEase_->isEase = true;
                      
                        it->cube->SynchronizeColor();

                        if (GetRand(0, 1) == 0)
                        {
                            it->thetaVel_ = float(GetRand(10, 15));
                        }
                        else
                        {
                            it->thetaVel_ = -float(GetRand(10, 15));
                        }

                    }
                    it->frameCount_ = 0;
                }
            }
            it->frameCount_++;
            if (it->isExist_)
            {
                it->cube->trans.pos.y += it->velocity.y;

                it->cube->trans.defaultQuatenion.z_axis.deltaTheta += it->thetaVel_ / 20.0f;

                unsigned int currentColor = (unsigned int)ColorVectorToUint32(it->cube->color);
                it->fadeOutEase_->CountEaseLinear(it->startColor_, it->endColor_, currentColor);
                it->cube->color = Uint32ToColorVector(currentColor);
                it->cube->color = { it->cube->color.x * 255.0f,it->cube->color.y * 255.0f ,it->cube->color.z * 255.0f, it->cube->color.w * 255.0f };
                it->cube->SynchronizeColor();

                if (!it->fadeOutEase_->isEase)
                {
                    it->isExist_ = false;
                }

            }
        }
        else if (type == ParticleType::GetKey)
        {
            if (!it->hasExisted_) // ← まだ発生してない
            {
                if (it->frameCount_ >= it->spawnFrame_)
                {
                    // === パーティクルを1回だけ生成 ===
                    // ランダムな角度と速度を設定
                    it->theta_ = static_cast<float>(rand()) / RAND_MAX * 2.0f * float(M_PI);
                    it->cube->trans.pos.x = static_cast<float>(it->startPos_.x);
                    it->cube->trans.pos.y = static_cast<float>(it->startPos_.y);

                    it->velocity.x = it->speed_ * cosf(it->theta_);
                    it->velocity.y = it->speed_ * sinf(it->theta_);

                    if (GetRand(0, 1) == 0)
                    {
                        it->cube->color = { 235, 228, 0, 60 };
                        it->startColor_ = 0xEBE43c;
                        it->endColor_ = 0xEBE40000;
                    }
                    else
                    {
                        it->cube->color = { 151, 154, 0, 136 };
                        it->startColor_ = 0x979A003c;
                        it->endColor_ = 0x979A0000;
                    }

                    it->thetaVel_ = (rand() % 2 == 0) ? float(GetRand(3, 6)) : -float(GetRand(3, 6));

                    // 発生フラグ
                    it->isExist_ = true;
                    it->hasExisted_ = true; 
                    it->fadeOutEase_->isEase = true;
                }
            }

            // パーティクルの移動処理
            if (it->isExist_) {

                it->cube->trans.pos.x += it->velocity.x;
                it->cube->trans.pos.y += it->velocity.y;

                it->cube->trans.defaultQuatenion.z_axis.deltaTheta += it->thetaVel_ / 100.0f;

                // スケールの補間
                it->fadeOutEase_->CountEaseLinear({ 0.4f, 0.4f, 0.01f }, { 0.0f, 0.0f, 0.0f }, it->cube->trans.scale);

                // イージングが終了したらパーティクル終了
                if (!it->fadeOutEase_->isEase) {
                    it->isExist_ = false;
                }
            }

            // フレームカウントはhasExisted_がfalseの間だけ増やす
            if (!it->hasExisted_) {
                it->frameCount_++;
            }
        }

        // 寿命管理
        if (it->hasLifetime)
        {
            it->lifetime -= deltaTime;
            if (it->lifetime <= 0.0f)
            {
                it = particles_.erase(it);
                continue;
            }
        }

        ++it;
    }

    // パーティクルインスタンスの更新
    for (auto& particle : particles_)
    {
        particle.cube->Update(vpMat_);
    }
}


void ParticleSystem::AddEmitter(ParticleEmitter* emitter) 
{
    emitters_.push_back(emitter);
}

void ParticleSystem::Draw()
{
    for (auto& particle : particles_)
    {
        ParticleType type = particle.type;

        if (type == ParticleType::SelectSceneBackGround)
        {
            NoviceLike::SetBlendMode(kBlendModeAdd);
            
            particle.cube->Draw();
            
            NoviceLike::SetBlendMode(kBlendModeNormal);
        }
        else if (type == ParticleType::GameSceneBackGround)
        {
            NoviceLike::SetBlendMode(kBlendModeAdd);
            for (int i = 0; i < 5; ++i)
            {
                particle.cube->Draw();
            }

            NoviceLike::SetBlendMode(kBlendModeNormal);
        }
        else if (type == ParticleType::Goal)
        {
            if (!enableGoalParticles_) continue;
            NoviceLike::SetBlendMode(kBlendModeAdd);
          
            particle.cube->Draw();
            

            NoviceLike::SetBlendMode(kBlendModeNormal);
        }
        else if (type == ParticleType::Key)
        {
            if (!enableKeyParticles_) continue;
            NoviceLike::SetBlendMode(kBlendModeAdd);

            for (int i = 0; i < 5; ++i)
            {
                particle.cube->Draw();
            }

            NoviceLike::SetBlendMode(kBlendModeNormal);
        }
        else if (type == ParticleType::Clear)
        {
            if (!enableClearParticles_) continue;
            NoviceLike::SetBlendMode(kBlendModeAdd);

            for (int i = 0; i < 3; ++i)
            {
                particle.cube->Draw();
            }

            NoviceLike::SetBlendMode(kBlendModeNormal);
        }
        else if (type == ParticleType::PlayerIdle)
        {
            NoviceLike::SetBlendMode(kBlendModeAdd);

            for (int i = 0; i < 3; ++i)
            {
                particle.cube->Draw();
            }

            NoviceLike::SetBlendMode(kBlendModeNormal);
        }
        else if (type == ParticleType::GetKey)
        {
            if (!enableGetKeyParticles_) continue;
            NoviceLike::SetBlendMode(kBlendModeAdd);

                particle.cube->Draw();
            NoviceLike::SetBlendMode(kBlendModeNormal);
        }
        else
        {
            NoviceLike::SetBlendMode(kBlendModeAdd);

            particle.cube->Draw();


            NoviceLike::SetBlendMode(kBlendModeNormal);
        }
    }
    
}