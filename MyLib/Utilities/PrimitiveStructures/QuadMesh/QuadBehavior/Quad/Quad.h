#pragma once
#include "../QuadBehavior.h"

struct Quad :QuadBehavior
{
	virtual void Update(Matrix4 vpMat_ = 0)override;
	virtual void Draw(int enableLighting_ = true)override;
};

