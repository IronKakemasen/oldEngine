#include "EssentialInstanceIncludes.h"

void EssentialInstanceInitialize()
{
	//[各種リソース]
	ResourceInstances::Initilalize();
	//[ライト]
	LightInstances::Initialize();
	//[デバッグシーン]
	DebugSceneAssets::Initialize(LightInstances::sun.get());

}