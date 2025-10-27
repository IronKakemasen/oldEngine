#pragma once
#include"../../PrimitiveClass.h"

struct UnitSphere :PrimitiveClassBehavior
{
	static inline int count = -1;

	UnitSphere();
	void Update(Matrix4 vpMat_);
	void Draw(int enableLighting = true);
	void Initialize(Vector3 pos_, Vector3 scale_, TagTexture tag_, Vector4<float> color_ = { 255,255,255,255 });

};

