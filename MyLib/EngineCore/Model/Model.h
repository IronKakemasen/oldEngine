#pragma once

#include "./ModelData/ModelData.h"
#include "../Mesh/Mesh.h"
#include <memory>

struct Model
{
	ModelData modelData;
	std::vector<int> textureHandle;
	std::vector<std::unique_ptr<Mesh>> mesh;

};
