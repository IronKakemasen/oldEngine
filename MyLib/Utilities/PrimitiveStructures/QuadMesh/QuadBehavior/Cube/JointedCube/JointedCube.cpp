#include "JointedCube.h"

JointedCube::JointedCube ()
{

}

void JointedCube ::Update(Matrix4 vpMat_)
{
	if (!do_purge)
	{
		ConnectVertexes();
	}

	else
	{
		Purge();
	}

	trans.SetWorldMatBasedQuaternion();
	trans.SetUVMat();

	//毎フレーム頂点を結合する

	for (int i = 0; i < 6; ++i)
	{
		//毎フレーム法線を計算
		local_surfaces[i].SetNormal();
		local_surfaces[i].Update(vpMat_);
	}
}

void JointedCube ::Draw(int enableLighting_)
{
	for (int k = 0; k < 6; ++k)
	{
		NoviceLike::DrawQuad(local_surfaces[k].leftTop, local_surfaces[k].rightTop,
			local_surfaces[k].leftBottom, local_surfaces[k].rightBottom, local_surfaces[k].texHandle, local_surfaces[k].color,
			local_surfaces[k].trans.mat, enableLighting_, drawMode, local_surfaces[k].trans.uvMat);
	}
}

