#pragma once
#include "CounterAndTime.h"
#include <functional>

struct TimedCall
{
	std::function<void()> funcObj;
	CounterAndInvTime counterAndTime;

	void Update();

	TimedCall(float performTime_, std::function<void()> funcObj_);
};

