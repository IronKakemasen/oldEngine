#include "DebugMouseInstance.h"


std::unique_ptr<TransformDebugMouse> DebugMouseInstances::debugTransMouse;

void DebugMouseInstances::Instantiate()
{
	DebugMouseInstances::debugTransMouse = std::make_unique<TransformDebugMouse>();
}

void DebugMouseInstances::Initialize()
{
	Instantiate();
	//mouseBehavior_ = MouseAssets::debugTransMouse.get();
}
