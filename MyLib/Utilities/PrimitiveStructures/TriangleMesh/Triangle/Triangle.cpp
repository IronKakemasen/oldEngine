#include "Triangle.h"


void Triangle::Update(Matrix4 vpMat_)
{
	trans.SetUVMat();
	trans.SetWorldMatBasedQuaternion();
	trans.SetWVP_Mat(vpMat_);
}

void Triangle::Draw(int  enableLighting)
{
	NoviceLike::DrawTriangle(left, top, right, color, texHandle, trans.mat, drawMode, trans.uvMat,enableLighting);

}
