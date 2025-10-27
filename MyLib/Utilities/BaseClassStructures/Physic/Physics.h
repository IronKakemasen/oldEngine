#pragma once
#include "../VectorAndMatrix/VectorAndMatrix.h"

struct Physics3D
{
	static constexpr float kGravityScale = 0.0025f;
	Vector3 velocity;

	//重力を加える
	void AddGravity();
	//力を加える
	void AddForce(Vector3 power_, float mass_ = 1.0f);
	//速度を制限する
	void AdjustVelocity(Vector3 limimt_);

};


