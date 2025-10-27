#include "Quad.h"

void Quad::Draw(int enableLighting_)
{
	NoviceLike::DrawQuad(leftTop, rightTop, leftBottom, rightBottom, texHandle, color, trans.mat, enableLighting_, drawMode, trans.uvMat);
}

void Quad::Update(Matrix4 vpMat_)
{
	trans.SetUVMat();
	trans.SetWorldMatBasedQuaternion();
	trans.SetWVP_Mat(vpMat_);
}
