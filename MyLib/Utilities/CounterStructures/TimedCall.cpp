#include "TimedCall.h"

void TimedCall::Update()
{
	counterAndTime.Add();

	if (counterAndTime.count >= 1.0f)
	{
		counterAndTime.count = 0.0f;
		funcObj();
	}
}

TimedCall::TimedCall(float performTime_, std::function<void()> funcObj_)
{
	funcObj = funcObj_;
	counterAndTime.Initialize(performTime_);
}
