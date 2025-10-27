#pragma once
#include "../VectorAndMatrix/VectorAndMatrix.h"

struct Segment3D
{
	//StartPoint
	Vector3 s;
	//EndPoint
	Vector3 e;

	Vector3 GetDiff();
	Vector3 GetPoint(float rate_);
	void Set(Vector3 st_, Vector3 ed_);
	Segment3D(Vector3 st_, Vector3 ed_);
	Segment3D()
	{
	};
};

struct Segment2D
{
	//StartPoint
	Vector2 s;
	//EndPoint
	Vector2 e;

	Vector2 GetDiff();
	Vector2 GetPoint(float rate_);
	void Set(Vector2 st_, Vector2 ed_);
	Segment2D(Vector2 st_, Vector2 ed_);
	Segment2D()
	{
	}
};
