#include "DebugCameraInstances.h"
#include "../../NoviceLike/NoviceLike.h"

CameraBehavior* DebugCameraInstances::debugCameraBehavior = nullptr;
std::unique_ptr<SphericalDebugCamera> DebugCameraInstances::sphericalDebugCamera;
std::unique_ptr<SimpleDebugCamera> DebugCameraInstances::simpleDebugCamera;

void DebugCameraInstances::Instantiate()
{
	DebugCameraInstances::sphericalDebugCamera = std::make_unique<SphericalDebugCamera>();
	DebugCameraInstances::simpleDebugCamera = std::make_unique<SimpleDebugCamera>();
}

void DebugCameraInstances::SetStartCamera(CameraBehavior* camera_)
{
	camera_ = DebugCameraInstances::sphericalDebugCamera.get();
}

CameraBehavior* DebugCameraInstances::ChangeCamera(CameraBehavior* camera_, CameraBehavior* debug_)
{
	if (NoviceLike::keys[DIK_LSHIFT] && NoviceLike::keys[DIK_Z] && !NoviceLike::preKeys[DIK_Z])
	{
		CameraBehavior::switcher = !CameraBehavior::switcher;
	}

	CameraBehavior* cameras[2]
	{
		camera_,
		debug_
	};

	return cameras[CameraBehavior::switcher];
}

void DebugCameraInstances::Initialize()
{
	Instantiate();
	//camera_ = DebugCameraAssets::sphericalDebugCamera.get();

	//SetStartCamera(camera_);
}