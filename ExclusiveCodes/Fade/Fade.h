#pragma once

#include"../../MyLib/Utilities/PrimitiveStructures/QuadMesh/QuadBehavior/Sprite/Sprite.h"
#include"../../MyLib/Utilities/BaseClassStructures/VectorAndMatrix/VectorAndMatrix.h"

class Fade
{
public:
   enum State {
       kFadeIn,
       kFadeOut
   };

   void Update();
   void Draw();
   void FadeStart();
   void TimerUpdate();
   //状態をカエル
   void SwitchStatus(float inv_time_ = 0.01666666666f);

   void ChangStatus(State state, float inv_time_ = 0.01666666666f);
   Fade();

   Sprite sprite_;
   State state_ = kFadeOut;
   int textureHandle_ = -1;
   Vector4<float> color_ = { 0.0f, 0.0f, 0.0f, 255.0f }; // Fixed template argument
   float timer_ = 0.0f;
   float fps_ = 60.0f;
   float startAlpha_ = 0.0f;
   float endAlpha_ = 255.0f;
};
