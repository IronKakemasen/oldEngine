#include "BaseClass.h"

float const inv_255 = 1.0f / 255.0f;
float const inv_100 = 1.0f / 100.0f;

Vector2 ConvertToScreen(Vector3 dst_, Matrix4& viewProjectionMat_)
{
	Vector4<float> tmp = { dst_.x,dst_.y,dst_.z,1.0f };

	Matrix4 VPV_mat = viewProjectionMat_.Multiply(Get_ViewportTransformation3D());
	tmp = tmp.GetMultiply(VPV_mat);
	float const inv_w = 1.0f / tmp.w;

	//チェック
	tmp = { tmp.x * inv_w ,tmp.y * inv_w  ,tmp.z * inv_w ,1.0f };

	return Vector2{ tmp.x ,tmp.y };
}
