#include "UnitSphere.h"

UnitSphere::UnitSphere()
{
	trans.uvPos = { 0.5f,0.5f };
	count++;
}

void UnitSphere::Update(Matrix4 vpMat_)
{
	trans.SetUVMat();
	trans.SetWorldMatBasedQuaternion();
	trans.SetWVP_Mat(vpMat_);
}
void UnitSphere::Draw(int enableLighting)
{
	NoviceLike::DrawSphere(color, trans.mat, texHandle, drawMode, trans.uvMat, enableLighting);
}

void UnitSphere::Initialize(Vector3 pos_, Vector3 scale_, TagTexture tag_, Vector4<float> color_)
{
	trans.scale = scale_;
	trans.pos = pos_;
	texHandle = (int)tag_;
	color = color_;
}

