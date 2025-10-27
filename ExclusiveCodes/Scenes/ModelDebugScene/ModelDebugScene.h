#pragma once
#include "../../../MyLib/Utilities/SceneStructures/SceneBehavior.h"
#include "../../../MyLib/Utilities/CameraStructures/SphericalDebugCamera.h"
#include "../../Characters/RobotChara/RobotChara.h"
#include "../../../MyLib/Utilities/MouseStructures/TransformDebugMouse.h"


struct ModelDebugScene :public SceneBehavior
{
    //デバッグ用カメラ
    SphericalDebugCamera debugCamera;
    TransformDebugMouse transformDebugMouse;


public:

    virtual void Update() override;
    virtual void Render() override;
    virtual void Initialize() override;
    virtual void Debug() override;

    ModelDebugScene(MapTextures* texID_, MapModels* myModels_);

    // カードの配置パラメータ
    Vector3 directionalArrowPos = { 1.0f,1.0f,0.0f };
};
