#pragma once
#include "DebugScene.h"

class Sun;

namespace DebugSceneAssets
{
	extern std::unique_ptr<DebugScene> debugScene;
	extern SceneBehavior* debugSceneBehavior;

	void Instantiate();
	void Initialize(Sun* sun_);
}