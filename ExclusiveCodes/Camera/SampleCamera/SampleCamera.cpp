#include "SampleCamera.h"
#include "../../GameObjects/Player/Player.h"

SampleCamera::SampleCamera()
{
    Initialize();
}

void SampleCamera::Update()
{

    trans.SetWorldMatBasedQuaternion();
    viewProjectionMat = CreateViewProjectionMatrix();

    SendCameraInfomation();
}

void SampleCamera::Initialize()
{
    trans.pos = { 0.0f,5.0f,-3.0f };
    trans.defaultQuatenion.x_axis.deltaTheta = 20.0f;
}

