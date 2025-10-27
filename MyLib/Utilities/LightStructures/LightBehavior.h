#pragma once
#include "../BaseClassStructures/Transform/Transform.h"
#include <string>

enum LightType
{
	kDirectionalLight,


	kCount
};

struct LightBehavior
{
	LightType lightType;
	std::string name = "";
	Transform4 trans;

	virtual void Update(Matrix4 vpvMat_) = 0;
	virtual void Draw() = 0;
	virtual void Debug() = 0;
	virtual void Initialize(Vector3 pos_, Vector4<float> color_, std::string name_, float intensity_ = 1.0f) = 0;
};

