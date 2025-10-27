#pragma once

#include "../../../Utilities/BaseClassStructures/VertexData/VertexData.h"
#include <string>
#include <vector>


struct MaterialData
{
	std::string textureFilePath;
};

struct ModelData
{
	std::vector<std::vector<VertexData>> verticles;
	std::vector<MaterialData> materialData;

	static std::vector<MaterialData> LoadMaterialTemplateFile(std::string const& directoryPath_, std::string const& fileName_);
	static ModelData LoadObjFile(std::string const& directoryPath_, std::string const& fileName_);

};
