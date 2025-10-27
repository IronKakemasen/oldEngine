#include "ResourceInstance.h"

std::unique_ptr <MapModels> ResourceInstances::mapModels;
std::unique_ptr < MapTextures> ResourceInstances::mapTextures;

void ResourceInstances::Instantiate()
{
	ResourceInstances::mapTextures = std::make_unique<MapTextures>();
	ResourceInstances::mapModels = std::make_unique<MapModels>();

}

void ResourceInstances::Initilalize()
{
	Instantiate();
}


