#pragma once
#include "../TriangleBehavior.h"

struct Triangle :TriangleBehavior
{
	virtual void Update(Matrix4 vpMat_)override;
	virtual void Draw(int enableLighting = true)override;
};

