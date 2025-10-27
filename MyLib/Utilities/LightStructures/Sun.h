#pragma once

#include "LightBehavior.h"
#include "../../EngineCore/Light/DirectionalLight.h"


class Sun :public DirectionalLight,LightBehavior
{

public:
	virtual void Update(Matrix4 vpvMat_)override;
	virtual void Draw()override;
	virtual void Debug()override;
	virtual void Initialize(Vector3 pos_, Vector4<float> color_, std::string name_,float intensity_ = 1.0f)override;
};
