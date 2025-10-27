#include "DisjointedCube.h"

DisjointedCube::DisjointedCube()
{

}
void DisjointedCube::Update(Matrix4 vpMat_)
{
	trans.SetWorldMatBasedQuaternion();
	trans.SetUVMat();

	//上面 = 0 , 底面 = 1 , 側面(左) = 2 , 側面(右) = 3 , 側面(奥) = 4 , 側面(手前) = 5
	for (int i = 0; i < 6; ++i)
	{
		local_surfaces[i].Update(vpMat_);
	}
}

void DisjointedCube::Draw(int enableLighting_)
{
	for (int k = 0; k < 6; ++k)
	{
		NoviceLike::DrawQuad(local_surfaces[k].leftTop, local_surfaces[k].rightTop,
			local_surfaces[k].leftBottom, local_surfaces[k].rightBottom, local_surfaces[k].texHandle, local_surfaces[k].color,
			local_surfaces[k].trans.mat, enableLighting_, drawMode, local_surfaces[k].trans.uvMat);
	}

}

