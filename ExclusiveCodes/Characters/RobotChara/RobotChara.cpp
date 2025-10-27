#include "RobotChara.h"

CharacterParts::CharacterParts()
{
    around.model = MapModels::GetModelMusic(TagModel::kGearAround);
    center.model = MapModels::GetModelMusic(TagModel::kGearCenter);
    eye.model = MapModels::GetModelMusic(TagModel::kGearEye);
    large.model = MapModels::GetModelMusic(TagModel::kGearLarge);
    middle.model = MapModels::GetModelMusic(TagModel::kGearMiddle);
    g_small.model = MapModels::GetModelMusic(TagModel::kGearSmall);

    centerPos = { 0,0,0.0557f };
    largePos = { 0.15827f,0.112879f,0 };
    g_smallPos = { -0.23951f,0.13557f,0 };
    eyePos = { -0.12263f,-0.19237f,0 };

    center.color = { 255,160,92,255 };
    eye.color = { 255,0,0,255 };
    large.color = { 92,92,255,255 };

}

//[Animator]--------------------------------------------------------------------------------------------------------
void RobotChara::Animator::Initialize(std::function<bool(float counter_)>* funcs_)
{
    //for (int i = 0; i < (int)AnimationState::kCount; ++i)
    //{
    //	animFuncs[i] = &funcs_[i];S
    //}
}

RobotChara::Animator::Animator()
{

}

std::string RobotChara::Animator::OutputStateInString(int state_)
{

    return "iyudageiueaob";

}




//[RobotChara]--------------------------------------------------------------------------------------------------------
RobotChara::RobotChara()
{
    charaParts.around.trans.pos = {};
    charaParts.center.trans.pos = charaParts.centerPos;
    charaParts.large.trans.pos = charaParts.largePos;
    charaParts.g_small.trans.pos = charaParts.g_smallPos;
    charaParts.eye.trans.pos = charaParts.eyePos;

    //[ ペアレント化 ]
    charaParts.around.trans.BeChildren(&charaParts.kara);
    charaParts.center.trans.BeChildren(&charaParts.kara);
    charaParts.eye.trans.BeChildren(&charaParts.kara);
    charaParts.large.trans.BeChildren(&charaParts.kara);
    charaParts.g_small.trans.BeChildren(&charaParts.kara);
    charaParts.middle.trans.BeChildren(&charaParts.eye.trans);


    //[ 特定の面をマウスから干渉できなくする ]
    //各アニメーションの初期化
    idle.Initialize(&parts);
    moving.Initialize(&parts);
    jumping.Initialize(&parts);
    dead.Initialize(&parts);
    enter.Initialize(&parts);
    falling.Initialize(&parts);
    goal.Initialize(&parts);


    idle.charaParts = &charaParts;
    moving.charaParts = &charaParts;
    jumping.charaParts = &charaParts;
    dead.charaParts = &charaParts;
    enter.charaParts = &charaParts;
    falling.charaParts = &charaParts;
    goal.charaParts = &charaParts;


    //関数の代入
    animator.animFuncs.emplace_back(&idle);
    animator.animFuncs.emplace_back(&moving);
    animator.animFuncs.emplace_back(&jumping);
    animator.animFuncs.emplace_back(&falling);
    animator.animFuncs.emplace_back(&dead);
    animator.animFuncs.emplace_back(&enter);
    animator.animFuncs.emplace_back(&goal);

}


void RobotChara::Draw(DrawMode mode_)
{

    charaParts.around.Draw();
    charaParts.center.Draw();
    charaParts.eye.Draw();
    charaParts.large.Draw();
    charaParts.g_small.Draw();
    charaParts.middle.Draw();

}
void RobotChara::Initialize(Transform4* objectTrans_)
{
    charaParts.kara.BeChildren(objectTrans_);
}

void RobotChara::Reset()
{

}


void RobotChara::Update(Matrix4 vpMat_, int GameObjState_, float stateCount)
{

    (*animator.animFuncs[GameObjState_])(stateCount);

    charaParts.kara.SetWorldMatBasedQuaternion();

    //モデルのアップデート
    charaParts.around.Update(vpMat_);
    charaParts.center.Update(vpMat_);
    charaParts.eye.Update(vpMat_);
    charaParts.large.Update(vpMat_);
    charaParts.g_small.Update(vpMat_);
    charaParts.middle.Update(vpMat_);


}

//[IDLE]--------------------------------------------------------------------------------------------------------
void RobotChara::Idle::operator()(float counter_)
{
 /*   float const numRotate = 2.0f;

    float n2 = counter_ * 6.28f * numRotate;*/
    float n = counter_ * 6.28f;

    float thetaX = Easing::Lerp(0.0f, 360.0f, counter_);

    charaParts->around.trans.defaultQuatenion.x_axis.deltaTheta += 3;
    charaParts->center.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;
    charaParts->g_small.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;
    charaParts->large.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;

    float currentTheta = sinf(n) * 20.0f + 160.0f;

    charaParts->middle.trans.defaultQuatenion.x_axis.deltaTheta = currentTheta;
    charaParts->middle.trans.defaultQuatenion.z_axis.deltaTheta = -thetaX;

    charaParts->around.trans.pos = Easing::Lerp(charaParts->around.trans.pos, { 0.0f,0.0f,0.0f }, 0.1f);
    charaParts->center.trans.pos = Easing::Lerp(charaParts->center.trans.pos, charaParts->centerPos, 0.1f);
    charaParts->eye.trans.pos = Easing::Lerp(charaParts->eye.trans.pos, charaParts->eyePos, 0.1f);
    charaParts->g_small.trans.pos = Easing::Lerp(charaParts->g_small.trans.pos, charaParts->g_smallPos, 0.1f);
    charaParts->large.trans.pos = Easing::Lerp(charaParts->large.trans.pos, charaParts->largePos, 0.1f);

}

void RobotChara::Idle::Initialize(std::map<std::string, CubeForModel>* parts_)
{
    parts = parts_;
}

//[MOVE]--------------------------------------------------------------------------------------------------------
void RobotChara::Moving::operator()(float counter_)
{
    float thetaX = Easing::Lerp(0.0f, 720.0f, counter_);

    //ここでthetaを拝借してループ運動を作成
    float currentTheta = sinf(thetaX) * 45.0f + 90.0f;

    float n = counter_ * 6.28f;

    charaParts->around.trans.defaultQuatenion.x_axis.deltaTheta = Easing::Lerp(charaParts->around.trans.defaultQuatenion.x_axis.deltaTheta, currentTheta, 0.2f);

    charaParts->center.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;
    charaParts->g_small.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;
    charaParts->large.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;
    charaParts->middle.trans.defaultQuatenion.x_axis.deltaTheta = Easing::Lerp(charaParts->eye.trans.defaultQuatenion.x_axis.deltaTheta, 0.0f, 0.2f);
    charaParts->middle.trans.defaultQuatenion.z_axis.deltaTheta = -thetaX;

    charaParts->around.trans.pos = Easing::Lerp(charaParts->around.trans.pos, { 0.0f,0.0f,0.0f }, 0.1f);
    charaParts->center.trans.pos = Easing::Lerp(charaParts->center.trans.pos, charaParts->centerPos, 0.1f);
    charaParts->eye.trans.pos = Easing::Lerp(charaParts->eye.trans.pos, charaParts->eyePos, 0.1f);
    charaParts->g_small.trans.pos = Easing::Lerp(charaParts->g_small.trans.pos, charaParts->g_smallPos, 0.1f);
    charaParts->large.trans.pos = Easing::Lerp(charaParts->large.trans.pos, charaParts->largePos, 0.1f);

}

void RobotChara::Moving::Initialize(std::map<std::string, CubeForModel>* parts_)
{
    parts = parts_;
}

//[Jumping]--------------------------------------------------------------------------------------------------------
void RobotChara::Jumping::operator()(float counter_)
{

    float thetaX = Easing::Lerp(0.0f, 360.0f, counter_);

    charaParts->around.trans.defaultQuatenion.x_axis.deltaTheta += 3;

    charaParts->center.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;
    charaParts->g_small.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;
    charaParts->large.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;
    charaParts->middle.trans.defaultQuatenion.z_axis.deltaTheta = -thetaX;

    charaParts->g_small.trans.defaultQuatenion.x_axis.deltaTheta = thetaX;
    charaParts->large.trans.defaultQuatenion.x_axis.deltaTheta = -thetaX;
    charaParts->middle.trans.defaultQuatenion.x_axis.deltaTheta = -thetaX;

    charaParts->around.trans.pos.y = Easing::Lerp(0.0f, 0.1f, counter_);
    charaParts->eye.trans.pos.y = Easing::Lerp(charaParts->eyePos.y, charaParts->eyePos.y + 0.3f, counter_);
    charaParts->g_small.trans.pos.y = Easing::Lerp(charaParts->g_smallPos.y, charaParts->g_smallPos.y + 0.4f, counter_);
    charaParts->large.trans.pos.y = Easing::Lerp(charaParts->largePos.y, charaParts->largePos.y + 0.4f, counter_);

}

void RobotChara::Jumping::Initialize(std::map<std::string, CubeForModel>* parts_)
{
    parts = parts_;
}

//[Dead]--------------------------------------------------------------------------------------------------------
void RobotChara::Dead::operator()(float counter_)
{

    float thetaX = Easing::EaseOutQuint(0.0f, 360.0f, counter_);

    charaParts->around.trans.defaultQuatenion.x_axis.deltaTheta = Easing::Lerp(charaParts->around.trans.defaultQuatenion.x_axis.deltaTheta, 135.0f, 0.2f);
    charaParts->around.trans.defaultQuatenion.y_axis.deltaTheta = thetaX * 4.0f;
    charaParts->center.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;
    charaParts->g_small.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;
    charaParts->large.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;
    charaParts->middle.trans.defaultQuatenion.z_axis.deltaTheta = -thetaX;

    charaParts->g_small.trans.defaultQuatenion.x_axis.deltaTheta = thetaX;
    charaParts->large.trans.defaultQuatenion.x_axis.deltaTheta = -thetaX;
    charaParts->middle.trans.defaultQuatenion.x_axis.deltaTheta = -thetaX;

    charaParts->around.trans.pos = Easing::Lerp(Vector3{ 0.0f,0.0f,0.0f }, Vector3{ -0.1f,-0.1f,0.0f }, counter_);
    charaParts->center.trans.pos.y = Easing::Lerp(charaParts->centerPos.y, charaParts->centerPos.y + 0.5f, counter_);
    charaParts->eye.trans.pos = Easing::Lerp(charaParts->eyePos, { charaParts->eyePos.x - 0.3f,charaParts->eyePos.y + 0.3f,charaParts->eyePos.z }, counter_);
    charaParts->g_small.trans.pos.y = Easing::Lerp(charaParts->g_smallPos.y, charaParts->g_smallPos.y + 0.4f, counter_);
    charaParts->large.trans.pos = Easing::Lerp(charaParts->largePos, { charaParts->largePos.x + 0.2f,charaParts->largePos.y + 0.2f,charaParts->largePos.z }, counter_);
}

void RobotChara::Dead::Initialize(std::map<std::string, CubeForModel>* parts_)
{
    parts = parts_;
}

//[Enter]--------------------------------------------------------------------------------------------------------
void RobotChara::Enter::operator()(float counter_)
{


    float thetaX = Easing::Lerp(360.0f, 0.0f, counter_);

    charaParts->around.trans.defaultQuatenion.x_axis.deltaTheta += 6;
    charaParts->center.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;

    if (counter_ < 0.5f) {
        charaParts->eye.trans.scale = Easing::Lerp(Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 2.0f,2.0f,2.0f }, counter_);
        charaParts->g_small.trans.scale = Easing::Lerp(Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 2.0f,2.0f,2.0f }, counter_);
        charaParts->large.trans.scale = Easing::Lerp(Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 2.0f,2.0f,2.0f }, counter_);
        charaParts->center.trans.scale = Easing::Lerp(Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 2.0f,2.0f,2.0f }, counter_);
    } else {
        charaParts->eye.trans.scale = Easing::EaseOutBounce(Vector3{ 2.0f,2.0f,2.0f }, Vector3{ 1.0f,1.0f,1.0f }, counter_);
        charaParts->g_small.trans.scale = Easing::EaseOutBounce(Vector3{ 2.0f,2.0f,2.0f }, Vector3{ 1.0f,1.0f,1.0f }, counter_);
        charaParts->large.trans.scale = Easing::EaseOutBounce(Vector3{ 2.0f,2.0f,2.0f }, Vector3{ 1.0f,1.0f,1.0f }, counter_);
        charaParts->center.trans.scale = Easing::EaseOutBounce(Vector3{ 2.0f,2.0f,2.0f }, Vector3{ 1.0f,1.0f,1.0f }, counter_);
    }
}

void RobotChara::Enter::Initialize(std::map<std::string, CubeForModel>* parts_)
{
    parts = parts_;
}



//[Falling]--------------------------------------------------------------------------------------------------------
void RobotChara::Falling::operator()(float counter_)
{

    float thetaX = Easing::Lerp(0.0f, 360.0f, counter_);

    charaParts->around.trans.defaultQuatenion.x_axis.deltaTheta += 3;

    charaParts->center.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;
    charaParts->g_small.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;
    charaParts->large.trans.defaultQuatenion.z_axis.deltaTheta = thetaX;


    charaParts->around.trans.pos.y = Easing::Lerp(0.1f, 0.0f, counter_);
    charaParts->eye.trans.pos.y = Easing::Lerp(charaParts->eyePos.y + 0.3f, charaParts->eyePos.y, counter_);
    charaParts->g_small.trans.pos.y = Easing::Lerp(charaParts->g_smallPos.y + 0.4f, charaParts->g_smallPos.y, counter_);
    charaParts->large.trans.pos.y = Easing::Lerp(charaParts->largePos.y + 0.4f, charaParts->largePos.y, counter_);


}

void RobotChara::Falling::Initialize(std::map<std::string, CubeForModel>* parts_)
{
    parts = parts_;
}

//[Goal]--------------------------------------------------------------------------------------------------------
void RobotChara::Goal::operator()(float counter_)
{
    float theta = Easing::Lerp(0.0f, 360.0f, counter_);
    float easingTheta = Easing::EasingInBack(0.0f,720.0f,counter_);

    charaParts->kara.defaultQuatenion.y_axis.deltaTheta = easingTheta;

    charaParts->around.trans.defaultQuatenion.x_axis.deltaTheta = Easing::Lerp(charaParts->around.trans.defaultQuatenion.x_axis.deltaTheta, 135.0f, 0.2f);
    charaParts->around.trans.defaultQuatenion.y_axis.deltaTheta = theta * 4.0f;

    charaParts->center.trans.defaultQuatenion.z_axis.deltaTheta = theta;
    charaParts->g_small.trans.defaultQuatenion.z_axis.deltaTheta = theta;
    charaParts->large.trans.defaultQuatenion.z_axis.deltaTheta = theta;
    charaParts->middle.trans.defaultQuatenion.z_axis.deltaTheta = -theta;

}

void RobotChara::Goal::Initialize(std::map<std::string, CubeForModel>* parts_)
{

}


