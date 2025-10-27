#include "FPSController.h"

void FPSController::TimeAdjust()
{
	//今の時間を取得
	QueryPerformanceCounter(&mTimeEnd);
	//(今の時間 - 前フレームの時間) / 周波数 = 経過時間
	double frameTime = (double)(mTimeEnd.QuadPart - mTimeStart.QuadPart) / (double)mTimeFreq.QuadPart;
	//処理時間に余裕がある場合はその分待つ
	if (frameTime < kFrameTime)
	{
		//sleepの時間を計測
		DWORD sleepTime = (DWORD)((kFrameTime - frameTime) * 1000.0);
		timeBeginPeriod(1);
		//寝る
		Sleep(sleepTime);
		timeEndPeriod(1);
	}


}
