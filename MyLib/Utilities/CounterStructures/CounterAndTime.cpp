#include "CounterAndTime.h"

CounterAndInvTime::CounterAndInvTime(float performTime_)
{
	Initialize(performTime_);
}

//設定した加算カウント値をカウンターにいれる
void CounterAndInvTime::Add()
{
	if (count < 1.0f)
	{
		count += inv_Time;
	}

	else
	{
		count = 1.0f;
	}
}

//加算カウント値を設定する
void CounterAndInvTime::Initialize(float sumFrame_)
{
	inv_Time = 1.0f / sumFrame_;

}

//終了を返す関数
bool CounterAndInvTime::IsEnd()
{
	bool ret = false;

	if (count >= 1.0f)
	{
		count = 0.0f;
		ret = true;
	}

	return ret;
}


