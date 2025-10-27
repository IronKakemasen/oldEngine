#include "StageSelectScene.h"
#include "../../InGameController/InGameController.h"

StageSelectScene::StageSelectScene(MapTextures* mapTextures_, MapModels* mapModels_)
{

    mapTextures = mapTextures_;
    mapModels = mapModels_;

    //インスタンス化
    objManager = std::make_unique<ObjectManager>();
    lightSun = std::make_unique<Sun>();
    emitter = std::make_unique<ParticleEmitter>();
    particleSystem = std::make_unique<ParticleSystem>();
    inGameMouse = std::make_unique<InGameMouse>();

    //シーンコントローラにこのシーンを登録
    SceneController::GetInstance()->Add(SceneController::TagScene::kSelect, this);

    Initialize();

    //ジョイントキューブの初期化
    int tex[6] = { 1,1,1,1,1,1 };
    // ジョイントキューブの初期化
    cursorCube.Initialize(1.7f, 2.2f, 0.0001f, { -3.0f,0.5f,2.0f }, tex);
    cursorCube.texHandle = (int)TagTexture::kWhite2x2;
    cursorCube.color = { 255,255,86,255 };
    cursorCube.SynchronizeTexAndColor();


    // 難易度ごとのステージ数
    const int easyCount = 5;
    const int normalCount = 4;
    const int hardCount = 3;

    int easyStartID = 0;
    int normalStartID = easyCount;
    int hardStartID = easyCount + normalCount;

    // 難易度に応じたカードを配置
    //// Hard
    //AddCards(hardCount, 0, Difficulty::Hard, hardStartID);
    // Normal
    AddCards(normalCount, 0, Difficulty::Normal, normalStartID);
    // Easy
    AddCards(easyCount, 1, Difficulty::Easy, easyStartID);

    emitter->Initialize(
        ParticleType::SelectSceneBackGround,
        { 0.0f, 0.0f, 0.0f },
        0.8f,
        8.0f,
        50
    );

    particleSystem->AddEmitter(emitter.get());
    particleSystem->Initialize();

    //スプライトの初期化
    sprite.InitializeSprite({ 640,360,0 }, 1280, 720, (int)TagTexture::kWhite2x2, { 0,0,0,255 });
    stageSprite.InitializeSprite({ 180,70,0 }, 400, 100, (int)TagTexture::kStageFont, { 255,255,255,255 });
}

void StageSelectScene::Update()
{
    AudioPlayer::GetInstance().PlayBGM(AudioHandle::Get(AudioID::gameScene), true, 10);

    //メインカメラの更新処理
    camera.Update();
    mouse.Update(camera.viewProjectionMat, camera.GetTrans()->GetWorldPos());
    //ライトの更新処理
    lightSun->Update(camera.viewProjectionMat);

    // まず全カードの色を元に戻す
    for (auto& card : cards) {
        switch (card->row) {
        //case 0: // Hard
        //    card->jointCube.color = { 255, 0, 58, 255 };
        //    card->AuraJointCube.color = { 255, 0, 58, 60 };
        //    break;
        case 0: // Normal
            card->jointCube.color = { 0, 50, 255, 255 };
            card->AuraJointCube.color = { 0, 50, 255, 60 };
            break;
        case 1: // Easy
            card->jointCube.color = { 0, 255, 71, 255 };
            card->AuraJointCube.color = { 0, 255, 71, 60 };
            break;
        }
        card->jointCube.SynchronizeColor();
    }


    // 入力処理
    // 移動先の行のカード数に列を合わせる
    auto ClampSelectedCol = [&](int row) 
        {
        if (stageCounts[row] == 0) {
            selectedCol[row] = 0;
        }
        else if (selectedCol[row] >= stageCounts[row]) {
            selectedCol[row] = stageCounts[row] - 1;
        }
        };

    if (fadeState != FadeState::FadeIn && fadeState != FadeState::FadeOut)
    {
        if (NoviceLike::IsKeyTrigger(DIK_W) || NoviceLike::IsKeyTrigger(DIK_UP)) {
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::cardTouch), false, 70);
            int newRow = selectedRow + 1;
            if (newRow < 2) {
                // 現在の列番号が移動先の最大列数より大きければ調整
                if (selectedCol[selectedRow] >= stageCounts[newRow]) {
                    selectedCol[newRow] = stageCounts[newRow] - 1;
                }
                else {
                    selectedCol[newRow] = selectedCol[selectedRow];                 
                }
                selectedRow = newRow;
                // 回転開始トリガーを付ける
                Card* card = GetSelectedCard();
                if (card) {
                    card->isRotating = true;
                    card->targetRotationY += 360.0f;
                }
            }
        }

        if (NoviceLike::IsKeyTrigger(DIK_S) || NoviceLike::IsKeyTrigger(DIK_DOWN)) {
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::cardTouch), false, 70);
            int newRow = selectedRow - 1;
            if (newRow >= 0) {
                if (selectedCol[selectedRow] >= stageCounts[newRow]) {
                    selectedCol[newRow] = stageCounts[newRow] - 1;
                }
                else {
                    selectedCol[newRow] = selectedCol[selectedRow];                    
                }
                selectedRow = newRow;

                // 回転開始トリガーを付ける
                Card* card = GetSelectedCard();
                if (card) {
                    card->isRotating = true;
                    card->targetRotationY += 360.0f;
                }
            }
        }

        if (NoviceLike::IsKeyTrigger(DIK_A) || NoviceLike::IsKeyTrigger(DIK_LEFT)) {
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::cardTouch), false, 70);
            if (selectedCol[selectedRow] > 0) {
                selectedCol[selectedRow]--;
                // 回転開始トリガーを付ける
                Card* card = GetSelectedCard();
                if (card) {
                    card->isRotating = true;
                    card->targetRotationY += 360.0f;
                }
            }
        }

        if (NoviceLike::IsKeyTrigger(DIK_D) || NoviceLike::IsKeyTrigger(DIK_RIGHT)) {
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::cardTouch), false, 70);
            if (selectedCol[selectedRow] < stageCounts[selectedRow] - 1) {
                selectedCol[selectedRow]++;
                // 回転開始トリガーを付ける
                Card* card = GetSelectedCard();
                if (card) {
                    card->isRotating = true;
                    card->targetRotationY += 360.0f;
                }
            }
        }

        Card* hoveredCard = nullptr;
        for (auto& card : cards)
        {
            if (inGameMouse->CollisionWithMouse3D(card->jointCube.trans.pos, 50.0f, camera.viewProjectionMat)) {
                hoveredCard = card.get();
                break;
            }
        }

        // 前フレームと違うカードに乗った瞬間だけSEを鳴らす
        if (hoveredCard != nullptr && hoveredCard != prevHoveredCard) {
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::cardTouch), false, 100);
            selectedRow = hoveredCard->row;
            selectedCol[selectedRow] = hoveredCard->col;
            cursorCube.trans.pos = hoveredCard->originalPos;
            hoveredCard->isRotating = true;
            hoveredCard->targetRotationY += 360.0f;  // 1 回転分の角度を追加
        }

        prevHoveredCard = hoveredCard;

        // SPACEでステージ遷移
        if (NoviceLike::IsKeyTrigger(DIK_SPACE) || ((NoviceLike::mouseState.rgbButtons[0] && !NoviceLike::preMouseState.rgbButtons[0]) && hoveredCard != nullptr)) {
            Card* selectedCard = GetSelectedCard();
            if (selectedCard)
            {
                AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::decision), false, 100);
                InGameController::currentStageNo = static_cast<int>(selectedCard->moveValue);
                InGameController::haveStartdeFromSelect = true;
                fadeState = FadeState::FadeOut;
                // シーン遷移

            }
        }
    }

    Card* selectedCard = GetSelectedCard();
 
    if (GetSelectedCard()) 
    {
        Vector3 cardPos = GetSelectedCard()->originalPos;
        cursorCube.trans.pos = {
            cardPos.x,
            cardPos.y,
            cardPos.z
        }; 
    }

  
    if (selectedCard) {
        // cursorCubeの位置と重なっているカードの色を変える   
        selectedCard->AuraJointCube.color = { 255, 255, 0, 120 };  
        selectedCard->AuraJointCube.SynchronizeColor();
    }


    // カード更新
    for (auto& card : cards) {
        card->Update(camera.viewProjectionMat);
    }

    // 星更新
    for (auto& star : selectStars) {
        star->Update(camera.viewProjectionMat);
    }

    // 選択カードのハイライト処理
    Card* highlightCard = GetSelectedCard();
    if (highlightCard) {
        highlightCard->isAura = true;
    }

    // カードの更新
    for (auto& card : cards) {
        card->Update(camera.viewProjectionMat);
    }
    
    // 星更新
    for (auto& star : selectStars) {
        star->Update(camera.viewProjectionMat);
    }

    //cursorCube.Update(camera.viewProjectionMat);

    particleSystem->Update(camera.viewProjectionMat);

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
            SceneController::ReceiveSignal(SceneController::TagScene::kInGame);
        }
        break;

    case FadeState::None:
    case FadeState::Complete:
        break;
    }

    for (auto& card : cards)
    {
        if (!InGameController::isCleared[static_cast<int>(card->moveValue)])
        {
            if (!selectedCard)
            {
                switch (card->row) {

                case 0: // Normal
                    card->jointCube.color = { 0, 50, 255, 255 };
                    card->AuraJointCube.color = { 0, 50, 255, 60 };
                    break;
                case 1: // Easy
                    card->jointCube.color = { 0, 255, 71, 255 };
                    card->AuraJointCube.color = { 0, 255, 71, 60 };
                    break;
                }
                card->jointCube.SynchronizeColor();
            }
        }
        else
        {
            card->jointCube.color = { 60.0f,60.0f ,60.0f , 255.0f };
            card->jointCube.SynchronizeColor();
        }
    }


    //スプライトの更新処理
    sprite.Update(camera.viewProjectionMat);
    sprite.color.w = static_cast<unsigned char>(fadeAlpha * 255);
    stageSprite.Update(camera.viewProjectionMat);

}

void StageSelectScene::Render()
{
    //グリッド線
   /* DrawGrid(camera.viewProjectionMat);*/

    // カードの描画
    for (auto& card : cards) {
        card->Draw();
    }

    // 星描画
    for (auto& star : selectStars) {
        star->Draw();
    }

   /* cursorCube.Draw();*/

    particleSystem->Draw();

    if (fadeAlpha > 0.0f) {
        //スプライトの描画
        sprite.Draw();
    }

    stageSprite.Draw();
}

void StageSelectScene::Initialize()
{
    //ウィンドウカラー
    NoviceLike::ChangeWindowColor({ 0.025f,0.025f,0.025f,1.0f });
    //ライトの初期化
    lightSun->Initialize({ 0,1,-1 }, { 255,255,255,255 }, "SUN");

    camera.trans.defaultQuatenion.x_axis.deltaTheta = 0.0f;
    camera.trans.pos = { 5.2f,3.4f,-6.6f };

    fadeState = FadeState::FadeIn; 
    fadeAlpha = 1.0f;              
    fadeSpeed = 0.01f;
}

void StageSelectScene::AddCards(int count, int row, Difficulty difficulty, int stageID)
{
    int starCount = 0;

    switch (difficulty)
    {
    case Difficulty::Easy: starCount = 1; break;
    case Difficulty::Normal: starCount = 2; break;
   /* case Difficulty::Hard: starCount = 3; break;*/
    }

    for (int i = 0; i < count; ++i)
    {
        auto card = std::make_unique<Card>();

        float x = i * cardSpacing;
        float y = row * rowOffset;
        float z = 0.0f;

        card->originalPos = { x, y, z };
        card->currentPos = card->originalPos;
        card->targetPos = card->originalPos;
        // rowとcol をセットする
        
        card->row = row;
        card->col = i;
        card->Initialize((float)(i + 1));
        card->moveValue = float(stageID + i);

        // 難易度によって色を変える
        switch (difficulty)
        {
        case Difficulty::Easy:
            card->jointCube.color = { 0, 255, 71, 255 };
            card->AuraJointCube.color = { 0, 255, 71, 60 };
            break;
        case Difficulty::Normal:
            card->jointCube.color = { 0, 50, 255, 255 };
            card->AuraJointCube.color = { 0, 50, 255, 60 };
            break;
        /*case Difficulty::Hard:
            card->jointCube.color = { 255, 0, 58, 255 };
            card->AuraJointCube.color = { 255, 0, 58, 60 };
            break;*/
        }

        card->jointCube.SynchronizeColor();

        // 星を左に並べる
        float starSpacing = 1.8f;
        float baseX = card->originalPos.x - 3.8f;
        float baseY = card->originalPos.y;
        float baseZ = card->originalPos.z;

        // 中心揃え用にオフセットを計算
        float totalWidth = (starCount - 1) * starSpacing;
        float offsetX = totalWidth / 2.0f;

        if (i == 0)
        {

            for (int s = 0; s < starCount; ++s)
            {
                auto star = std::make_unique<SelectStar>();
                star->Initialize();
                star->jointCube.trans.pos = {
                    baseX - (s * starSpacing) + offsetX,
                    baseY,
                    baseZ
                };
                                   
                selectStars.push_back(std::move(star));
            }
        }

        cards.push_back(std::move(card));
    }
}

void StageSelectScene::Debug()
{
    if (ImGui::TreeNode("Camera"))
    {
        ImGui::DragFloat3("Pos", reinterpret_cast<float*>(&camera.trans.pos), 0.1f);
        ImGui::DragFloat3("scale", reinterpret_cast<float*>(&camera.trans.scale), 0.01f);
        ImGui::DragFloat3("targetDir", reinterpret_cast<float*>(&camera.trans.targetDir), 0.01f);
        ImGui::DragFloat("defaultQuaternion_X_AXIS",
            &camera.trans.defaultQuatenion.x_axis.deltaTheta, 0.1f);
        ImGui::DragFloat("defaultQuaternion_Y_AXIS",
            &camera.trans.defaultQuatenion.y_axis.deltaTheta, 0.1f);
        ImGui::DragFloat("defaultQuaternion_Z_AXIS",
            &camera.trans.defaultQuatenion.z_axis.deltaTheta, 0.1f);

        ImGui::TreePop();
    }

}

Card* StageSelectScene::GetSelectedCard()
{
    int row = selectedRow;
    int col = selectedCol[row];

    // 累積で探す
    int index = 0;
    for (int r = 0; r < row; ++r) {
        index += stageCounts[r];
    }
    index += col;

    if (index >= 0 && index < cards.size()) {
        return cards[index].get();
    }

    return nullptr;
}

