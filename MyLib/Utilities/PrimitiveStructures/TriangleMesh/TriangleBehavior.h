#pragma once
#include "../PrimitiveClass.h"

struct TriangleBehavior :PrimitiveClassBehavior
{
	VertexData left;
	VertexData top;
	VertexData right;
	static inline int count = -1;
	static inline Vector2 const kCentroid_equilateraltriangle = { 0.5f,0.70710678118f };


	TriangleBehavior();

	virtual void Update(Matrix4 vpMat_) = 0;
	virtual void Draw(int enableLighting = true) = 0;

	void Initialize(float width_, float height_, Vector3 pos_, int texHandle_, Vector4<float> color_ = { 255,255,255,255 });
	void Initialize(Vector3 local_leftPos_, Vector3 local_topPos_, Vector3 local_rightPos_,
		Vector3 pos_, int texHandle_, Vector4<float> color_ = { 255,255,255,255 });

	void SetVertex(float width_, float height_);
	void SetVertex(Vector3 local_leftPos_, Vector3 local_topPos_, Vector3 local_rightPos_);
	void SetNormal();
	void SetTexcoord(
		Vector2 left_ = { 0.0f - kCentroid_equilateraltriangle.u,1.0f - kCentroid_equilateraltriangle.v },
		Vector2 top_ = { 0.5f - kCentroid_equilateraltriangle.u,0.0f - kCentroid_equilateraltriangle.v },
		Vector2 right_ = { 1.0f - kCentroid_equilateraltriangle.u,1.0f - kCentroid_equilateraltriangle.v });

};


