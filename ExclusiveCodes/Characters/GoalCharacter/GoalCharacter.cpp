#include "GoalCharacter.h"



DoorParts::DoorParts() {
    middle.model = MapModels::GetModelMusic(TagModel::kGearMiddle);
    eye.model = MapModels::GetModelMusic(TagModel::kGearEye);
    keyhole.model = MapModels::GetModelMusic(TagModel::kKeyHole);
    doorL.model = MapModels::GetModelMusic(TagModel::kDoor);
    doorR.model = MapModels::GetModelMusic(TagModel::kDoor);

    quad.Initialize(1.0f, 1.0f, { 0.0f,0.0f,-0.1f }, TagTexture::kCardBack);
    //quad.leftTop.texcoord.u = (-1.0f);
    //quad.rightTop.texcoord.u = (-1.0f);
    //quad.leftBottom.texcoord.u = (-1.0f);
    //quad.rightBottom.texcoord.u = (-1.0f);
    quad.color = COLOR_PURPLE;

    middle.trans.scale = { 1.5f,1.5f,1.5f };
    eye.trans.scale = { 1.5f,1.5f,1.5f };

    keyHolePos = { -0.25f,0.0f ,-0.1f };
    eyePos = { -0.25f,0.0f ,-0.171466f };
    middlePos = { -0.25f, 0.0f, -0.171466f };

    eye.color = COLOR_PURPLE;
    doorL.color = COLOR_BRONZE;
    doorR.color = COLOR_BRONZE;
}

GoalCharacter::Animator::Animator() {

}

//[CLOSE]--------------------------------------------------------------------------------------------------------
void GoalCharacter::Close::operator()(float counter_)
{


    if (counter_ < 0.25f) {
        float n = counter_ * 6.28f * 4.0f;
        float currentTheta = sinf(n) * 10.0f;
        float easeOutBack = Easing::EaseOutBack(doorParts->root.defaultQuatenion.z_axis.deltaTheta, currentTheta, 0.5f);
        doorParts->root.defaultQuatenion.z_axis.deltaTheta = easeOutBack;
    } else {
        doorParts->root.defaultQuatenion.z_axis.deltaTheta = Easing::Lerp(10.0f, 0.0f, counter_);
    }

    doorParts->Lkara.defaultQuatenion.y_axis.deltaTheta = 0.0f;
    doorParts->Rkara.defaultQuatenion.y_axis.deltaTheta = 0.0f;
    doorParts->eye.color = COLOR_PURPLE;
}

void GoalCharacter::Close::Initialize(std::map<std::string, CubeForModel>* parts_)
{
    parts = parts_;
}

//[UNLOCK]--------------------------------------------------------------------------------------------------------
void GoalCharacter::Unlock::operator()(float counter_)
{
    doorParts->root.defaultQuatenion.z_axis.deltaTheta = Easing::Lerp(doorParts->root.defaultQuatenion.z_axis.deltaTheta, 0.0f, 0.2f);

    float theta = Easing::EaseOutBack(0.0f, 1080.0f, counter_);
    doorParts->middle.trans.defaultQuatenion.z_axis.deltaTheta = -theta;
    doorParts->keyhole.trans.defaultQuatenion.z_axis.deltaTheta = theta;
    doorParts->eye.trans.defaultQuatenion.z_axis.deltaTheta = theta;
    doorParts->eye.color.x = Easing::Lerp(92.0f, 255.0f, counter_);
    doorParts->eye.color.y = Easing::Lerp(66.0f, 0.0f, counter_);
    doorParts->eye.color.z = Easing::Lerp(184.0f, 0.0f, counter_);
}

void GoalCharacter::Unlock::Initialize(std::map<std::string, CubeForModel>* parts_)
{
    parts = parts_;
}

//[OPEN]--------------------------------------------------------------------------------------------------------
void GoalCharacter::Open::operator()(float counter_)
{
    float theta = Easing::Lerp(0.0f, -60.0f, counter_);
    doorParts->Lkara.defaultQuatenion.y_axis.deltaTheta = theta;
    doorParts->Rkara.defaultQuatenion.y_axis.deltaTheta = -theta;

    doorParts->middle.trans.defaultQuatenion.z_axis.deltaTheta = Easing::Lerp(doorParts->middle.trans.defaultQuatenion.z_axis.deltaTheta, 0.0f, 0.5f);
    doorParts->keyhole.trans.defaultQuatenion.z_axis.deltaTheta = Easing::Lerp(doorParts->keyhole.trans.defaultQuatenion.z_axis.deltaTheta, 0.0f, 0.5f);
    doorParts->eye.trans.defaultQuatenion.z_axis.deltaTheta = Easing::Lerp(doorParts->eye.trans.defaultQuatenion.z_axis.deltaTheta, 0.0f, 0.5f);
    //float  = Easing::Lerp(0.0f, -60.0f, counter_);
  
    float sinF = sinf(doorParts->quad.trans.uvRotate);

    doorParts->quad.trans.uvRotate += 2.0f;
    doorParts->quad.trans.uvScale = { sinF * 2.0f ,sinF * 2.0f };
    doorParts->quad.trans.uvPos.v += 1.5f;


}

void GoalCharacter::Open::Initialize(std::map<std::string, CubeForModel>* parts_)
{
    parts = parts_;
}


void GoalCharacter::Update(Matrix4 vpMat_, int GameObjState_, float stateCount)
{

    (*animator.animFuncs[GameObjState_])(stateCount);

    doorParts.root.SetWorldMatBasedQuaternion();
    doorParts.Lkara.SetWorldMatBasedQuaternion();
    doorParts.Rkara.SetWorldMatBasedQuaternion();

    //モデルのアップデート

    doorParts.middle.Update(vpMat_);
    doorParts.eye.Update(vpMat_);
    doorParts.keyhole.Update(vpMat_);
    doorParts.doorL.Update(vpMat_);
    doorParts.doorR.Update(vpMat_);
    doorParts.quad.Update(vpMat_);

    //からのトランスフォームの更新をする
}

void GoalCharacter::Initialize(Transform4* objectTrans_)
{
    doorParts.root.BeChildren(objectTrans_);
}

void GoalCharacter::Draw(DrawMode mode_)
{
    doorParts.middle.Draw();
    doorParts.eye.Draw(0);
    doorParts.keyhole.Draw();
    doorParts.doorL.Draw();
    doorParts.doorR.Draw();

    NoviceLike::SetCullMode(CullMode::kNone);
    doorParts.quad.Draw(0);
    NoviceLike::SetCullMode(CullMode::kBack);

}

GoalCharacter::GoalCharacter()
{
    doorParts.Lkara.pos = { -0.75f,0.0f,-0.13f * 0.5f };
    doorParts.Rkara.pos = { 0.75f,0.0f,0 };


    doorParts.doorL.trans.pos = { -0.5f,0.0f,0.0f };
    doorParts.doorR.trans.pos = { -0.5f,0.0f,0.0f };

    doorParts.root.targetDir = { 0,0,-1 };
    doorParts.Lkara.targetDir = { 0,0,-1 };


    doorParts.Lkara.BeChildren(&doorParts.root);
    doorParts.Rkara.BeChildren(&doorParts.root);
    doorParts.quad.trans.BeChildren(&doorParts.root);

    doorParts.doorL.trans.BeChildren(&doorParts.Lkara);
    doorParts.doorR.trans.BeChildren(&doorParts.Rkara);

    doorParts.keyhole.trans.BeChildren(&doorParts.doorL.trans);
    doorParts.middle.trans.BeChildren(&doorParts.doorL.trans);
    doorParts.eye.trans.BeChildren(&doorParts.doorL.trans);

    doorParts.middle.trans.pos = doorParts.middlePos;
    doorParts.eye.trans.pos = doorParts.eyePos;
    doorParts.keyhole.trans.pos = doorParts.keyHolePos;

    close.Initialize(&parts);
    unlock.Initialize(&parts);
    open.Initialize(&parts);

    close.doorParts = &doorParts;
    unlock.doorParts = &doorParts;
    open.doorParts = &doorParts;

    //関数の代入
    animator.animFuncs.emplace_back(&close);
    animator.animFuncs.emplace_back(&unlock);
    animator.animFuncs.emplace_back(&open);



}

