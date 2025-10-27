#pragma once

#include "../BaseClassStructures/Transform/Transform.h"
#include "../BaseClassStructures/Material/Material.h"
#include "../BaseClassStructures/VertexData/VertexData.h"

extern float const inv_255;
extern float const inv_100;

Vector2 ConvertToScreen(Vector3 dst_, Matrix4& viewProjectionMat_);
