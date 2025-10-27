#pragma once
#include "../VectorAndMatrix/VectorAndMatrix.h"
#include <array>

//上面 = 0 , 底面 = 1 , 側面(左) = 2 , 側面(右) = 3 , 側面(奥) = 4 , 側面(手前) = 5
enum class CubeSurface
{
	kTop,
	kBottom,
	kLeft,
	kRight,
	kBeyond,
	kFront
};

enum Corner
{
	LT,
	RT,
	RB,
	LB,

	kNumCorner
};

struct Rect
{
	float left;
	float top;
	float bottom;
	float right;

	void SetRectShape(Vector3 centerPos_, float width_, float height_);
};

struct Rect3D
{
	float left;
	float top;
	float bottom;
	float right;
	float front;
	float back;

	void SetCube(Vector3 centerPos_, float width_, float height_, float depth_);
};


struct RectShape
{
	float width;
	float height;

	//移動量加算後の、4つ角の座標をセットする
	void SetChangedCornerPositions(std::array<Vector3, Corner::kNumCorner>* dst_changedCornerPositions_,
		Vector3 centerPos_, Vector3 delta_pos_);
	//頂点ゲット
	Vector3 GetCornerPos(Corner corner_, Vector3 centerPos_);
};

struct CubeShape
{
	float width = 0.0f;
	float height = 0.0f;
	float depth = 0.0f;

	void Set(float width_, float height_, float depth_);
};

struct CircleShape
{
	float radius = 0.0f;
};
