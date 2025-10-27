#pragma once
#include "TransformDebugMouse.h"

namespace DebugMouseInstances
{
	extern std::unique_ptr<TransformDebugMouse> debugTransMouse;
	extern MouseBehavior* debugMouseBehavior;


	void Instantiate();
	void Initialize();
}
