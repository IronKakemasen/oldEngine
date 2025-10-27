#include "CameraBehavior.h"
#include <algorithm>
#include "SphericalDebugCamera.h"
#include "SimpleDebugCamera.h"
#include "../../NoviceLike/NoviceLike.h"

Matrix4 CameraBehavior::CreateViewProjectionMatrix()
{
	return Get_VPMat(trans.mat.World);
}

void CameraBehavior::SendCameraInfomation()
{
	Vector3 tmp = tmp.GetMultiply(trans.mat.World);
	NoviceLike::SetCameraPos(trans.GetWorldPos());
}



