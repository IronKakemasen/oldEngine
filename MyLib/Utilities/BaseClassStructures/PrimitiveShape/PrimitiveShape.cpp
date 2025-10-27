#include "PrimitiveShape.h"


void Rect::SetRectShape(Vector3 centerPos_, float width_, float height_)
{
	left = -width_ * 0.5f + centerPos_.x;
	right = width_ * 0.5f + centerPos_.x;
	bottom = -height_ * 0.5f + centerPos_.y;
	top = height_ * 0.5f + centerPos_.y;
}

void Rect3D::SetCube(Vector3 centerPos_, float width_, float height_, float depth_)
{
	left = -width_ * 0.5f + centerPos_.x;
	right = width_ * 0.5f + centerPos_.x;
	bottom = -height_ * 0.5f + centerPos_.y;
	top = height_ * 0.5f + centerPos_.y;
	front = -depth_ * 0.5f + centerPos_.z;
	back = depth_ * 0.5f + centerPos_.z;

}


void RectShape::SetChangedCornerPositions(std::array<Vector3, Corner::kNumCorner>* dst_changedCornerPositions_,
	Vector3 centerPos_, Vector3 delta_pos_)
{
	for (uint32_t i = 0; i < dst_changedCornerPositions_->size(); ++i)
	{
		dst_changedCornerPositions_->at(i) = GetCornerPos(static_cast<Corner>(i), centerPos_ + delta_pos_);
	}
}

Vector3 RectShape::GetCornerPos(Corner corner_, Vector3 centerPos_)
{
	Vector3 ret_corner[kNumCorner] =
	{
		{-width / 2.0f + centerPos_.x,height / 2.0f + centerPos_.y,1.0f},	//LT
		{width / 2.0f + centerPos_.x,height / 2.0f + centerPos_.y,1.0f},	//RT
		{width / 2.0f + centerPos_.x,-height / 2.0f + centerPos_.y,1.0f},	//RB
		{-width / 2.0f + centerPos_.x,-height / 2.0f + centerPos_.y,1.0f},	//LB
	};

	return ret_corner[corner_];
}

void CubeShape::Set(float width_, float height_, float depth_)
{
	width = width_;;
	height = height_;
	depth = depth_;

}
