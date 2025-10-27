#pragma once

#include "../VectorAndMatrix/VectorAndMatrix.h"
#include <algorithm>


struct Quaternion
{
	Vector3 axis = { 0,0,1.0f };
	float deltaTheta = 0.0f;

	Quaternion();
	Quaternion(Vector3 axis_);
	Vector4<float> Get();


};

struct QuaternionDefaultSet
{
	Quaternion x_axis;
	Quaternion y_axis;
	Quaternion z_axis;

	QuaternionDefaultSet();

	Vector4<float> GetComposite();
};


//==================================================
//https://edom18.hateblo.jp/entry/2018/04/18/104054
//==================================================
Vector4<float> ConvertToQuaternion(Matrix4 m);
Vector4<float> LookAt(Vector3 target_, Vector3 pos_);
//==================================================
//==================================================\
