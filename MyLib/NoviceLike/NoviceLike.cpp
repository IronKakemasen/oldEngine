#include "NoviceLike.h"
#include "../Utilities/BaseClassStructures/PrimitiveShape/PrimitiveShape.h"

void NoviceLike::DrawTriangle(VertexData left_, VertexData top_, VertexData right_,
	Vector4<float> color_, int texHandle_,TransformationMatrix tMat_, DrawMode drawMode_, Matrix4 uvMat_, int enableLighting_)
{
	pDxCommon->DrawTriangle(left_, top_, right_, color_, texHandle_,tMat_, drawMode_, uvMat_, enableLighting_);
}

void NoviceLike::DrawQuad(VertexData leftTop_, VertexData rightTop_, VertexData leftBottom_, VertexData rightBottom_,
	int texHandle_, Vector4<float> color_, TransformationMatrix tMat_, int enableLighting_,DrawMode drawMode_, Matrix4 uvMat_ )
{
	pDxCommon->DrawQuad(leftTop_, rightTop_, leftBottom_, rightBottom_, texHandle_, color_, tMat_, enableLighting_, drawMode_, uvMat_);
}

void NoviceLike::DrawLine(Vector3 start_, Vector3 end_, int texHandle_,Vector4<float> color_, TransformationMatrix tMat_, Matrix4 uvMat_ )
{
	pDxCommon->DrawLine(start_, end_, texHandle_,color_, tMat_, uvMat_);
}

void NoviceLike::DrawSphere(Vector4<float> color_, TransformationMatrix tMat_, int texHandle_, DrawMode drawMode_, Matrix4 uvMat_,int enableLighting_)
{
	pDxCommon->DrawSphere(color_, tMat_, texHandle_ , drawMode_, uvMat_, enableLighting_);
}

void NoviceLike::DrawCube(VertexData* leftTop_, VertexData* rightTop_, VertexData* leftBottom_, VertexData* rightBottom_,
	int* texHandle_, TransformationMatrix* tMat_, Vector4<float> color_, int enableLighting_,DrawMode drawMode_, Matrix4* uvMat_)
{
	pDxCommon->DrawCube(leftTop_, rightTop_, leftBottom_, rightBottom_,texHandle_, tMat_, color_, enableLighting_, drawMode_, uvMat_);
}

void NoviceLike::DrawModel(Mesh* mesh_, UINT verticesSIze_,int texHandle_ ,Vector4<float> color_, TransformationMatrix tMat_, int enableLighting_, Matrix4 uvMat_)
{
	pDxCommon->DrawModel(mesh_, verticesSIze_, texHandle_,color_, tMat_,enableLighting_, uvMat_);
}

void NoviceLike::DrawTetrahedra(VertexData* left_, VertexData* top_, VertexData* right_,
	Vector4<float> color_, int texHandle_, TransformationMatrix* tMat_, DrawMode drawMode_, Matrix4* uvMat_)
{
	pDxCommon->DrawTetrahedra(left_, top_, right_, color_, texHandle_, tMat_, drawMode_, uvMat_);
}

void NoviceLike::DrawAxis(Vector3 pos_, Vector3* targetDir_, float coe_ , Matrix4 vpMat_)
{
	float len = 1.0f * coe_;
	float len2 = len * 2.0f;
	TransformationMatrix constantBuffer;
	constantBuffer.WVP = constantBuffer.WVP.Multiply(vpMat_);
	Matrix4 ConstantUVMat;

	Vector3 xAxis[2] = { {pos_.x - len*0.5f,pos_.y,pos_.z},{pos_.x + len,pos_.y,pos_.z} };
	Vector3 yAxis[2] = { {pos_.x ,pos_.y - len * 0.5f,pos_.z},{pos_.x ,pos_.y + len,pos_.z} };
	Vector3 zAxis[2] = { {pos_.x ,pos_.y,pos_.z - len * 0.5f},{pos_.x ,pos_.y,pos_.z + len} };

	DrawLine(xAxis[0], xAxis[1], 0, { 0,255,0,255 }, constantBuffer,ConstantUVMat);
	DrawLine(yAxis[0], yAxis[1], 0, { 0,0,255,255 }, constantBuffer,ConstantUVMat);
	DrawLine(zAxis[0], zAxis[1], 0, { 255,0,0,255 }, constantBuffer,ConstantUVMat);

	if (targetDir_)
	{
		Vector3 targetAxis[2] = { {pos_.x ,pos_.y,pos_.z },
		{pos_.x + targetDir_->x * len2  ,pos_.y + targetDir_->y * len2 ,pos_.z + targetDir_->z * len2 } };
		DrawLine(targetAxis[0], targetAxis[1], 1, { 255.0f,255,255,255 }, constantBuffer, ConstantUVMat);
	}
}


uint16_t NoviceLike::LoadTextureFile(std::string filePath_)
{
	return pDxCommon->LoadTextureFile(filePath_);
}

//さすがに書き方がヤヴァイ兵長
Model NoviceLike::CreateModel(std::string const& directoryPath_obj_, std::string const& fileName_obj_)
{
	Model ret_model;

	//モデルデータ読み込み
	ret_model.modelData = ModelData::LoadObjFile(directoryPath_obj_, fileName_obj_);
	//メッシュ作成
	for(auto itr = ret_model.modelData.verticles.begin(); itr != ret_model.modelData.verticles.end();++itr)
	{
		ret_model.mesh.emplace_back(pDxCommon->CreateMesh(static_cast<UINT>((*itr).size()), 0, 1));
	}

	//頂点データのみデータ転送
	auto itr_vertexMap = ret_model.modelData.verticles.begin();
	for (auto itr = ret_model.mesh.begin(); itr != ret_model.mesh.end(); ++itr, ++itr_vertexMap)
	{
		std::memcpy((*itr)->vertexMap, (*itr_vertexMap).data(), sizeof(VertexData) * (*itr_vertexMap).size());
	}

	//テクスチャを設定
	for (auto itr_mtData = ret_model.modelData.materialData.begin(); itr_mtData != ret_model.modelData.materialData.end(); ++itr_mtData)
	{
		ret_model.textureHandle.emplace_back(pDxCommon->LoadTextureFile((*itr_mtData).textureFilePath));
	}

	return ret_model;
}

std::unique_ptr<Model> NoviceLike::CreateModelMusic(std::string const& directoryPath_obj_, std::string const& fileName_obj_)
{
	std::unique_ptr<Model> ret_model = std::make_unique<Model>();;

	//モデルデータ読み込み
	ret_model->modelData = ModelData::LoadObjFile(directoryPath_obj_, fileName_obj_);
	//メッシュ作成
	for (auto itr = ret_model->modelData.verticles.begin(); itr != ret_model->modelData.verticles.end(); ++itr)
	{
		ret_model->mesh.emplace_back(pDxCommon->CreateMesh(static_cast<UINT>((*itr).size()), 0, 1));
	}

	//頂点データのみデータ転送
	auto itr_vertexMap = ret_model->modelData.verticles.begin();
	for (auto itr = ret_model->mesh.begin(); itr != ret_model->mesh.end(); ++itr, ++itr_vertexMap)
	{
		std::memcpy((*itr)->vertexMap, (*itr_vertexMap).data(), sizeof(VertexData) * (*itr_vertexMap).size());
	}

	//テクスチャを設定
	for (auto itr_mtData = ret_model->modelData.materialData.begin(); itr_mtData != ret_model->modelData.materialData.end(); ++itr_mtData)
	{
		ret_model->textureHandle.emplace_back(pDxCommon->LoadTextureFile((*itr_mtData).textureFilePath));
	}

	return std::move(ret_model);

}


void NoviceLike::InputDirectionalLightData(Vector3 worldPos_, Vector4<float> color_, float intensity_)
{
	pDxCommon->InputDirectionalLightData(worldPos_, color_, intensity_);
}

void NoviceLike::SetMousePos(POINT* point_)
{
	pDxCommon->SetMousePos(point_);
}

void NoviceLike::SetBlendMode(BlendMode mode_)
{
	pDxCommon->SetBlendMode(mode_);
}

void NoviceLike::SetCullMode(CullMode cullMode_)
{
	pDxCommon->SetCullMode(cullMode_);
}
void NoviceLike::SetShaderSet(ShaderSetType shaderSetType_)
{
	pDxCommon->SetShaderSet(shaderSetType_);
}

void NoviceLike::SetCameraPos(Vector3 cameraWorld_)
{
	pDxCommon->SetCameraPos(cameraWorld_);
}


uint16_t NoviceLike::CreateOS_Set()
{
	return pDxCommon->CreateOS_Set();
}

void NoviceLike::ChangeWindowColor(Vector4<float> col_)
{
	pDxCommon->ChangeWindowColor(col_);
}


void NoviceLike::BeginFrame(BYTE* key, DIMOUSESTATE* mouseState_)
{
	QueryPerformanceFrequency(&pDxCommon->fpsController.mTimeFreq);
	QueryPerformanceCounter(&pDxCommon->fpsController.mTimeStart);

	memcpy(NoviceLike::preKeys, NoviceLike::keys, 256);
	memcpy(NoviceLike::preMouseState.rgbButtons, NoviceLike::mouseState.rgbButtons, 4);
	NoviceLike::preMousePos = NoviceLike::mousePos;

	pDxCommon->BeginFrame(key, mouseState_);
}

void NoviceLike::EndFrame()
{
	pDxCommon->EndFrame();
}

void NoviceLike::Initialize(int32_t kWindowWidth_, int32_t kWindowHeight_, LPCWSTR const windowTitle_)
{
	pDxCommon->Initialize(kWindowWidth_, kWindowHeight_, windowTitle_);
}

void NoviceLike::Finalize()
{
	pDxCommon->Finalize();
}

//キー入力トリガーを検知( 引数「DIK_」)
bool NoviceLike::IsKeyTrigger(int key_)
{
	bool ret = false;

	if (keys[key_] && !preKeys[key_]) ret = true;

	return ret;
}

//キー入力プレスを検知( 引数「DIK_」)
bool NoviceLike::IsKeyPress(int key_)
{
	bool ret = false;

	if (keys[key_]) ret = true;

	return ret;

}

