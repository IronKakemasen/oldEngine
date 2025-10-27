#pragma once

#include "../../../MyLib/Utilities/PrimitiveStructures/QuadMesh/QuadBehavior/Sprite/Sprite.h"
#include<functional>

struct InGameMouse;
class InGameController;
class Button
{
public:
    Button(const float& width, const float& height, const Vector3& position, bool isCircle);

    ~Button();
    void Update(InGameMouse& inGameMouse, const Matrix4& vpMat);
    void Draw();
    void SetTextureHandle(int textureHandle);
    void TimerUpdate();
    void Initialize();

    //ボタン
    Sprite sprite;
    //押したフラグ
    bool isTrigger = false;
    //ホバー
    bool isHover = false;
    bool isCircle_ = false;
    float scalingTimer = 0.0f;
    float width_ = { 0.0f };
    float height_ = { 0.0f };
    //外部から設定できるコールバック
    std::function<void()> onTrigger;
    Vector4<float> triggerColor = { 255,255,255,255 };
    Vector4<float> hoverColor = { 255,255,255,255 };
    Vector4<float> baseColor = { 255,255,255,128 };
};

