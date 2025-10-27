#pragma once
#include "CatmullBehavior.h"

struct CatmullSample :CatmullBehavior
{
	Vector3 tmpCoe = { 150.0f,75.0f,50.0f };
	float const inv_ = 1.0f / 180.0f;
	float t = 0.0f;


	virtual void Update(Matrix4 vpMat_)override;
	virtual void Draw()override;
	virtual void Initialize()override;
	virtual void ControllPoints(Vector3& edPoint, float rate, int No)override;


	CatmullSample();
};

