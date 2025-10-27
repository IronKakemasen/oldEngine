#include "BenriFunctions.h"

float GetRadian(float rotate_)
{
	static float const inv180 = 1.0f / 180.0f;
	return rotate_ * 3.1415927f * inv180;
}
