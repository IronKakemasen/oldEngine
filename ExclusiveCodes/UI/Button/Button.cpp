#include "Button.h"
#include "../../../MyLib/Utilities/EasingFuctionStructures/EasingFunctions.h"
#include "../../InGameMouse/InGameMouse.h"
#include"../UI.h"

//上に凸の二次関数　放物線　0~1まで
template<typename T>
static inline T Concave(const T& st_, const T& end_, float t_)
{

    float y = -4.0f * pow(t_ - 1.0f / 2.0f, 2.0f) + 1.0f;

    return Easing::EaseOutBack(st_, end_, y);
}


Button::Button(const float& width, const float& height, const Vector3& position, bool isCircle) {

    width_ = width;
    height_ = height;
    //ボタンのスプライトの初期化
    sprite.InitializeSprite(position, width_, height_, (int)TagTexture::kWhite2x2, { 255,255,255,255 });
    //sprite.trans.pos = position;
    isCircle_ = isCircle;
    if (isCircle) {
        width_ /= 2.0f;
    }
    
};

void Button::SetTextureHandle(int textureHandle) {

    //テクスチャハンドル
    sprite.texHandle = textureHandle;

};


void Button::Initialize() {
    scalingTimer = 0.0f;
};


void Button::TimerUpdate() {

    if (scalingTimer == 1.0f) { return; }

    scalingTimer += 1.0f / 20.0f;

    if (scalingTimer > 1.0f) {
        scalingTimer = 1.0f;
    }
};

void Button::Update(InGameMouse& inGameMouse, const Matrix4& vpMat) {

    //ホバーを偽に
    isHover = false;
    //トリガーを偽に
    isTrigger = false;


    if (isCircle_) {
        if (inGameMouse.CollisionWithMouse2D({ sprite.trans.pos.x, sprite.trans.pos.y }, width_, vpMat)) {
            //ホバーする
            isHover = true;
        }
    } else {
        if (inGameMouse.CollisionWithMouseAABB2D(sprite.trans.pos, width_, height_, 8.0f)) {
            //ホバーする
            isHover = true;
        }
    }



    if (isHover) {

        if (NoviceLike::mouseState.rgbButtons[0] && !NoviceLike::preMouseState.rgbButtons[0]) {
            //マウスを押した瞬間
            isTrigger = true;
            scalingTimer = 0.0f;
        }
    }

    //ステータスごとの処理
    if (isTrigger) {

        sprite.color = triggerColor;

        if (onTrigger) {
            onTrigger();
        }

    } else if (isHover) {
        //ホバー時の処理
        sprite.color = hoverColor;
    } else {
        //それ以外の時の処理
        sprite.color = baseColor;

    }

    TimerUpdate();

    sprite.trans.scale.x = Concave(1.0f, 1.2f, scalingTimer);
    sprite.trans.scale.y = Concave(1.0f, 1.2f, scalingTimer);

    sprite.Update(vpMat);

};

void Button::Draw() {

    sprite.Draw();
};

Button::~Button() {

}
