#include "CubeBehavior.h"


void CubeBehavior::SynchronizeColor()
{
	for (int i = 0; i < 6; ++i)
	{
		local_surfaces[i].color = color;
	}
}

void CubeBehavior::Purge()
{
	static float const purgeSpeed = 0.05f;
	
	for (int i = 0; i < 6; ++i)
	{
		local_surfaces[i].trans.pos = local_surfaces[i].trans.pos + local_surfaces[i].purgeDir * purgeSpeed;

		local_surfaces[i].trans.defaultQuatenion.x_axis.deltaTheta += 
			local_surfaces[i].purgeDir.x * local_surfaces[i].purgeRotateSpeed;
		local_surfaces[i].trans.defaultQuatenion.y_axis.deltaTheta +=
			local_surfaces[i].purgeDir.x * local_surfaces[i].purgeRotateSpeed;
		local_surfaces[i].trans.defaultQuatenion.z_axis.deltaTheta +=
			local_surfaces[i].purgeDir.x * local_surfaces[i].purgeRotateSpeed;

	}

}


void CubeBehavior::UseAsSpecial()
{
	for (int i = 0; i < 6; ++i)
	{
		local_surfaces[i].ResetTexcoord();
	}
}


void CubeBehavior::UV_Oyakoka()
{
	for (int i = 0; i < 6; ++i)
	{
		local_surfaces[i].trans.uvPos = { 0.0f,0.0f};
		local_surfaces[i].trans.parent_uvMat = &trans.uvMat;
	}
}

void CubeBehavior::SynchronizeTexAndColor()
{
	for (int i = 0; i < 6; ++i)
	{
		local_surfaces[i].texHandle = texHandle;
		local_surfaces[i].color = color;
	}
}


void CubeBehavior::SetSurface(float width_, float height_, float depth_, int* texHandles)
{
	Vector3 tmpLocal_pos = { 0.0f,0.0f,depth_ * 0.5f };

	auto tmpFunc = [](Quad& rectShape_, Vector3 const rotate_, Vector3 const pos_)
	{
		Matrix4 tmpMat = Get_STRMat3D({ 1.0f,1.0f,1.0f }, rotate_, pos_);

		rectShape_.leftTop.position = rectShape_.leftTop.position.GetMultiply(tmpMat);
		rectShape_.rightTop.position = rectShape_.rightTop.position.GetMultiply(tmpMat);
		rectShape_.leftBottom.position = rectShape_.leftBottom.position.GetMultiply(tmpMat);
		rectShape_.rightBottom.position = rectShape_.rightBottom.position.GetMultiply(tmpMat);
	};

	//上面 = 0 , 底面 = 1 , 側面(左) = 2 , 側面(右) = 3 , 側面(奥) = 4 , 側面(手前) = 5
	local_surfaces[0].SetVertex(width_, depth_);	//上面
	local_surfaces[1].SetVertex(width_, depth_);	//底面
	local_surfaces[2].SetVertex(depth_, height_);	//側面(左)
	local_surfaces[3].SetVertex(depth_, height_);	//側面(右)
	local_surfaces[4].SetVertex(width_, height_);	//側面(奥)
	local_surfaces[5].SetVertex(width_, height_);	//側面(手前)

	//回転
	tmpFunc(local_surfaces[0], { 90.0f,0.0f,0.0f }, { 0.0f,0.0f,-height_ * 0.5f });
	tmpFunc(local_surfaces[1], { -90.0f,0.0f,0.0f }, { 0.0f,0.0f,-height_ * 0.5f });
	tmpFunc(local_surfaces[2], { 0.0f,90.0f,0.0f }, { 0.0f,0.0f,-width_ * 0.5f });
	tmpFunc(local_surfaces[3], { 0.0f,-90.0f,0.0f }, { 0.0f,0.0f,-width_ * 0.5f });
	tmpFunc(local_surfaces[4], { 180.0f,0.0f,0.0f }, { 0.0f,0.0f,-depth_ * 0.5f });
	tmpFunc(local_surfaces[5], { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,-depth_ * 0.5f });


	for (int i = 0; i < 6; ++i)
	{
		local_surfaces[i].SetNormal();
		local_surfaces[i].SetTexcoord();
		local_surfaces[i].texHandle = texHandles[i];
		local_surfaces[i].color = color;

	}

}

void CubeBehavior::SetNormal()
{
	for (int i = 0; i < 6; ++i)
	{
		local_surfaces[i].SetNormal();
	}
}


CubeBehavior::CubeBehavior()
{
	CubeBehavior::count++;

	trans.uvPos = { 0.5f,0.5f};

	for (int i = 0; i < 6; ++i)
	{
		local_surfaces[i].trans.parent_trans = &trans;
	}

}


void CubeBehavior::Initialize(float width_, float height_, float depth_, Vector3 local_pos_, int* texHandles, Vector4<float> commonColor_)
{
	size = { width_ ,height_ ,depth_ };
	color = commonColor_;
	SetSurface(width_, height_, depth_, texHandles);
	trans.pos = local_pos_;
}

void CubeBehavior::ConnectVertexes()
{
	//6面体を構成する矩形の頂点を結合
	local_surfaces[C_LEFT].leftTop.position = local_surfaces[C_TOP].leftTop.position;
	local_surfaces[C_BEYOND].rightTop.position = local_surfaces[C_TOP].leftTop.position;

	local_surfaces[C_BEYOND].leftTop.position = local_surfaces[C_TOP].rightTop.position;
	local_surfaces[C_RIGHT].rightTop.position = local_surfaces[C_TOP].rightTop.position;

	local_surfaces[C_FRONT].rightTop.position = local_surfaces[C_TOP].rightBottom.position;
	local_surfaces[C_RIGHT].leftTop.position = local_surfaces[C_TOP].rightBottom.position;

	local_surfaces[C_FRONT].leftTop.position = local_surfaces[C_TOP].leftBottom.position;
	local_surfaces[C_LEFT].rightTop.position = local_surfaces[C_TOP].leftBottom.position;


	local_surfaces[C_LEFT].leftBottom.position = local_surfaces[C_BOTTOM].leftBottom.position;
	local_surfaces[C_BEYOND].rightBottom.position = local_surfaces[C_BOTTOM].leftBottom.position;

	local_surfaces[C_BEYOND].leftBottom.position = local_surfaces[C_BOTTOM].rightBottom.position;
	local_surfaces[C_RIGHT].rightBottom.position = local_surfaces[C_BOTTOM].rightBottom.position;

	local_surfaces[C_FRONT].leftBottom.position = local_surfaces[C_BOTTOM].leftTop.position;
	local_surfaces[C_RIGHT].leftBottom.position = local_surfaces[C_BOTTOM].rightTop.position;

	local_surfaces[C_FRONT].rightBottom.position = local_surfaces[C_BOTTOM].rightTop.position;
	local_surfaces[C_LEFT].rightBottom.position = local_surfaces[C_BOTTOM].leftTop.position;

}

float CubeBehavior::GetBotttomMinPos()
{
	float min = local_surfaces[C_BOTTOM].leftTop.position.y;

	if (min > local_surfaces[C_BOTTOM].rightTop.position.y)
	{
		min = local_surfaces[C_BOTTOM].rightTop.position.y;
	}

	if (min > local_surfaces[C_BOTTOM].rightBottom.position.y)
	{
		min = local_surfaces[C_BOTTOM].rightBottom.position.y;
	}

	if (min > local_surfaces[C_BOTTOM].leftBottom.position.y)
	{
		min = local_surfaces[C_BOTTOM].leftBottom.position.y;
	}

	return min;
}


