#pragma once
#include "../../MyLib/Utilities/MouseStructures/MouseBehavior.h"
#include "../../MyLib/Utilities/CounterStructures/CounterAndTime.h"

struct InGameMouse:MouseBehavior
{
	CounterAndInvTime counter;
	float inc = 0;
	float degree = 90;

	virtual void Update(Matrix4 vpMat_, Vector3 cameraPos_) override;
	virtual void Debug() override;
	virtual void Draw(Matrix4 vpMat_) override;
	InGameMouse();
};

