#pragma once
#include "../ResourceStructures/MapModels.h"


struct ModelObject
{
	Transform4 trans;
	Vector4<float> color = {255,255,255,255};
	std::unique_ptr<Model> model;

	ModelObject(Vector3 pos_, Vector4 <float>color_ = { 255,255,255,255 });
	ModelObject();

	void Update(Matrix4 vpMat_);
	void Draw(int enableLighting_ = 1);
	void Initialize(Vector3 pos_, std::unique_ptr<Model> model_, Vector4 <float>color_ = { 255,255,255,255 });
};

