#include "DebugScene.h"
#include "../../LightStructures/Sun.h"
#include "../../MouseStructures/TransformDebugMouse.h"
#include "../../CameraStructures/DebugCameraInstances.h"
#include "../../DebugUIStructures/DebugUI.h"
#include "../../ResourceStructures/MapModels.h"
#include "../../GlobalVariablesStructures/GlobalVariables.h"


CameraBehavior* DebugScene::GetCameraBehavior()
{
	return cameraBehavior;
}

MouseBehavior* DebugScene::GetMouseBehavior()
{
	return mouseBehavior;
}

DebugScene::DebugScene()
{

}


void DebugScene::Update()
{
	NoviceLike::ChangeWindowColor({ 0.1f,0.1f,0.1f,0.5f });
	//カメラ
	cameraBehavior->Update();
	cameraBehavior->SendCameraInfomation();
	//マウス
	mouseBehavior->Update(cameraBehavior->viewProjectionMat, cameraBehavior->GetTrans()->GetWorldPos());

	//太陽
	sun->Update(cameraBehavior->viewProjectionMat);


	//カメラチェンジ
	cameraBehavior = DebugCameraInstances::ChangeCamera(simpleDebugCamera,sphericalDebugCamera);

}

//[ 描画処理 ]================================================================================================================================================
void DebugScene::Render()
{
	DrawGrid(DebugCameraInstances::debugCameraBehavior->viewProjectionMat);

	mouseBehavior->Draw(DebugCameraInstances::debugCameraBehavior->viewProjectionMat);
}

void DebugScene::Initialize()
{
	NoviceLike::ChangeWindowColor({ 0.1f,0.1f,0.1f,1.0f });

}

void DebugScene::Set(SimpleDebugCamera* simpleDebugCamera_, SphericalDebugCamera* sphericalDebugCamera_,
	TransformDebugMouse* debugMouse_, Sun* sun_, CameraBehavior* cameraBehavior_, MouseBehavior* mouseBehavior_,
	MapModels* mapModels_)
{
	//[カメラ]
	simpleDebugCamera = simpleDebugCamera_;
	sphericalDebugCamera = sphericalDebugCamera_;
	transformDebugMouse = debugMouse_;
	sun = sun_;
	cameraBehavior = cameraBehavior_;
	mouseBehavior = mouseBehavior_;
	mapModels = mapModels_;


}

void DebugScene::Debug()
{
	DebugSceneUI::GetInstance()->DebugBasicalParameter();

}
