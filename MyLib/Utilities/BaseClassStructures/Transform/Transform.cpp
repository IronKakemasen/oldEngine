#include "Transform.h"
#include "../../BenriFunctionStructures/BenriFunctions.h"

void UVTransform::SetUVMat()
{
	if (parent_uvMat)
	{
		uvMat = Get_SRTMat3D({ uvScale.u, uvScale.v ,0.0f }, { 0,0,uvRotate }, { uvPos.u,uvPos.v,0.0f });
		uvMat = uvMat.Multiply(*parent_uvMat);
	}

	else
	{
		uvMat = Get_SRTMat3D({ uvScale.u, uvScale.v ,0.0f }, { 0,0,uvRotate }, { uvPos.u,uvPos.v,0.0f });
	}
}

void Transform4::SetWorldMatBasedQuaternion()
{
	Vector4<float> quaternion = defaultQuatenion.GetComposite();

	//ターゲット方向のクォータニオン
	targetDir = targetDir.GetNormalized();
	Adjust(targetDir.z, -0.00001f, 0.00001f, 0.00001f);
	Vector4<float> target_qua = LookAt(targetDir, pos);
	//合成する
	quaternion = CompositeQuaternion(quaternion, target_qua);

	if (parent_trans)
	{
		mat.World = Get_SQrTMat3D(scale, quaternion, pos);
		mat.World = mat.World.Multiply(parent_trans->mat.World);
	}

	else
	{
		mat.World = Get_SQrTMat3D(scale, quaternion, pos);
	}
}

Matrix4 Transform4::GetWMat()
{
	Matrix4 ret;
	Vector4<float> quaternion = defaultQuatenion.GetComposite();

	//ターゲット方向のクォータニオン
	targetDir = targetDir.GetNormalized();
	Adjust(targetDir.z, -0.00001f, 0.00001f, 0.00001f);
	Vector4<float> target_qua = LookAt(targetDir, pos);
	//合成する
	quaternion = CompositeQuaternion(quaternion, target_qua);

	if (parent_trans)
	{
		ret = Get_SQrTMat3D(scale, quaternion, pos);
		ret = ret.Multiply(parent_trans->mat.World);
	}

	else
	{
		ret = Get_SQrTMat3D(scale, quaternion, pos);
	}

	return ret;
}

Matrix4 Transform4::GetRTMat()
{
	Matrix4 ret;
	Vector4<float> quaternion = defaultQuatenion.GetComposite();

	//ターゲット方向のクォータニオン
	targetDir = targetDir.GetNormalized();
	Adjust(targetDir.z, -0.00001f, 0.00001f, 0.00001f);
	Vector4<float> target_qua = LookAt(targetDir, pos);
	//合成する
	quaternion = CompositeQuaternion(quaternion, target_qua);

	if (parent_trans)
	{
		ret = Get_SQrTMat3D({1,1,1}, quaternion, pos);
		ret = ret.Multiply(parent_trans->mat.World);
	}

	else
	{
		ret = Get_SQrTMat3D({ 1,1,1 }, quaternion, pos);
	}

	return ret;

}

void Transform4::SetWVP_Mat(Matrix4 vpMat_)
{
	mat.WVP = mat.World.Multiply(vpMat_);

}

void Transform4::TranslatePosition(Vector3 const delta_pos_)
{
	//移動
	pos = pos + delta_pos_;
}

Vector3 Transform4::GetWorldPos()
{
	return
	{
		mat.World.m[3][0],
		mat.World.m[3][1],
		mat.World.m[3][2],
	};
}

void Transform4::BeChildren(Transform4* parent_)
{
	parent_trans = parent_;
}
