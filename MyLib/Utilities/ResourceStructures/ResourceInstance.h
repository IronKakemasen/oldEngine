#pragma once

#include "MapTextures.h"
#include "MapModels.h"

namespace ResourceInstances
{
	extern std::unique_ptr<MapModels> mapModels;
	extern std::unique_ptr < MapTextures> mapTextures;


	void Instantiate();
	void Initilalize();
}
