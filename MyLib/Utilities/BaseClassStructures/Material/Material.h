#pragma once
#include "../VectorAndMatrix/VectorAndMatrix.h"


struct Material
{
	Vector4<float> color;
	int32_t enableLighting;
	float pad_enableLighting[3];
	Matrix4 uvTransform;
	float shininess = 0.5f;
	float pad_shininess[3];
};

struct CommonVariables
{
	float time = 0.0f;
	float pad_time[3];
	Vector3 camera_world;
	float pad_cameraW;
};