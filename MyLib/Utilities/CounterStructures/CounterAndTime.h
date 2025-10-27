#pragma once

struct CounterAndInvTime
{
	float count = 0.0f;
	float inv_Time;

	CounterAndInvTime(float performTime_);
	CounterAndInvTime(){}
	void Add();
	void Initialize(float sumFrame_);
	bool IsEnd();
};
