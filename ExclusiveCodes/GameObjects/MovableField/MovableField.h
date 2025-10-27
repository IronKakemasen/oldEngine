#pragma once
#include "../MovableBlock/MovableBlock.h"


class MovableField
{
public:
    MovableField();
    void Initialize();
    void Update(Matrix4 vpMat_);
    void Draw();

    void Activate(MovableBlock::ArrowDirection dir, const Vector3& pos);
    void ActivatePredictive(MovableBlock::ArrowDirection dir, const Vector3& pos, Matrix4 vpMat_);

    void SetDirection(MovableBlock::ArrowDirection dir);
    void SetPosition(const Vector3& pos);

    bool enabled = false; // 領域が有効なときだけ判定
    MovableBlock::ArrowDirection direction;
    JointedCube jointCube;
    float fieldLength; // 領域の長さ
    bool isStretch = false;

    bool isFading = false;
    float fadeWaitTimer = 0.0f;
    float fadeOutTimer = 0.0f;
    float kFadeWaitDuration = 0.1f;      // 待機時間
    float kFadeOutDuration = 1.0f;       // フェード時間

    float easingTime = 0.0f;           // 経過時間
    float easingDuration = 0.7f;       // 伸ばす時間
    float easingStartLength = 0.0f;    // 開始時の長さ
    float easingTargetLength = 10.0f;   // 最大長さ

    std::function<void()> onEasingFinished;

    bool isEasing = false;

	float palseTimer = 0.0f; // α変化用タイマー

private:
    Vector3 position;
    float stretchAmount = 0.2f;  // 伸ばす量

};
