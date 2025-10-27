#include "SphericalDebugCamera.h"
#include "../DebugUIStructures/DebugUI.h"
#include "../BenriFunctionStructures/BenriFunctions.h"


void SphericalDebugCamera::Update()
{
	float const speed = 2.0f;

	if (NoviceLike::keys[DIK_A]) sphericalCoordinates.y += speed;
	if (NoviceLike::keys[DIK_D]) sphericalCoordinates.y -= speed;

	if (NoviceLike::keys[DIK_W])sphericalCoordinates.z += speed;
	if (NoviceLike::keys[DIK_S])sphericalCoordinates.z -= speed;

	if (NoviceLike::keys[DIK_O])sphericalCoordinates.x -= speed * 0.025f;
	if (NoviceLike::keys[DIK_K])sphericalCoordinates.x += speed * 0.025f;


	sphericalCoordinates.z = std::clamp(sphericalCoordinates.z, -179.5f, -0.5f);

	Vector3 buff = { sphericalCoordinates.x,GetRadian(sphericalCoordinates.y),
	GetRadian(sphericalCoordinates.z) };
	trans.pos = ConvertToRectangularCoordinate(buff);
	trans.targetDir = centerPos - trans.pos;

	trans.SetWorldMatBasedQuaternion();
	viewProjectionMat = CreateViewProjectionMatrix();
}

void SphericalDebugCamera::Initialize()
{
	sphericalCoordinates = { 7.5f,180.0f,-45.0f };
}

void SphericalDebugCamera::Debug()
{
	static bool DoClear = false;

	ImGui::Checkbox("DebugMode[Shift_Z]", &switcher);
	ImGui::Text("[ W , A , S , D ]:Move [ O , K ]:MoveForwardBack");
	ImGui::DragFloat3("pos", reinterpret_cast<float*>(&trans.pos), 0.125f);
	ImGui::DragFloat3("targetDir", reinterpret_cast<float*>(&trans.targetDir), 0.0125f);

	ImGui::Checkbox("clear", &DoClear);
	if (DoClear)
	{
		Initialize();
		DoClear = false;
	}

}


SphericalDebugCamera::SphericalDebugCamera()
{
	centerPos = { 0.0f,0.0f,0.0f };
	trans.targetDir = centerPos;

	Initialize();
	DebugSceneUI::GetInstance()->AddToDebugUI("SphericalDebugCamera", std::bind(&SphericalDebugCamera::Debug, this));

}
