#pragma once
#include "Sun.h"
#include <memory>


namespace LightInstances
{
	extern std::unique_ptr<Sun> sun;

	void Initialize();
}