#include "ModelDebugScene.h"


ModelDebugScene::ModelDebugScene(MapTextures* mapTextures_, MapModels* mapModels_)
{
    mapTextures = mapTextures_;
    mapModels = mapModels_;

    //インスタンス化
    objManager = std::make_unique<ObjectManager>();
    lightSun = std::make_unique<Sun>();


    //シーンコントローラにこのシーンを登録
    SceneController::GetInstance()->Add(SceneController::TagScene::kNone, this);



    Initialize();
}

void ModelDebugScene::Update()
{
    //メインカメラの更新処理
    debugCamera.Update();
    transformDebugMouse.Update(debugCamera.viewProjectionMat, debugCamera.GetTrans()->GetWorldPos());

    //ライトの更新処理
    lightSun->Update(debugCamera.viewProjectionMat);


}

void ModelDebugScene::Render()
{
    //グリッド線
    DrawGrid(debugCamera.viewProjectionMat);

    transformDebugMouse.Draw(debugCamera.viewProjectionMat);

}

void ModelDebugScene::Initialize()
{
    //ウィンドウカラー
    NoviceLike::ChangeWindowColor({ 0.025f,0.025f,0.025f,1.0f });

    //ライトの初期化
    lightSun->Initialize({ 0,1,-1 }, { 255,255,255,255 }, "SUN");


}

void ModelDebugScene::Debug()
{

}

