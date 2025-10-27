#include "Physics.h"
#include <algorithm>

void Physics3D::AddGravity()
{
	velocity.y += -kGravityScale;
}

void Physics3D::AdjustVelocity(Vector3 limimt_)
{
	//速度補正
	velocity.x = std::clamp(velocity.x, -limimt_.x, limimt_.x);
	velocity.y = std::clamp(velocity.y, -limimt_.y, limimt_.y);
	velocity.z = std::clamp(velocity.z, -limimt_.z, limimt_.z);

}

void Physics3D::AddForce(Vector3 power_, float mass_ )
{
	velocity = velocity + (power_ * mass_);
}


