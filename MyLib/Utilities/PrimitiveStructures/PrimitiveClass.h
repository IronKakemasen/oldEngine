#pragma once
#include "../../NoviceLike/NoviceLike.h"
#include "../BaseClassStructures/PrimitiveShape/PrimitiveShape.h"
#include "../ResourceStructures/MapTextures.h"

struct PrimitiveClassBehavior
{
	Transform4 trans;
	Vector4<float> color;
	DrawMode drawMode = DrawMode::kSolid;
	int texHandle = 0;

	PrimitiveClassBehavior();
};


