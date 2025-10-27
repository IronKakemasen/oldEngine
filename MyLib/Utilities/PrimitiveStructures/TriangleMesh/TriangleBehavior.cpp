#include "TriangleBehavior.h"

TriangleBehavior::TriangleBehavior()
{
	TriangleBehavior::count++;
	trans.uvPos = kCentroid_equilateraltriangle;
}

void TriangleBehavior::Initialize(float width_, float height_, Vector3 pos_, int texHandle_, Vector4<float> color_)
{
	trans.pos = pos_;
	SetVertex(width_, height_);
	SetNormal();
	SetTexcoord();

	texHandle = texHandle_;
	color = color_;
}

void TriangleBehavior::Initialize(Vector3 local_leftPos_, Vector3 local_topPos_, Vector3 local_rightPos_,
	Vector3 pos_,int texHandle_,Vector4<float> color_ )
{
	trans.pos = pos_;
	SetVertex(local_leftPos_, local_topPos_, local_rightPos_);
	SetNormal();
	SetTexcoord();

	texHandle = texHandle_;
	color = color_;
}

void TriangleBehavior::SetTexcoord(Vector2 left_ , Vector2 top_ , Vector2 right_ )
{
	left.texcoord = left_;
	top.texcoord = top_;
	right.texcoord = right_;
}


void TriangleBehavior::SetNormal()
{
	Vector3 R2L = left.position - right.position;
	Vector3 L2T = top.position - left.position;
	Vector3 T2R = right.position - top.position;

	Vector3 crossVec;
	crossVec = R2L.GetCross(L2T);
	left.normal = { crossVec.x,crossVec.y,crossVec.z };

	crossVec = L2T.GetCross(T2R);
	top.normal = { crossVec.x,crossVec.y,crossVec.z };

	crossVec = T2R.GetCross(R2L);
	right.normal = { crossVec.x,crossVec.y,crossVec.z };
}

void TriangleBehavior::SetVertex(Vector3 local_leftPos_, Vector3 local_topPos_, Vector3 local_rightPos_)
{
	//左の頂点
	left.position = local_leftPos_;
	//上の頂点
	top.position = local_topPos_;
	//右の頂点
	right.position = local_rightPos_;
}

void TriangleBehavior::SetVertex(float width_, float height_)
{
	//重心はあとで

	//左の頂点
	left.position.x = -width_ / 2.0f;
	left.position.y = -height_ / 2.0f;
	//上の頂点
	top.position.x = 0.0f;
	top.position.y = height_ / 2.0f;
	//右の頂点
	right.position.x = width_ / 2.0f ;
	right.position.y = -height_ / 2.0f;
}
