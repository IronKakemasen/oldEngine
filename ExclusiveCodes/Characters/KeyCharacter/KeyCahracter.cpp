#include "KeyCharacter.h"




void KeyCharacter::Update(Matrix4 vpMat_, int GameObjState_, float stateCount)
{

    //モデルのアップデート
    key.Update(vpMat_);

    
}

void KeyCharacter::Initialize(Transform4* objectTrans_)
{
    key.trans.BeChildren(objectTrans_);
  

}

void KeyCharacter::Draw(DrawMode mode_)
{
    key.Draw();
 
}

KeyCharacter::KeyCharacter()
{
    //int t[6];
    //Fill(t, (int)TagTexture::kTestGoal);
    //cube.Initialize(1.0f, 1, 1, { 0,0,0 }, t);

    key.model = MapModels::GetModelMusic(TagModel::kKey);
    key.color = { 255,255,0,255 };
}

