#include "Segment.h"
#include "../../EasingFuctionStructures/EasingFunctions.h"

Vector3 Segment3D::GetDiff()
{
	return e - s;
}

Vector3 Segment3D::GetPoint(float rate_)
{
	return Easing::Lerp(s, e, rate_);
}

void Segment3D::Set(Vector3 st_, Vector3 ed_)
{
	s = st_;
	e = ed_;
}
Segment3D::Segment3D(Vector3 st_, Vector3 ed_)
{
	Set(st_, ed_);
}

Vector2 Segment2D::GetDiff()
{
	return e - s;
}

Vector2 Segment2D::GetPoint(float rate_)
{
	return Easing::Lerp(s, e, rate_);
}

void Segment2D::Set(Vector2 st_, Vector2 ed_)
{
	s = st_;
	e = ed_;
}
Segment2D::Segment2D(Vector2 st_, Vector2 ed_)
{
	Set(st_, ed_);
}

