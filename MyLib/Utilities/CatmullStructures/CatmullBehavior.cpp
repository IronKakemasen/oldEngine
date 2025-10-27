#include "CatmullBehavior.h"

Vector3 CatmullBehavior::GetComputedNum(size_t index_, float t_)
{
	//p0からp3までの制御点を取得
	Vector3 p0 = controllPoints[index_ - 1].get()->trans.pos;
	Vector3 p1 = controllPoints[index_].get()->trans.pos;
	Vector3 p2 = controllPoints[index_ + 1].get()->trans.pos;
	Vector3 p3 = controllPoints[index_ + 2].get()->trans.pos;

	return 	p1 +
		(p0 * (-tension) + p2 * tension) * t_ +
		(p0 * tension * 2.0f + p1 * (tension - 3.0f) + p2 * (3.0f - 2.0f * tension) + p3 * (-tension)) * t_ * t_ +
		(p0 * (-tension) + p1 * (2.0f - tension) + p2 * (tension - 2.0f) + p3 * tension) * t_ * t_ * t_;

}

void CatmullBehavior::Set(float dt_, float numCorners_,float radius_, Vector3 setPos_)
{
	radius = radius_;
	dt = dt_;
	numCorners = numCorners_;
	kNumModel = (1.0f / dt) * (numCorners );
	trans.pos = setPos_;

}
