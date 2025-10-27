#include "DebugSceneAssets.h"
#include "../../LightStructures/Sun.h"
#include "../../CameraStructures/DebugCameraInstances.h"
#include "../../MouseStructures/DebugMouseInstance.h"
#include "../../ResourceStructures/ResourceInstance.h"

std::unique_ptr<DebugScene> DebugSceneAssets::debugScene;
SceneBehavior* DebugSceneAssets::debugSceneBehavior = nullptr;

void DebugSceneAssets::Initialize(Sun* sun_)
{
	Instantiate();
	DebugCameraInstances::Initialize();
	DebugMouseInstances::Initialize();

	//debugScene
	DebugSceneAssets::debugScene->Set(DebugCameraInstances::simpleDebugCamera.get(), DebugCameraInstances::sphericalDebugCamera.get(),
		DebugMouseInstances::debugTransMouse.get(), sun_, DebugCameraInstances::sphericalDebugCamera.get(),
		DebugMouseInstances::debugTransMouse.get(),ResourceInstances::mapModels.get());
	DebugSceneAssets::debugScene->Initialize();

	//currentDebugScene
	DebugSceneAssets::debugSceneBehavior = DebugSceneAssets::debugScene.get();

}

void DebugSceneAssets::Instantiate()
{
	DebugSceneAssets::debugScene = std::make_unique<DebugScene>();

}