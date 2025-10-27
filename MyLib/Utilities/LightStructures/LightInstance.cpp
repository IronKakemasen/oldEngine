#include "LightInstance.h"


std::unique_ptr<Sun> LightInstances::sun(new Sun);

void LightInstances::Initialize()
{
	LightInstances::sun->Initialize({ -30,6.2f,4.5f }, { 255,255,255,255 }, "Sun");
}
