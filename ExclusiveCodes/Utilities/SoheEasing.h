#pragma once

#include "../../MyLib/Utilities/BaseClassStructures/VectorAndMatrix/VectorAndMatrix.h"

// イージングタイプの列挙クラス
enum class EasingType
{
    EaseLinear,
    EaseInSine,
    EaseOutSine,
    EaseInOutSine,
    EaseInQuad,
    EaseOutQuad,
    EaseInOutQuad,
    EaseInCubic,
    EaseOutCubic,
    EaseInOutCubic,
    EaseInQuart,
    EaseOutQuart,
    EaseInOutQuart,
    EaseInExpo,
    EaseOutExpo,
    EaseInOutExpo,
    EaseInCirc,
    EaseOutCirc,
    EaseInOutCirc,
    EaseInBack,
    EaseOutBack,
    EaseInOutBack,
    EaseInElastic,
    EaseOutElastic,
    EaseInOutElastic,
    EaseInBounce,
    EaseOutBounce,
    EaseInOutBounce
};

class SoheEasing
{
public:
    // コンストラクタ
    SoheEasing();
    // デストラクタ
    ~SoheEasing();
public:
    // メンバ関数
    // イージング関数の設定
    void SetEasing(EasingType easingType);

    // イージング処理
  /*  template <typename T>*/
    void CountEaseLinear(int start, int end, int& current);
    void CountEaseLinear(float start, float end, float& current);
    void CountEaseLinear(unsigned int start, unsigned int end, unsigned int& current);
    void CountEaseLinear(Vector3 start, Vector3 end, Vector3& curren);
    void ReverseEaseLinear(float start, float end, float& current);
    void ReverseEaseLinear(Vector3 start, Vector3 end, Vector3& current);
    void OnceReverseEaseLinear(Vector3 start, Vector3 end, Vector3& current);
    void OnceReverseColorLinear(unsigned int start, unsigned int end, unsigned int& current);

    // イージングの初期化
    void InitEasing();
    // 色遷移のイージング関数
    void ReverseColorLinear(unsigned int start, unsigned int end, unsigned int& current);

public:
    // メンバ変数
    float interval = 0.01f;
    float cycle = 0.0f;
    float timer = 0.0f;
    float easeTimer = 0.0f;
    bool isEase = false;
    bool isReverse = true;
    bool hasReverse = false;
    int frameCount = 0;
    int runCount = 0;
    unsigned int fadeColor = 0x00000000;

    // スタートの変数
    unsigned int startColor = 0x000000ff;
    int intStartPos = 0;
    float floatStartPos = 0.0f;
    Vector2 vec2StartPos = { 0.0f, 0.0f };

    // エンドの変数
    unsigned int endColor = 0x000000ff;
    int intEndPos = 0;
    float floatEndPos = 0.0f;
    Vector2 vec2EndPos = { 0.0f, 0.0f };

private:
    // イージング関数
     // 静的メンバ関数として定義
    static float EaseLinear(const float& t);
    static float EaseInSine(const float& t);
    static float EaseOutSine(const float& t);
    static float EaseInOutSine(const float& t);
    static float EaseInQuad(const float& t);
    static float EaseOutQuad(const float& t);
    static float EaseInOutQuad(const float& t);
    static float EaseInCubic(const float& t);
    static float EaseOutCubic(const float& t);
    static float EaseInOutCubic(const float& t);
    static float EaseInQuart(const float& t);
    static float EaseOutQuart(const float& t);
    static float EaseInOutQuart(const float& t);
    static float EaseInExpo(const float& t);
    static float EaseOutExpo(const float& t);
    static float EaseInOutExpo(const float& t);
    static float EaseInCirc(const float& t);
    static float EaseOutCirc(const float& t);
    static float EaseInOutCirc(const float& t);
    static float EaseInBack(const float& t);
    static float EaseOutBack(const float& t);
    static float EaseInOutBack(const float& t);
    static float EaseInElastic(const float& t);
    static float EaseOutElastic(const float& t);
    static float EaseInOutElastic(const float& t);
    static float EaseInBounce(const float& t);
    static float EaseOutBounce(const float& t);
    static float EaseInOutBounce(const float& t);

    // イージング関数を取得
    float (*GetEasingFunction(EasingType easingType))(const float&);
    float (*easingFunc)(const float&);
    EasingType easingType_ = EasingType::EaseInOutSine;
};

