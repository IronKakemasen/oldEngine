#pragma once
#include <windows.h>

struct FPSController
{
	const double kFrameTime = 1.0 / 65.0;
	LARGE_INTEGER mTimeStart;
	LARGE_INTEGER mTimeEnd;
	LARGE_INTEGER mTimeFreq;
	double frameTime;

	void TimeAdjust();
};
