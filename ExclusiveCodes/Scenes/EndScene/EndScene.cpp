#include "EndScene.h"

EndScene::EndScene(MapTextures* mapTextures_, MapModels* mapModels_)
{
    mapTextures = mapTextures_;
    mapModels = mapModels_;

    //インスタンス化
    objManager = std::make_unique<ObjectManager>();
    mainCamera = std::make_unique<SampleCamera>();
    lightSun = std::make_unique<Sun>();

    particleSystem = std::make_unique<ParticleSystem>();
    particleSystem->Initialize();

    //シーンコントローラにこのシーンを登録
    SceneController::GetInstance()->Add(SceneController::TagScene::kNone, this);

    //ゲームオブジェクトマネージャーに、ゲームオブジェクトとして登録
    //objManager->RegisterAsGameObject(samplePlayer.get());


    //サンプルスプライトの初期化
    sampleSprite.InitializeSprite({ 50,50,0 }, 100, 100, (int)TagTexture::kEye, { 255,255,255,255 });

    Initialize();
}

void EndScene::Update()
{
    //メインカメラの更新処理
    mainCamera->Update();

    //ライトの更新処理
    lightSun->Update(mainCamera->viewProjectionMat);

    //ゲームオブジェクトの更新処理
    for (auto itr = objManager->GetObjData().begin(); itr != objManager->GetObjData().end(); ++itr)
    {
        if ((*itr)->isActive)
        {
            (*itr)->Update(mainCamera->viewProjectionMat);
            //当たり判定
            objManager->ChackAllCollision((*itr));
            //コリジョンバック
            (*itr)->CollisionBackUpdate();
        }
    }

    //音の処理は任せた
    if (NoviceLike::IsKeyTrigger(DIK_T))
    {
        AudioPlayer::GetInstance().PlayAudio(audioData1, false, 20);
    }

    //サンプルスプライトの更新処理
    sampleSprite.Update(mainCamera->viewProjectionMat);
    sampleSprite.trans.uvPos.u -= 0.01f;

    //パーティクルシステムの更新処理
    //particleSystem->Update(mainCamera->viewProjectionMat);


}

void EndScene::Render()
{
    //グリッド線
    DrawGrid(mainCamera->viewProjectionMat);

    //ゲームオブジェクトの更新処理
    for (auto itr = objManager->GetObjData().begin(); itr != objManager->GetObjData().end(); ++itr)
    {
        if ((*itr)->isActive)
        {
            (*itr)->Draw();
        }
    }

    //サンプルスプライトの描画
    sampleSprite.Draw();

    //パーティクルシステムの描画
    //particleSystem->Draw();
}

void EndScene::Initialize()
{
    //ウィンドウカラー
    NoviceLike::ChangeWindowColor({ 0.1f,0.1f,0.1f,1.0f });

    //メインカメラのポジションにプレイヤーのポジションをセットする
    Vector3 cameraPos = { 0.0f,0.0f,-2.0f };

    //メインカメラの初期化
    mainCamera->Initialize();

    //ライトの初期化
    lightSun->Initialize({ 0,1,-1 }, { 255,255,255,255 }, "SUN");


    //ゲームオブジェクトの初期化
    for (auto itr = objManager->GetObjData().begin(); itr != objManager->GetObjData().end(); ++itr)
    {
        (*itr)->Initialize();
    }

    emitter = new ParticleEmitter(
        ParticleType::None,  // or another valid type
        { 0.0f, 1.0f, 0.0f },  // 発生位置
        1.0f,                // spawnInterval: 0.1秒おきに出す（10個で1秒）
        2.0f                 // lifetime: 2秒で消える
    );
    particleSystem->AddEmitter(emitter);
}

void EndScene::Debug()
{

    ImGui::Begin("DebugBasicalParameter");
    ImGui::Text("deltaTime%.5f", pDxCommon->deltaTime);
    ImGui::Text("FPS%.2f", ImGui::GetIO().Framerate);
    ImGui::End();


    ImGui::Begin("MainCamera");
    ImGui::DragFloat3("Pos", reinterpret_cast<float*>(&mainCamera->trans.pos), 0.1f);
    ImGui::DragFloat3("targetDir", reinterpret_cast<float*>(&mainCamera->trans.targetDir), 0.1f);
    ImGui::DragFloat("defaultQuaternion_X_AXIS",
        &mainCamera->trans.defaultQuatenion.x_axis.deltaTheta, 0.1f);
    ImGui::DragFloat("defaultQuaternion_Y_AXIS",
        &mainCamera->trans.defaultQuatenion.y_axis.deltaTheta, 0.1f);
    ImGui::DragFloat("defaultQuaternion_Z_AXIS",
        &mainCamera->trans.defaultQuatenion.z_axis.deltaTheta, 0.1f);

    ImGui::End();

    //エンドシーン
    ImGui::Begin("EndScene");
    ImGui::End();
}

