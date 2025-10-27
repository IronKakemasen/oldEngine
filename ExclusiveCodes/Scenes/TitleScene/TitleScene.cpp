#include "TitleScene.h"

TitleScene::TitleScene(MapTextures* mapTextures_, MapModels* mapModels_)
{

    mapTextures = mapTextures_;
    mapModels = mapModels_;

    //インスタンス化
    objManager = std::make_unique<ObjectManager>();
    mainCamera = std::make_unique<SampleCamera>();
    lightSun = std::make_unique<Sun>();
    //パーティクルシステムのインスタンス化
    particleSystem = std::make_unique<ParticleSystem>();
    particleSystem->Initialize();

    for (int i = 0; i < CARD_MAX; ++i) {
        auto& card = cards.emplace_back(std::make_unique<Quad>());
        float z = 4.0f - std::abs((CARD_MAX / 2) - i); // 対称な z を計算
        card->Initialize(1.5f, 2.0f, { i * 2.0f - 4.0f, 0.0f, z }, TagTexture::kCardBack);
        card->trans.defaultQuatenion.y_axis.deltaTheta = -60.0f + 30.0f * i;
    }


    //シーンコントローラにこのシーンを登録
    SceneController::GetInstance()->Add(SceneController::TagScene::kTitle, this);

    //ゲームオブジェクトマネージャーに、ゲームオブジェクトとして登録
    //objManager->RegisterAsGameObject(samplePlayer.get());
    ModelCreate();

    //スプライトの初期化
    sprite.InitializeSprite({ 640,360,0 }, 1280, 720, (int)TagTexture::kWhite2x2, { 0,0,0,255 });

    Initialize();
}

void TitleScene::ModelCreate() {

    //ゴールの初期化
    goalCharacter.Initialize(&goalMainTrans);

    titleModel.model = MapModels::GetModelMusic(TagModel::kTitle);
    centerGearModel.model = MapModels::GetModelMusic(TagModel::kGearCenter);
    aroundGearModel.model = MapModels::GetModelMusic(TagModel::kGearAround);
    keyModel.model = MapModels::GetModelMusic(TagModel::kKey);

    centerGearModel.trans.pos = { 0.0f,-1.0f,0.0f };
    aroundGearModel.trans.pos = { 0.0f,-1.0f,0.0f };
    centerGearModel.trans.defaultQuatenion.x_axis.deltaTheta = 90.0f;
    aroundGearModel.trans.defaultQuatenion.x_axis.deltaTheta = 90.0f;
    centerGearModel.trans.scale = { 4.0f,4.0f,4.0f };
    aroundGearModel.trans.scale = { 4.0f,4.0f,4.0f };

    keyModel.trans.BeChildren(&titleModel.trans);
    keyModel.trans.pos = { 2.0f,-0.7f,0.0f };
    keyModel.trans.defaultQuatenion.y_axis.deltaTheta = 45.0f;
    titleModel.color = COLOR_BRONZE;
};

void TitleScene::ModelInitalize() {

    for (int i = 0; i < GOAL_STATES; ++i) {
        counterAndInvTime[i].Initialize(60.0f);
    }

    cameraTimer.Initialize(60.0f);

    goalMainTrans.pos = { 0.0f,0.0f,0.0f };
    //閉じている状態
    goalState = CLOSE;



};
void TitleScene::ModelUpdate() {

    //カウンターの更新処理
    counterAndInvTime[goalState].Add();

    cameraTimer.Add();

    counterAndInvTime[GoalState::CLOSE].IsEnd();

    if (counterAndInvTime[GoalState::UNLOCK].IsEnd()) {
        goalState = OPEN;
    }

    float theta = 6.28f * counterAndInvTime[CLOSE].count;
    titleModel.trans.pos.y = sinf(theta) * 0.125f + 1.0f;

    centerGearModel.trans.defaultQuatenion.z_axis.deltaTheta = theta * 2.0f;
    aroundGearModel.trans.defaultQuatenion.z_axis.deltaTheta += 3.0f;

    //ゴールの更新処理
    goalMainTrans.SetWorldMatBasedQuaternion();
    goalCharacter.Update(mainCamera->viewProjectionMat, goalState, counterAndInvTime[goalState].count);


    //モデルの更新処理
    titleModel.Update(mainCamera->viewProjectionMat);
    centerGearModel.Update(mainCamera->viewProjectionMat);
    aroundGearModel.Update(mainCamera->viewProjectionMat);
    keyModel.Update(mainCamera->viewProjectionMat);

    //カードの更新処理
    for (auto& card : cards) {
        card->Update(mainCamera->viewProjectionMat);
    }

};


void TitleScene::ModelDraw() {

    //ゴールキャラクターの描画
    goalCharacter.Draw();

    titleModel.Draw(1);
    centerGearModel.Draw();
    aroundGearModel.Draw();
    keyModel.Draw();

    NoviceLike::SetCullMode(CullMode::kNone);
    //カードの描画処理
    for (auto& card : cards) {
        card->Draw();
    }
    NoviceLike::SetCullMode(CullMode::kBack);

};


void TitleScene::Update()
{
    AudioPlayer::GetInstance().PlayBGM(AudioHandle::Get(AudioID::gameScene), true, 10);

    //メインカメラの更新処理
    mainCamera->Update();

    static int c = 0;

    if (goalState == CLOSE) {

        mainCamera->trans.pos = Easing::EaseOutQuint(mainCamera->trans.pos, { 0.0f,2.0f,-2.0f }, 0.005f);
        mainCamera->trans.defaultQuatenion.x_axis.deltaTheta = Easing::EaseOutQuint(0.0f, 45.0f, cameraTimer.count);
    } else {
        mainCamera->trans.defaultQuatenion.x_axis.deltaTheta = Easing::EaseOutQuint(45.0f, 0.0f, cameraTimer.count);
        mainCamera->trans.pos = Easing::EaseOutQuint(mainCamera->trans.pos, Vector3{ 0.0f,0.0f,-2.0f }, 0.01f);
    }

    //ライトの更新処理
    lightSun->Update(mainCamera->viewProjectionMat);

    //モデルの更新処理
    ModelUpdate();

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

    //スプライトの更新処理
    sprite.Update(mainCamera->viewProjectionMat);

    //パーティクルシステムの更新処理
    //particleSystem->Update(mainCamera->viewProjectionMat);
    if (fadeState != FadeState::FadeIn && fadeState != FadeState::FadeOut)
    {

        if (NoviceLike::IsKeyTrigger(DIK_SPACE) || NoviceLike::mouseState.rgbButtons[0] && !NoviceLike::preMouseState.rgbButtons[0])
        {
            fadeState = FadeState::FadeOut;
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::decision), false, 100);
            goalState = UNLOCK;
            cameraTimer.IsEnd();
        }

    }

    // フェード処理
    switch (fadeState)
    {
    case FadeState::FadeIn:
        fadeAlpha -= fadeSpeed;
        if (fadeAlpha <= 0.0f)
        {
            fadeAlpha = 0.0f;
            fadeState = FadeState::None;
        }
        break;

    case FadeState::FadeOut:
        fadeAlpha += fadeSpeed;
        if (fadeAlpha >= 1.0f)
        {
            fadeAlpha = 1.0f;
            fadeState = FadeState::Complete;
            // シーン遷移
            SceneController::ReceiveSignal(SceneController::TagScene::kSelect);
        }
        break;

    case FadeState::None:
    case FadeState::Complete:
        break;
    }

    c++;
    int t = c % 60;
    if (t <= 30)
    {
        mouseSprite.texHandle = (int)TagTexture::kPress;
    }
    else
    {
        mouseSprite.texHandle = (int)TagTexture::kCursor;
    }

    //スプライトの更新処理
    sprite.Update(mainCamera->viewProjectionMat);
    sprite.color.w = static_cast<unsigned char>(fadeAlpha * 255);
    mouseSprite.Update(mainCamera->viewProjectionMat);
}

void TitleScene::Render()
{

#ifdef _DEBUG

    //グリッド線
    DrawGrid(mainCamera->viewProjectionMat);
#endif // _DEBUG


    //ゲームオブジェクトの更新処理
    for (auto itr = objManager->GetObjData().begin(); itr != objManager->GetObjData().end(); ++itr)
    {
        if ((*itr)->isActive)
        {
            (*itr)->Draw();
        }
    }

    //モデルの描画
    ModelDraw();

    //サンプルスプライトの描画
    sprite.Draw();

	if (fadeState != FadeState::FadeIn && fadeState != FadeState::FadeOut && goalState != GoalState::OPEN)
	{
        mouseSprite.Draw();
	}

    //パーティクルシステムの描画
    //particleSystem->Draw();
}

void TitleScene::Initialize()
{
    //ウィンドウカラー
    NoviceLike::ChangeWindowColor({ 0.00f,0.00f,0.03f,1.0f });

    //メインカメラの初期化
    mainCamera->Initialize();
    mainCamera->trans.pos = { 0.0f,0.0f,-8.0f };

    //ライトの初期化
    lightSun->Initialize({ 0,1,-1 }, COLOR_PURPLE, "SUN");

    ModelInitalize();

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

    fadeState = FadeState::FadeIn;
    fadeAlpha = 1.0f;
    fadeSpeed = 0.01f;

    float n = 0.25f;
    mouseSprite.InitializeSprite({1100, 600, 0}, 394 * n, 512 * n, (int)TagTexture::kCursor, { 120,120,120,255 });
}

void TitleScene::Debug()
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

    //タイトルシーン
    ImGui::Begin("TitleScene");
    ImGui::End();
}

