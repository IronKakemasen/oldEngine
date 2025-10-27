#pragma once

#include "SphericalDebugCamera.h"
#include "SimpleDebugCamera.h"
#include <memory>

namespace DebugCameraInstances
{
	extern CameraBehavior* debugCameraBehavior;
	extern std::unique_ptr<SphericalDebugCamera> sphericalDebugCamera;
	extern std::unique_ptr<SimpleDebugCamera> simpleDebugCamera;

	void SetStartCamera(CameraBehavior* camera_);
	void Initialize();
	void Instantiate();
	CameraBehavior* ChangeCamera(CameraBehavior* camera_, CameraBehavior* debug_);

}

