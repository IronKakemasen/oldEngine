#include "Fade.h"
#include "../../MyLib/Utilities/EasingFuctionStructures/EasingFunctions.h"

Fade::Fade() {

    textureHandle_ = (int)TagTexture::kWhite2x2;
    sprite_.InitializeSprite({ 0,0,0 }, 2560, 1440, textureHandle_, color_);

};

void Fade::FadeStart() {
    timer_ = 0.0f;
};

//状態をカエル
void Fade::SwitchStatus(float inv_time_) {

    fps_ = inv_time_;
    state_ = (state_ == kFadeIn) ? kFadeOut : kFadeIn;
    FadeStart();

};

void Fade::ChangStatus(State state,float inv_time_) {

    fps_ = inv_time_;
    state_ = state;
    FadeStart();

};

void Fade::TimerUpdate() {

    if (timer_ == 1.0f) { return; }

    timer_ += 1.0f * fps_;

    if (timer_ > 1.0f) {
        timer_ = 1.0f;
    }
};


void Fade::Update() {

    TimerUpdate();

    if (state_ == kFadeIn) {
        sprite_.color.w = Easing::EaseOutCubic(startAlpha_, endAlpha_, timer_);
    }

    if (state_ == kFadeOut) {
        sprite_.color.w = Easing::EaseOutCubic(startAlpha_, endAlpha_, 1.0f - timer_);
    }

    sprite_.Update();
}

void Fade::Draw() {

    sprite_.Draw();

};;
