#include "SimpleDebugCamera.h"
#include "../DebugUIStructures/DebugUI.h"
#include "../../NoviceLike/NoviceLike.h"

SimpleDebugCamera::SimpleDebugCamera()
{
	Initialize();
	DebugSceneUI::GetInstance()->AddToDebugUI("SimpleDebugCamera", std::bind(&SimpleDebugCamera::Debug, this));

}

void SimpleDebugCamera::Update()
{
	float const speed = 0.05f;

	if (NoviceLike::keys[DIK_W]) trans.pos.y += speed;
	if (NoviceLike::keys[DIK_S]) trans.pos.y -= speed;

	if (NoviceLike::keys[DIK_A])trans.pos.x -= speed;
	if (NoviceLike::keys[DIK_D])trans.pos.x += speed;

	if (NoviceLike::keys[DIK_O])trans.pos.z += speed;
	if (NoviceLike::keys[DIK_K])trans.pos.z -= speed;


	trans.SetWorldMatBasedQuaternion();
	viewProjectionMat = CreateViewProjectionMatrix();
}

void SimpleDebugCamera::Initialize()
{
	trans.pos = { 0.0f,5.0f,-3.0f };
	trans.defaultQuatenion.x_axis.deltaTheta = 20.0f;
}

void SimpleDebugCamera::Debug()
{
	static bool DoClear = false;
	ImGui::Checkbox("DebugMode[Shift_Z]", &switcher);
	ImGui::Text("[W,A,S,D]:Move [O,K]:MoveForwardBack");
	ImGui::DragFloat3("Pos", reinterpret_cast<float*>(&trans.pos), 0.125f);
	ImGui::DragFloat("X_axis", &trans.defaultQuatenion.x_axis.deltaTheta, 0.25f);
	ImGui::DragFloat("Y_axis", &trans.defaultQuatenion.y_axis.deltaTheta, 0.25f);
	ImGui::DragFloat("Z_axis", &trans.defaultQuatenion.z_axis.deltaTheta, 0.25f);
	ImGui::Checkbox("clear", &DoClear);
	if (DoClear)
	{
		Initialize();
		DoClear = false;
	}
}

