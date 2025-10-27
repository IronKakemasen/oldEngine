#define _USE_MATH_DEFINES
#include <cmath>

#include "SoheEasing.h"


// コンストラクタ
SoheEasing::SoheEasing()
    : easingFunc(nullptr), easingType_(EasingType::EaseInOutSine) {
}

// デストラクタ
SoheEasing::~SoheEasing() {};

// イージング関数を設定
void SoheEasing::SetEasing(EasingType easingType)
{
    this->easingType_ = easingType;
    this->easingFunc = GetEasingFunction(easingType);
}

// イージング関数を取得
float (*SoheEasing::GetEasingFunction(EasingType easingType))(const float&)
{
    switch (easingType)
    {
    case EasingType::EaseLinear: return &SoheEasing::EaseLinear;
    case EasingType::EaseInSine: return &SoheEasing::EaseInSine;
    case EasingType::EaseOutSine: return &SoheEasing::EaseOutSine;
    case EasingType::EaseInOutSine: return &SoheEasing::EaseInOutSine;
    case EasingType::EaseInQuad: return &SoheEasing::EaseInQuad;
    case EasingType::EaseOutQuad: return &SoheEasing::EaseOutQuad;
    case EasingType::EaseInOutQuad: return &SoheEasing::EaseInOutQuad;
    case EasingType::EaseInCubic: return &SoheEasing::EaseInCubic;
    case EasingType::EaseOutCubic: return &SoheEasing::EaseOutCubic;
    case EasingType::EaseInOutCubic: return &SoheEasing::EaseInOutCubic;
    case EasingType::EaseInQuart: return &SoheEasing::EaseInQuart;
    case EasingType::EaseOutQuart: return &SoheEasing::EaseOutQuart;
    case EasingType::EaseInOutQuart: return &SoheEasing::EaseInOutQuart;
    case EasingType::EaseInExpo: return &SoheEasing::EaseInExpo;
    case EasingType::EaseOutExpo: return &SoheEasing::EaseOutExpo;
    case EasingType::EaseInOutExpo: return &SoheEasing::EaseInOutExpo;
    case EasingType::EaseInCirc: return &SoheEasing::EaseInCirc;
    case EasingType::EaseOutCirc: return &SoheEasing::EaseOutCirc;
    case EasingType::EaseInOutCirc: return &SoheEasing::EaseInOutCirc;
    case EasingType::EaseInBack: return &SoheEasing::EaseInBack;
    case EasingType::EaseOutBack: return &SoheEasing::EaseOutBack;
    case EasingType::EaseInOutBack: return &SoheEasing::EaseInOutBack;
    case EasingType::EaseInElastic: return &SoheEasing::EaseInElastic;
    case EasingType::EaseOutElastic: return &SoheEasing::EaseOutElastic;
    case EasingType::EaseInOutElastic: return &SoheEasing::EaseInOutElastic;
    case EasingType::EaseInBounce: return &SoheEasing::EaseInBounce;
    case EasingType::EaseOutBounce: return &SoheEasing::EaseOutBounce;
    case EasingType::EaseInOutBounce: return &SoheEasing::EaseInOutBounce;
    default: return &SoheEasing::EaseInOutSine;
    }
}

// イージングの初期化
void SoheEasing::InitEasing()
{
    interval = 0.01f;
    cycle = 0.0f;
    timer = 0.0f;
    easeTimer = 0.0f;
    isEase = false;
}

// 汎用線形補間（テンプレート）
//template <typename T>
void SoheEasing::CountEaseLinear(int start, int end, int& current)
{

    if (timer >= 1.0f) {
        current = end;
        timer = 0.0f;
        isEase = false;
    }
    else {
        timer += interval;
        easeTimer = easingFunc(timer);
        current = static_cast<int>((1.0f - easeTimer) * static_cast<int>(start) + easeTimer * static_cast<int>(end));
    }
}

void SoheEasing::CountEaseLinear(float start, float end, float& current)
{

    if (timer >= 1.0f) {
        current = end;
        timer = 0.0f;
        isEase = false;
    }
    else {
        timer += interval;
        easeTimer = easingFunc(timer);
        current = static_cast<float>((1.0f - easeTimer) * static_cast<float>(start) + easeTimer * static_cast<float>(end));
    }
}

// unsigned int 用の線形補間
void SoheEasing::CountEaseLinear(unsigned int start, unsigned int end, unsigned int& current)
{


    unsigned int startR = (start >> 24) & 0xff;
    unsigned int startG = (start >> 16) & 0xff;
    unsigned int startB = (start >> 8) & 0xff;
    unsigned int startA = start & 0xff;

    unsigned int endR = (end >> 24) & 0xff;
    unsigned int endG = (end >> 16) & 0xff;
    unsigned int endB = (end >> 8) & 0xff;
    unsigned int endA = end & 0xff;

    unsigned int newR, newG, newB, newA;
    if (timer >= 1.0f) {
        newR = endR;
        newG = endG;
        newB = endB;
        newA = endA;
        isEase = false;
    }
    else {
        timer += interval;
        if (timer >= 1.0f) {
            timer = 1.0f;
        }
        easeTimer = easingFunc(timer);
        newR = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(startR) + easeTimer * static_cast<float>(endR));
        newG = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(startG) + easeTimer * static_cast<float>(endG));
        newB = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(startB) + easeTimer * static_cast<float>(endB));
        newA = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(startA) + easeTimer * static_cast<float>(endA));
    }

    current = (newR << 24) | (newG << 16) | (newB << 8) | newA;
}

// Vector3 用の線形補間
void SoheEasing::CountEaseLinear(Vector3 start, Vector3 end, Vector3& current)
{

    if (timer >= 1.0f) {
        current = end;
        easeTimer = 0.0f;
        timer = 0.0f;
        isEase = false;

    }
    else {
        timer += interval;
        easeTimer = easingFunc(timer);
        current.x = (1.0f - easeTimer) * start.x + easeTimer * end.x;
        current.y = (1.0f - easeTimer) * start.y + easeTimer * end.y;
        current.z = (1.0f - easeTimer) * start.z + easeTimer * end.z;
    }
}

void SoheEasing::ReverseEaseLinear(float start, float end, float& current)
{
    if (isReverse) {
        // start -> end への遷移
        if (timer <= 0.0f) {
            current = start;
            easeTimer = 0.0f;
            timer = 1.0f;
            isReverse = false;  // 逆方向への遷移を開始
        }
        else {
            timer -= interval;
            if (timer <= 0.0f) {
                timer = 0.0f;
            }
            easeTimer = easingFunc(timer);
            current = (1.0f - easeTimer) * start + easeTimer * end;
        }
    }
    else {
        // end -> start への逆方向の遷移
        if (timer <= 0.0f) {
            current = end;
            easeTimer = 0.0f;
            timer = 1.0f;
            isReverse = true;  // 元の方向に戻る
        }
        else {
            timer -= interval;
            if (timer <= 0.0f) {
                timer = 0.0f;
            }
            easeTimer = easingFunc(timer);
            current = (1.0f - easeTimer) * end + easeTimer * start;
        }
    }
}

void SoheEasing::ReverseEaseLinear(Vector3 start, Vector3 end, Vector3& current)
{

    if (isReverse) {
        // start -> end への遷移
        if (timer <= 0.0f) {
            current = start;
            easeTimer = 0.0f;
            timer = 1.0f;
            isReverse = false;  // 逆方向への遷移を開始
        }
        else {
            timer -= interval;
            if (timer <= 0.0f) {
                timer = 0.0f;
            }
            easeTimer = easingFunc(timer);
            current.x = (1.0f - easeTimer) * start.x + easeTimer * end.x;
            current.y = (1.0f - easeTimer) * start.y + easeTimer * end.y;
            current.z = (1.0f - easeTimer) * start.z + easeTimer * end.z;
        }
    }
    else {
        // end -> start への逆方向の遷移
        if (timer <= 0.0f) {
            current = end;
            easeTimer = 0.0f;
            timer = 1.0f;
            isReverse = true;  // 元の方向に戻る
        }
        else {
            timer -= interval;
            if (timer <= 0.0f) {
                timer = 0.0f;
            }
            easeTimer = easingFunc(timer);
            current.x = (1.0f - easeTimer) * end.x + easeTimer * start.x;
            current.y = (1.0f - easeTimer) * end.y + easeTimer * start.y;
            current.z = (1.0f - easeTimer) * end.z + easeTimer * start.z;
        }
    }
}

void SoheEasing::OnceReverseEaseLinear(Vector3 start, Vector3 end, Vector3& current)
{
    if (isReverse) {
        // start -> end
        if (timer <= 0.0f) {
            current = start;
            easeTimer = 0.0f;
            timer = 1.0f;
            isReverse = false;  // 再び逆方向へ
        }
        else {
            timer -= interval;
            if (timer <= 0.0f) {
                timer = 0.0f;
                current = start;
                isEase = false;
            }
            easeTimer = easingFunc(timer);
            current.x = (1.0f - easeTimer) * start.x + easeTimer * end.x;
            current.y = (1.0f - easeTimer) * start.y + easeTimer * end.y;
            current.z = (1.0f - easeTimer) * start.z + easeTimer * end.z;
        }
    }
    else {
        // end -> start（逆方向）
        if (timer <= 0.0f) {
            current = end;
            easeTimer = 0.0f;
            timer = 1.0f;
            isReverse = true;
        }
        else {
            timer -= interval;
            if (timer <= 0.0f) {
                timer = 0.0f;
                current = end;
            }
            easeTimer = easingFunc(timer);
            current.x = (1.0f - easeTimer) * end.x + easeTimer * start.x;
            current.y = (1.0f - easeTimer) * end.y + easeTimer * start.y;
            current.z = (1.0f - easeTimer) * end.z + easeTimer * start.z;
        }
    }
}

// 色遷移のイージング関数
void SoheEasing::ReverseColorLinear(unsigned int start, unsigned int end, unsigned int& current)
{
    unsigned int startR = (start >> 24) & 0xff;
    unsigned int startG = (start >> 16) & 0xff;
    unsigned int startB = (start >> 8) & 0xff;
    unsigned int startA = start & 0xff;

    unsigned int endR = (end >> 24) & 0xff;
    unsigned int endG = (end >> 16) & 0xff;
    unsigned int endB = (end >> 8) & 0xff;
    unsigned int endA = end & 0xff;

    unsigned int newR, newG, newB, newA;

    if (isReverse) {
        // start -> end への遷移
        if (timer <= 0.0f) {
            newR = startR;
            newG = startG;
            newB = startB;
            newA = startA;
            easeTimer = 0.0f;
            timer = 1.0f;
            isReverse = false;

        }
        else {
            timer -= interval;
            if (timer <= 0.0f) {
                timer = 0.0f;
            }
            easeTimer = easingFunc(timer);
            newR = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(startR) + easeTimer * static_cast<float>(endR));
            newG = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(startG) + easeTimer * static_cast<float>(endG));
            newB = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(startB) + easeTimer * static_cast<float>(endB));
            newA = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(startA) + easeTimer * static_cast<float>(endA));
        }
    }
    else {
        // end -> start への逆方向の遷移
        if (timer <= 0.0f) {
            newR = endR;
            newG = endG;
            newB = endB;
            newA = endA;
            easeTimer = 0.0f;
            timer = 1.0f;
            isReverse = true;
        }
        else {
            timer -= interval;
            if (timer <= 0.0f) {
                timer = 0.0f;
            }
            easeTimer = easingFunc(timer);
            newR = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(endR) + easeTimer * static_cast<float>(startR));
            newG = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(endG) + easeTimer * static_cast<float>(startG));
            newB = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(endB) + easeTimer * static_cast<float>(startB));
            newA = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(endA) + easeTimer * static_cast<float>(startA));
        }
    }

    current = (newR << 24) | (newG << 16) | (newB << 8) | newA;
}

void SoheEasing::OnceReverseColorLinear(unsigned int start, unsigned int end, unsigned int& current)
{
    unsigned int startR = (start >> 24) & 0xff;
    unsigned int startG = (start >> 16) & 0xff;
    unsigned int startB = (start >> 8) & 0xff;
    unsigned int startA = start & 0xff;

    unsigned int endR = (end >> 24) & 0xff;
    unsigned int endG = (end >> 16) & 0xff;
    unsigned int endB = (end >> 8) & 0xff;
    unsigned int endA = end & 0xff;

    unsigned int newR, newG, newB, newA;

    if (isReverse) {
        // start -> end への遷移
        if (timer <= 0.0f) {
            newR = startR;
            newG = startG;
            newB = startB;
            newA = startA;
            easeTimer = 0.0f;
            timer = 1.0f;
            isReverse = false;
            if (hasReverse)
            {
                isEase = false;
            }
            hasReverse = false;
        }
        else {
            timer -= interval;
            if (timer <= 0.0f) {
                timer = 0.0f;
            }
            easeTimer = easingFunc(timer);
            newR = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(startR) + easeTimer * static_cast<float>(endR));
            newG = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(startG) + easeTimer * static_cast<float>(endG));
            newB = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(startB) + easeTimer * static_cast<float>(endB));
            newA = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(startA) + easeTimer * static_cast<float>(endA));
        }
    }
    else {
        // end -> start への逆方向の遷移
        if (timer <= 0.0f) {
            newR = endR;
            newG = endG;
            newB = endB;
            newA = endA;
            easeTimer = 0.0f;
            timer = 1.0f;
            isReverse = true;
            hasReverse = true;
        }
        else {
            timer -= interval;
            if (timer <= 0.0f) {
                timer = 0.0f;
            }
            easeTimer = easingFunc(timer);
            newR = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(endR) + easeTimer * static_cast<float>(startR));
            newG = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(endG) + easeTimer * static_cast<float>(startG));
            newB = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(endB) + easeTimer * static_cast<float>(startB));
            newA = static_cast<unsigned int>((1.0f - easeTimer) * static_cast<float>(endA) + easeTimer * static_cast<float>(startA));
        }
    }

    current = (newR << 24) | (newG << 16) | (newB << 8) | newA;
}

// イージングの関数群
float SoheEasing::EaseLinear(const float& t) {
    return t;
}
float SoheEasing::EaseInSine(const float& t) {
    return 1 - cosf((t * (float)M_PI) / 2);
}
float SoheEasing::EaseOutSine(const float& t) {
    return sinf((t * (float)M_PI) / 2);
}
float SoheEasing::EaseInOutSine(const float& t) {
    return 0.5f * (1 - cosf((float)M_PI * t));
}
float SoheEasing::EaseInQuad(const float& t) {
    return t * t;
}
float SoheEasing::EaseOutQuad(const float& t) {
    return -t * (t - 2);
}
float SoheEasing::EaseInOutQuad(const float& t) {
    if (t < 0.5f) {
        return 2.0f * t * t;
    }
    else {
        return -2.0f * t * (t - 2.0f) - 1.0f;
    }
}
float SoheEasing::EaseInCubic(const float& t) {
    return t * t * t;
}
float SoheEasing::EaseOutCubic(const float& t) {
    float t_ = t - 1;
    return 1 + t_ * t_ * t_;
}
float SoheEasing::EaseInOutCubic(const float& t) {
    if (t < 0.5f) {
        return 4.0f * t * t * t;
    }
    else {
        float t_ = t - 1;
        return 1 + 4.0f * t_ * t_ * t_;
    }
}
float SoheEasing::EaseInQuart(const float& t) {
    return t * t * t * t;
}
float SoheEasing::EaseOutQuart(const float& t) {
    float t_ = t - 1;
    return 1 - t_ * t_ * t_ * t_;
}
float SoheEasing::EaseInOutQuart(const float& t) {
    if (t < 0.5f) {
        return 8.0f * t * t * t * t;
    }
    else {
        float t_ = t - 1;
        return 1 - 8.0f * t_ * t_ * t_ * t_;
    }
}
float SoheEasing::EaseInExpo(const float& t) {
    return t == 0.0f ? 0.0f : powf(2, 10 * (t - 1));
}
float SoheEasing::EaseOutExpo(const float& t) {
    return t == 1.0f ? 1.0f : 1 - powf(2, -10 * t);
}
float SoheEasing::EaseInOutExpo(const float& t) {
    if (t == 0.0f || t == 1.0f) {
        return t;
    }
    float tt = t * 2;
    if (tt < 1) {
        return 0.5f * powf(2, 10 * (tt - 1));
    }
    tt -= 1;
    return 0.5f * (2 - powf(2, -10 * tt));
}
float SoheEasing::EaseInCirc(const float& t) {
    return 1 - sqrtf(1 - t * t);
}
float SoheEasing::EaseOutCirc(const float& t) {
    return sqrtf(1 - (t - 1) * (t - 1));
}
float SoheEasing::EaseInOutCirc(const float& t) {
    if (t < 0.5f) {
        return 0.5f * (1 - sqrtf(1 - t * t * 4));
    }
    else {
        float tt = t - 1;
        return 0.5f * (sqrtf(1 - tt * tt * 4) + 1);
    }
}
float SoheEasing::EaseInBack(const float& t) {
    const float s = 1.70158f;
    return t * t * ((s + 1) * t - s);
}
float SoheEasing::EaseOutBack(const float& t) {
    const float s1 = 6.00158f;
    const float s2 = s1 + 1;
    return 1 + s2 * powf(t - 1, 3) + s1 * powf(t - 1, 2);
}
float SoheEasing::EaseInOutBack(const float& t) {
    const float s1 = 6.00158f;
    const float s2 = s1 + 1;
    if (t < 0.5f) {
        return 0.5f * (t * t * ((s2 + 1) * t - s2));
    }
    else {
        return 0.5f * (t * t * ((s2 + 1) * t - s2) + 2);
    }
}
float SoheEasing::EaseInElastic(const float& t) {
    const float p = 0.3f;
    const float s = p / 4.0f;
    if (t == 0.0f || t == 1.0f) return t;
    return -powf(2, 10 * (t - 1)) * sinf((t - 1 - s) * (2 * (float)M_PI) / p);
}
float SoheEasing::EaseOutElastic(const float& t) {
    const float p = 0.3f;
    const float s = p / 4.0f;
    if (t == 0.0f || t == 1.0f) return t;
    return powf(2, -10 * t) * sinf((t - s) * (2 * (float)M_PI) / p) + 1;
}
float SoheEasing::EaseInOutElastic(const float& t) {
    const float p = 0.45f;
    const float s = p / 4.0f;
    if (t == 0.0f || t == 1.0f) return t;
    float tt = t * 2;
    if (tt < 1) {
        return -0.5f * powf(2, 10 * (tt - 1)) * sinf((tt - 1 - s) * (2 * (float)M_PI) / p);
    }
    tt -= 1;
    return powf(2, -10 * tt) * sinf((tt - s) * (2 * (float)M_PI) / p) * 0.5f + 1;
}
float SoheEasing::EaseInBounce(const float& t) {
    return 1.0f - EaseOutBounce(1.0f - t);
}
float SoheEasing::EaseOutBounce(const float& t) {
    if (t < (1 / 2.75f)) {
        return 7.5625f * t * t;
    }
    else if (t < (2 / 2.75f)) {
        float t_ = t - (1.5f / 2.75f);
        return 7.5625f * t_ * t_ + 0.75f;
    }
    else if (t < (2.5 / 2.75f)) {
        float t_ = t - (2.25f / 2.75f);
        return 7.5625f * t_ * t_ + 0.9375f;
    }
    else {
        float t_ = t - (2.625f / 2.75f);
        return 7.5625f * t_ * t_ + 0.984375f;
    }
}
float SoheEasing::EaseInOutBounce(const float& t) {
    if (t < 0.5f) {
        return 0.5f * EaseInBounce(t * 2.0f);
    }
    else {
        return 0.5f * EaseOutBounce(t * 2.0f - 1.0f) + 0.5f;
    }
}