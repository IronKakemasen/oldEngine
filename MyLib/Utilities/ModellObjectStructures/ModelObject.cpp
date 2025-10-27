#include "ModelObject.h"

ModelObject::ModelObject()
{
}

ModelObject::ModelObject(Vector3 pos_, Vector4 <float>color_ )
{
	trans.pos = pos_;
	color = color_;
}

void ModelObject::Update(Matrix4 vpMat_)
{
	trans.SetWorldMatBasedQuaternion();
	trans.SetUVMat();
	trans.SetWVP_Mat(vpMat_);
}

void ModelObject::Draw(int enableLighting_ )
{
	auto itr_vertexMap = model->modelData.verticles.begin();
	auto itr_texHandle = model->textureHandle.begin();

	for (auto itr = model->mesh.begin(); itr != model->mesh.end(); ++itr, ++itr_vertexMap)
	{

		NoviceLike::DrawModel((*itr).get(), static_cast<UINT>((*itr_vertexMap).size()),
			(*itr_texHandle), color, trans.mat, enableLighting_, trans.uvMat);

		auto copy = itr_texHandle;
		copy++;

		if (copy != model->textureHandle.end())
		{
			itr_texHandle++;
		}

	}

}

void ModelObject::Initialize(Vector3 pos_, std::unique_ptr<Model> model_, Vector4 <float>color_)
{
	trans.pos = pos_;
	color = color_;
	model = std::move(model_);
	if (model->textureHandle.size() == 0)
	{
		model->textureHandle.emplace_back(0);
	}

}
