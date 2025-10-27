#include "Sun.h"
#include "../DebugUIStructures/DebugUI.h"
#include "../../NoviceLike/NoviceLike.h"

void Sun::Initialize(Vector3 pos_, Vector4<float> color_, std::string name_, float intensity_)
{
	DebugSceneUI::GetInstance()->AddToDebugUI(name_, std::bind(&Sun::Debug, this));
	lightType = LightType::kDirectionalLight;

	name = name_;
	trans.pos = pos_;
	intensity = intensity_;
	color = color_;
}

void Sun::Update(Matrix4 vpvMat_)
{
	trans.SetWorldMatBasedQuaternion();
	trans.SetWVP_Mat(vpvMat_);
	NoviceLike::InputDirectionalLightData(trans.GetWorldPos(), color, intensity);
}

void Sun::Draw()
{
}

void Sun::Debug()
{
	ImGui::DragFloat3("Pos", reinterpret_cast<float*>(&trans.pos), 0.05f);
	ImGui::DragFloat4("Color", reinterpret_cast<float*>(&color), 0.5f);
	ImGui::DragFloat("intensity", reinterpret_cast<float*>(&intensity), 0.01f);

}


