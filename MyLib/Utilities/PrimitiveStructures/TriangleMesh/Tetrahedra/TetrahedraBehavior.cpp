#include "TetrahedraBehavior.h"


void TetrahedraBehavior::UV_Oyakoka()
{
	for (int i = 0; i < 4; ++i)
	{
		local_surfaces[i].trans.parent_uvMat = &trans.uvMat;
		local_surfaces[i].trans.uvPos = { 0,0 };
	}
}

void TetrahedraBehavior::SynchronizeTexAndColor()
{
	for (int i = 0; i < 4; ++i)
	{
		local_surfaces[i].texHandle = texHandle;
		local_surfaces[i].color = color;
	}
}

TetrahedraBehavior::TetrahedraBehavior()
{
	TetrahedraBehavior::count++;

	trans.uvPos = TriangleBehavior::kCentroid_equilateraltriangle;

	for (int i = 0; i < 4; ++i)
	{
		local_surfaces[i].trans.parent_trans = &trans;
	}

	trans.defaultQuatenion.x_axis.deltaTheta = 28.0F;
	trans.defaultQuatenion.z_axis.deltaTheta = -68.0f;

}

void TetrahedraBehavior::Initialize(Vector3 pos_, TagTexture texTag_, Vector4<float> color_,
	Vector3 local_leftPos_ ,Vector3 local_topPos_ ,
	Vector3 local_rightPos_,Vector3 furtherPos_)
{
	color = color_;
	texHandle = (int)texTag_;
	trans.pos = pos_;
	SetSurface(texTag_, color_,local_leftPos_, local_topPos_, local_rightPos_, furtherPos_);
}


void TetrahedraBehavior::SetSurface(TagTexture texTag_, Vector4<float> color_, Vector3 local_leftPos_, Vector3 local_topPos_,
	Vector3 local_rightPos_, Vector3 furtherPos_)
{
	//奥
	local_surfaces[0].SetVertex(local_leftPos_, local_topPos_, local_rightPos_);
	//local_surfaces[0].left.position = local_leftPos_;
	//local_surfaces[0].top.position = local_topPos_;
	//local_surfaces[0].right.position = local_rightPos_;
	//右
	local_surfaces[1].SetVertex(local_leftPos_, furtherPos_, local_topPos_);
	//local_surfaces[1].left.position = local_leftPos_;
	//local_surfaces[1].top.position = furtherPos_;
	//local_surfaces[1].right.position = local_topPos_;
	//左
	local_surfaces[2].SetVertex(local_topPos_, furtherPos_, local_rightPos_);
	//local_surfaces[2].left.position = local_topPos_;
	//local_surfaces[2].top.position = furtherPos_;
	//local_surfaces[2].right.position = local_rightPos_;
	//底面
	local_surfaces[3].SetVertex(local_rightPos_, furtherPos_, local_leftPos_);
	//local_surfaces[3].left.position = local_rightPos_;
	//local_surfaces[3].top.position = furtherPos_;
	//local_surfaces[3].right.position = local_leftPos_;

	for (int i = 0; i < 4; ++i)
	{
		local_surfaces[i].SetNormal();
		local_surfaces[i].SetTexcoord();
	}

	SynchronizeTexAndColor();
}
