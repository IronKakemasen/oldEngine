#include "GameScene.h"
#include"../../UI/UI.h"
#include "../../Utilities/SoheStructures.h"
#include "../../../MyLib/Utilities/BenriFunctionStructures/BenriFunctions.h"

GameScene::GameScene(MapTextures* mapTextures_, MapModels* mapModels_)
{
    mapTextures = mapTextures_;
    mapModels = mapModels_;

    //インスタンス化
    objManager = std::make_unique<ObjectManager>();
    mainCamera = std::make_unique<GameCamera>();
    lightSun = std::make_unique<Sun>();
    inGameController = std::make_unique<InGameController>();
    player = std::make_unique<Player>();
    goal = std::make_unique<Goal>();
    particleSystem = std::make_unique<ParticleSystem>();
    key = std::make_unique<Key>();
    inGameMouse = std::make_unique<InGameMouse>();
    directionalArrow = std::make_unique<DirectionalArrow>();
    movableField = std::make_unique<MovableField>();
    predictiveField = std::make_unique<MovableField>();
    cardManager = std::make_unique<CardManager>();
    arrowClickAreaManager = std::make_unique<ArrowClickAreaManager>();
    backGroundEmitter = std::make_unique<ParticleEmitter>();
    goalEmitter = std::make_unique<ParticleEmitter>();
    keyEmitter = std::make_unique<ParticleEmitter>();
    clearEmitter = std::make_unique<ParticleEmitter>();
    playerIdleEmitter = std::make_unique<ParticleEmitter>();
    getKeyEmitter = std::make_unique<ParticleEmitter>();

    //おｓでょえぴｊんｃｆ：ｐ
    mainCamera->zoomCounter = &inGameController->enterTimer.count;
    mainCamera->zoomInCounter= &inGameController->resultTimer.count;

    map0 = std::make_unique<Map0>();
    map1 = std::make_unique<Map1>();
    map2 = std::make_unique<Map2>();
    map3 = std::make_unique<Map3>();
    map4 = std::make_unique<Map4>();

    map5 = std::make_unique<Map5>();
    map6 = std::make_unique<Map6>();
    map7 = std::make_unique<Map7>();
    map8 = std::make_unique<Map8>();
    map9 = std::make_unique<Map9>();
    map10 = std::make_unique<Map10>();
    map11 = std::make_unique<Map11>();
    map12 = std::make_unique<Map12>();
    map13 = std::make_unique<Map13>();
    map14 = std::make_unique<Map14>();

    predictiveArrow = std::make_unique<PredictiveArrow>();
    predictiveCardUI = std::make_unique<PredictiveCardUI>();

    map1->mouse = inGameMouse.get();


    //keyにplayerとcollisionをセットする
    key->SetPlayerAndCollison(player.get());
    //goalにplayerとcollisionをセットする
    goal->SetPlayerAndCollison(player.get());

    //ゲームコントローラーにプレイヤーを入れる
    inGameController->player = player.get();
    //ゲームコントローラーにkeyを入れる
    inGameController->key = key.get();
    //ゲームコントローラーにゴール
    inGameController->goal = goal.get();
    //ゲームコントローラーにゲームカメラ
    inGameController->gameCamera = mainCamera.get();
    //ゲームコントローラーに矢印
    inGameController->directionalArrow = directionalArrow.get();
    //ゲームコントローラーに移動可能領域
    inGameController->movableField = movableField.get();
    //ゲームコントローラーにka-doUIyosoku
    inGameController->predictiveCardUI = predictiveCardUI.get();

    //ゲームコントローラーに矢印
    inGameController->cardManager = cardManager.get();
    //ゲームコントローラーに移動可能領域
    inGameController->arrowClickAreaManager = arrowClickAreaManager.get();


    //ゲームコントローラーのマップコンテナに登録する
    inGameController->RegisterMap(map0.get());
    inGameController->RegisterMap(map1.get());
    inGameController->RegisterMap(map2.get());
    inGameController->RegisterMap(map3.get());
    inGameController->RegisterMap(map4.get());
    inGameController->RegisterMap(map5.get());
    //inGameController->RegisterMap(map6.get());
    inGameController->RegisterMap(map7.get());
    inGameController->RegisterMap(map8.get());
    //inGameController->RegisterMap(map9.get());
    inGameController->RegisterMap(map10.get());
    //inGameController->RegisterMap(map11.get());
    //inGameController->RegisterMap(map12.get());
    //inGameController->RegisterMap(map13.get());
    //inGameController->RegisterMap(map14.get());


    //シーンコントローラにこのシーンを登録
    SceneController::GetInstance()->Add(SceneController::TagScene::kInGame, this);

    //ゲームオブジェクトマネージャーに、ゲームオブジェクトとして登録
    objManager->RegisterAsGameObject(player.get());
    objManager->RegisterAsGameObject(goal.get());
    objManager->RegisterAsGameObject(key.get());

    //全てのゲームオブジェクトのコリジョンバックを設定
    for (auto itr = objManager->GetObjData().begin(); itr != objManager->GetObjData().end(); ++itr)
    {
        (*itr)->SetCollisionBackTable();
    }

    //デバッグカメラ
    debugCamera.Initialize();
    debugCamera.trans.BeChildren(&debugCameraParent);
    debugCameraParent.pos = InGameMapBehavior::GetPositionBasedInIndex(6, 6);

    currentCamera = &debugCamera;

    //メインカメラのポジションにプレイヤーのポジションをセットする
    mainCamera->targetPosition = &player.get()->mainTrans.pos;

    Initialize();

    shrinkCounter.Initialize(60.0f);

    backGroundEmitter->Initialize(
        ParticleType::GameSceneBackGround,
        { 0.0f, 0.0f, 0.0f },
        0.3f,
        6.0f,
        30
    );

    goalEmitter->Initialize(
        ParticleType::Goal,
        { 0.0f, 0.0f, 0.0f },
        0.3f,
        2.0f,
        30
    );
    keyEmitter->Initialize(
        ParticleType::Key,
        { 0.0f, 0.0f, 0.0f },
        0.3f,
        2.0f,
        20
    );
    clearEmitter->Initialize(
        ParticleType::Clear,
        { 0.0f, 0.0f, 0.0f },
        0.05f,
        1.0f,
        20
    );
    playerIdleEmitter->Initialize(
        ParticleType::PlayerIdle,
        { 0.0f, 0.0f, 0.0f },
        0.1f,
        2.0f,
        30
    );
    getKeyEmitter->Initialize(
        ParticleType::GetKey,
        { 0.0f, 0.0f, 0.0f },
        0.02f,
        2.0f,
        30
    );

    particleSystem->AddEmitter(backGroundEmitter.get());
    particleSystem->AddEmitter(goalEmitter.get());
    particleSystem->AddEmitter(keyEmitter.get());
    particleSystem->AddEmitter(clearEmitter.get());
    particleSystem->AddEmitter(playerIdleEmitter.get());
    particleSystem->AddEmitter(getKeyEmitter.get());
    particleSystem->Initialize();
}

void GameScene::Update()
{
    AudioPlayer::GetInstance().PlayBGM(AudioHandle::Get(AudioID::gameScene), true, 10);

    //GameObject* player = nullptr;

    //デバッグカメラ
    if (cameraChanger) currentCamera = &debugCamera;
    else currentCamera = mainCamera.get();

    //デバッグカメラ専用のトランス
    debugCameraParent.SetWorldMatBasedQuaternion();

    //メインカメラの更新処理
    currentCamera->Update();

    //ライトの更新処理
    lightSun->Update(currentCamera->viewProjectionMat);

    //マウスの更新処理
    inGameMouse->Update(currentCamera->viewProjectionMat, {});

    //インゲームコントローラーの更新処理
    inGameController->Update(*inGameMouse, mainCamera->viewProjectionMat);

    //ゲームオブジェクトの更新処理
    //ポーズ以外の時
    if (inGameController->state != InGameController::State::kPause)
    {
        for (auto itr = objManager->GetObjData().begin(); itr != objManager->GetObjData().end(); ++itr)
        {
            if ((*itr)->isActive)
            {
                (*itr)->Update(currentCamera->viewProjectionMat);
                //当たり判定
                objManager->ChackAllCollision((*itr));
                //コリジョンバック
                (*itr)->CollisionBackUpdate();
            }
        }



    // MovableBlock全体の処理
    ActivatePredictCube();
    UpdateMovableBlock();

    // カードの更新
    UpdateCard();
 
    //現在マップの更新
    inGameController->stages[inGameController->currentStageNo]->Update(currentCamera->viewProjectionMat);

    ShrinkBlocks();
    UpdateParticle();
    particleSystem->Update(currentCamera->viewProjectionMat);

    }

}

void GameScene::Render()
{
    //ゲームオブジェクトの更新処理
    for (auto itr = objManager->GetObjData().begin(); itr != objManager->GetObjData().end(); ++itr)
    {
        if ((*itr)->isActive)
        {
            (*itr)->Draw();
        }
    }

    //現在マップの描画
    inGameController->stages[inGameController->currentStageNo]->Draw();


    predictiveCardUI->Draw();

    // 矢印の描画
    directionalArrow->Draw();

    // 領域の描画
    movableField->Draw();

    arrowClickAreaManager->Draw();
    cardManager->DrawAll();

    //ゲーム審判の描画
    inGameController->Draw();
    particleSystem->Draw();

    // カードの描画
    if (cardManager->HasSelectedCard())
    {
        // 予測矢印の描画
        predictiveArrow->Draw();

        predictiveField->Draw();
    }

}

void GameScene::Initialize()
{
    //ウィンドウカラー
    NoviceLike::ChangeWindowColor({ 0.00f,0.00f,0.03f,1.0f });

    //メインカメラの初期化
    mainCamera->Initialize();

    //ライトの初期化
    lightSun->Initialize({ 0,1,-1 }, { 255,255,255,255 }, "SUN");

    //ゲームオブジェクトの初期化
    for (auto itr = objManager->GetObjData().begin(); itr != objManager->GetObjData().end(); ++itr)
    {
        (*itr)->Initialize();
    }

    //ゲームコントローラーの初期化
    inGameController->Initialize();


}

void GameScene::Debug()
{
    static Vector2 mousePos;
    mousePos = { (float)NoviceLike::mousePos.x,(float)NoviceLike::mousePos.y };

    ImGui::Begin("DebugBasicalParameter");
    ImGui::Text("GameScene");
    ImGui::Text("deltaTime%.5f", pDxCommon->deltaTime);
    ImGui::Text("FPS%.2f", ImGui::GetIO().Framerate);
    ImGui::DragFloat2("MousePos", reinterpret_cast<float*>(&mousePos), 0.01f);
    ImGui::Checkbox("debugCameraMode", &cameraChanger);
    ImGui::Text("[Arrow Keys]:Move [ O , K ]:MoveForwardBack");

    if (ImGui::TreeNode("CurrentCamera"))
    {
        ImGui::DragFloat3("Pos", reinterpret_cast<float*>(&currentCamera->trans.pos), 0.1f);
        ImGui::DragFloat3("scale", reinterpret_cast<float*>(&currentCamera->trans.scale), 0.01f);
        ImGui::DragFloat3("targetDir", reinterpret_cast<float*>(&currentCamera->trans.targetDir), 0.01f);
        ImGui::DragFloat("defaultQuaternion_X_AXIS",
            &currentCamera->trans.defaultQuatenion.x_axis.deltaTheta, 0.1f);
        ImGui::DragFloat("defaultQuaternion_Y_AXIS",
            &currentCamera->trans.defaultQuatenion.y_axis.deltaTheta, 0.1f);
        ImGui::DragFloat("defaultQuaternion_Z_AXIS",
            &currentCamera->trans.defaultQuatenion.z_axis.deltaTheta, 0.1f);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Player"))
    {
        ImGui::Text(player->GetCurrentStateInString().c_str());
        ImGui::SliderFloat("stateCount", &player->stateCount[player->state], 0.0f, 1.0f);
        ImGui::SliderFloat3("targetDir", &player->mainTrans.targetDir.x, -1.0f, 1.0f);
        ImGui::SliderFloat3("mainTransPos", &player->mainTrans.pos.x, -1.0f, 100.0f);
        ImGui::Text("direction %d", player->direction);
        if (ImGui::Button("Init")) {
            player->Initialize();
        }
        ImGui::Text("isGetKey %d", player->isGetKey);
        ImGui::TreePop();

    }

    if (ImGui::TreeNode("Key"))
    {

        ImGui::SliderFloat("QuatenionRotate", &key->mainTrans.defaultQuatenion.y_axis.deltaTheta, 0.0f, 720.0f);
        ImGui::SliderFloat3("scale", &key->mainTrans.scale.x, 0.0f, 10.0f);
        ImGui::Text("isGravity %d", key->isGravity);

        if (ImGui::Button("Init")) {
            key->Initialize();
        }

        ImGui::TreePop();

    }
    if (ImGui::TreeNode("Goal"))
    {
        ImGui::Text("isGravity %d", goal->isGravity);
        ImGui::Text("State %d",goal->state);
        ImGui::SliderFloat("stateCount", &goal->stateCount[goal->state], 0.0f, 1.0f);
        if (ImGui::Button("Init")) {
            goal->Initialize();
        }

        ImGui::TreePop();

    }


    if (ImGui::TreeNode("InGameController"))
    {
        ImGui::Text(inGameController->GetCurrentStateInString().c_str());
        ImGui::SliderFloat("transitionTimer", &inGameController->transitionTimer, 0.0f, 10.0f);

        if (ImGui::Button("Reset")) {
            inGameController->ResetGame();
        }
        if (ImGui::Button("isPose")) {
            inGameController->ui->SwitchPauze();
        }
        ImGui::Text("isPose %d", inGameController->ui->isPause);


        ImGui::SliderFloat("UI moveTimer", &inGameController->ui->moveTimer, 0.0f, 1.0f);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("GameCamera")) {
        //ImGui::Text("isFollow %d", mainCamera->isFollow);

        //if (ImGui::Button("isFollow")) {
        //    mainCamera->isFollow = mainCamera->isFollow ? false : true;
        //    mainCamera->InitStartPosAndMoveTimer();
        //}
        ImGui::SliderFloat("followSpeed", &mainCamera->followSpeed, 0.0f, 1.0f);
        ImGui::TreePop();
    }

    ImGui::End();

    ImGui::Begin("IndexMap");
    for (uint32_t i = 0; i < InGameMap::kNumBlockVertical; ++i)
    {
        int tmp[InGameMap::kNumBlockHorizontal];

        for (int k = 0; k < InGameMap::kNumBlockHorizontal; ++k)
        {
            tmp[k] = inGameController->currentIndexPosRecords[i][k];
        }

        ImGui::Text("%d ,%d ,%d ,%d ,%d ,%d ,%d ,%d ,%d ,%d ,%d ,%d", tmp[0], tmp[1], tmp[2],
            tmp[3], tmp[4], tmp[5], tmp[6], tmp[7], tmp[8], tmp[9], tmp[10], tmp[11]);
    }
    ImGui::End();



}


































void GameScene::ActivatePredictCube()
{
    Card* card = cardManager->GetSelectedCard();

    //isActiveをfalseにしておく
    for (int i = 0; i < arrowClickAreaManager->kNumPredictionCubes; ++i)
    {
        arrowClickAreaManager->predictionalCubes[i].isActive = false;
    }

    //カードを選択していたら
    if (card)
    {
        //いったんすべてのブロックのインデックスポジションを記録する
        for (auto& block : inGameController->stages[inGameController->currentStageNo]->movableBlocks)
        {
            //IndexMapRecordsに記録する
            InGameController::RecordIndexPosition(block->mainTrans.GetWorldPos(), (int)TagChip::kMovableBlock);
        }

        for (auto itr = arrowClickAreaManager->arrowClickAreas.begin(); itr != arrowClickAreaManager->arrowClickAreas.end(); ++itr)
        {
            Vector3 clickAreaPos = (*itr)->chara.trans.GetWorldPos();

            if (inGameMouse->CollisionWithMouse3D(clickAreaPos, arrowClickAreaManager->collisionRadius, mainCamera->viewProjectionMat))
            {
                //すべてのムーバブルブロックのポジションの移動先をシミュレーションする
                for (auto move = inGameController->stages[inGameController->currentStageNo]->movableBlocks.begin();
                    move != inGameController->stages[inGameController->currentStageNo]->movableBlocks.end(); ++move)
                {

                    PredictionalCube* predicCube = nullptr;

                    //描画用キューブ
                    for (int k = 0; k < arrowClickAreaManager->kNumPredictionCubes; ++k)
                    {
                        if (!arrowClickAreaManager->predictionalCubes[k].isActive)
                        {
                            predicCube = &arrowClickAreaManager->predictionalCubes[k];
                            predicCube->cube.trans.pos = (*move)->mainTrans.pos;

                            predicCube->cube.local_surfaces[C_TOP].leftTop.position.x = -Chip::kBlockWidth * 0.5f;
                            predicCube->cube.local_surfaces[C_TOP].rightTop.position.x = Chip::kBlockWidth * 0.5f;
                            predicCube->cube.local_surfaces[C_TOP].rightBottom.position.x = Chip::kBlockWidth * 0.5f;
                            predicCube->cube.local_surfaces[C_TOP].leftBottom.position.x = -Chip::kBlockWidth * 0.5f;

                            predicCube->cube.local_surfaces[C_BOTTOM].leftTop.position.x = -Chip::kBlockWidth * 0.5f;
                            predicCube->cube.local_surfaces[C_BOTTOM].rightTop.position.x = Chip::kBlockWidth * 0.5f;
                            predicCube->cube.local_surfaces[C_BOTTOM].rightBottom.position.x = Chip::kBlockWidth * 0.5f;
                            predicCube->cube.local_surfaces[C_BOTTOM].leftBottom.position.x = -Chip::kBlockWidth * 0.5f;

                            predicCube->cube.local_surfaces[C_TOP].leftTop.position.y = Chip::kBlockWidth*0.5f;
                            predicCube->cube.local_surfaces[C_TOP].rightTop.position.y = Chip::kBlockWidth * 0.5f;
                            predicCube->cube.local_surfaces[C_TOP].rightBottom.position.y = Chip::kBlockWidth * 0.5f;
                            predicCube->cube.local_surfaces[C_TOP].leftBottom.position.y = Chip::kBlockWidth * 0.5f;
                           
                            predicCube->cube.local_surfaces[C_BOTTOM].leftTop.position.y = -Chip::kBlockWidth * 0.5f;
                            predicCube->cube.local_surfaces[C_BOTTOM].rightTop.position.y = -Chip::kBlockWidth * 0.5f;
                            predicCube->cube.local_surfaces[C_BOTTOM].rightBottom.position.y = -Chip::kBlockWidth * 0.5f;
                            predicCube->cube.local_surfaces[C_BOTTOM].leftBottom.position.y = -Chip::kBlockWidth * 0.5f;


                            break;
                        }
                    }

                    if (!predicCube) break;

                    float numSpace = 0;

                    IndexedPosition currentIndex = InGameMap::GetIndexedPositionBasedInPosition((*move)->mainTrans.pos);

                    Vector3 endPos;

                    switch (predictiveArrow->direction)
                    {
                    case MovableBlock::ArrowDirection::Up:
                    {
                        if ((*move)->mainTrans.pos.x< clickAreaPos.x)
                        {
                            continue;
                        }


                        IndexedPosition indexPos = InGameMapBehavior::GetIndexedPositionBasedInPosition((*move)->mainTrans.pos);

                        int nextTag;
                        int next = -1;

                        while (1)
                        {
                            if (indexPos.Y + next >= 0 && indexPos.Y + next < InGameMapBehavior::kNumBlockVertical - 1 &&
                                indexPos.X >= 0 && indexPos.X < InGameMapBehavior::kNumBlockHorizontal - 1)
                            {
                                nextTag = InGameController::currentIndexPosRecords[indexPos.Y + next][indexPos.X];

                                if (nextTag == (int)TagChip::kSpace && (nextTag != 9))
                                {
                                    numSpace++;
                                }
                                else if (nextTag == (int)TagChip::kBlock)
                                {
                                    break;
                                }


                                next--;
                            }

                            else
                            {
                                break;
                            }

                        }

                        Max(numSpace, card->moveValue);

                        float tmp = numSpace * ChipBehavior::kBlockHeight ;
                        
                        predicCube->cube.local_surfaces[C_TOP].leftTop.position.y += tmp;
                        predicCube->cube.local_surfaces[C_TOP].rightTop.position.y += tmp;
                        predicCube->cube.local_surfaces[C_TOP].rightBottom.position.y += tmp;
                        predicCube->cube.local_surfaces[C_TOP].leftBottom.position.y += tmp;

                        predicCube->isActive = bool(numSpace);

                        break;
                    }

                    case MovableBlock::ArrowDirection::Down:
                    {
                        if ((*move)->mainTrans.pos.x < clickAreaPos.x)
                        {
                            continue;
                        }

                        IndexedPosition indexPos = InGameMapBehavior::GetIndexedPositionBasedInPosition((*move)->mainTrans.pos);

                        int nextTag;
                        int next = 1;

                        while (1)
                        {
                            if ((indexPos.Y + next) >= 0 && (indexPos.Y + next) < InGameMapBehavior::kNumBlockVertical - 1 &&
                                indexPos.X >= 0 && indexPos.X < InGameMapBehavior::kNumBlockHorizontal - 1)
                            {
                                nextTag = InGameController::currentIndexPosRecords[indexPos.Y + next][indexPos.X];

                                if (nextTag == (int)TagChip::kSpace && (nextTag != 9))
                                {
                                    numSpace++;
                                }
                                else if (nextTag == (int)TagChip::kBlock)
                                {
                                    break;
                                }

                                next++;

                            }
                            else
                            {
                                break;
                            }


                        }


                        Max(numSpace, card->moveValue);

                        float tmp = numSpace * ChipBehavior::kBlockHeight;

                        predicCube->cube.local_surfaces[C_BOTTOM].leftTop.position.y -= tmp;
                        predicCube->cube.local_surfaces[C_BOTTOM].rightTop.position.y -= tmp;
                        predicCube->cube.local_surfaces[C_BOTTOM].rightBottom.position.y -= tmp;
                        predicCube->cube.local_surfaces[C_BOTTOM].leftBottom.position.y -= tmp;


                        predicCube->isActive = bool(numSpace);

                        break;
                    }

                    case MovableBlock::ArrowDirection::Left:
                    {
                        if ((*move)->mainTrans.pos.y < clickAreaPos.y)
                        {
                            continue;
                        }

                        IndexedPosition indexPos = InGameMapBehavior::GetIndexedPositionBasedInPosition((*move)->mainTrans.pos);

                        int nextTag;
                        int next = -1;

                        float numObj = 0;
                        int nextUnderTagEqualSpace = 0;
                        int nextUnderTag;

                        while (1)
                        {
                            if (indexPos.Y >= 0 && indexPos.Y < InGameMapBehavior::kNumBlockVertical - 1 &&
                                indexPos.X + next >= 0 && indexPos.X + next < InGameMapBehavior::kNumBlockHorizontal - 1)
                            {
                                nextTag = InGameController::currentIndexPosRecords[indexPos.Y][indexPos.X + next];
                                nextUnderTag = InGameController::currentIndexPosRecords[indexPos.Y + 1][indexPos.X + next];


                                if ((nextUnderTag != (int)TagChip::kBlock) && nextUnderTag != (int)TagChip::kMovableBlock)
                                {
                                    nextUnderTagEqualSpace++;
                                }

                                if ((nextTag == (int)TagChip::kSpace) || (nextTag == 9))
                                {
                                    numSpace++;
                                }
                                else if (nextTag == (int)TagChip::kGoal || nextTag == (int)TagChip::kPlayer || nextTag == (int)TagChip::kKey)
                                {
                                    numObj++;
                                }

                                else if (nextTag == (int)TagChip::kBlock)
                                {
                                    if (numObj > 0.0f && nextUnderTagEqualSpace > 0)
                                    {
                                        numSpace++;
                                    }


                                    break;
                                }


                                next--;

                            }
                            else
                            {
                                if (numObj > 0.0f && nextUnderTagEqualSpace > 0)
                                {
                                    numSpace++;
                                }


                                break;
                            }
                        }

                        Max(numSpace, card->moveValue);

                       float tmp = numSpace * ChipBehavior::kBlockHeight ;
                       predicCube->cube.local_surfaces[C_TOP].leftTop.position.x -= tmp;
                       predicCube->cube.local_surfaces[C_TOP].leftBottom.position.x -= tmp;
                       predicCube->cube.local_surfaces[C_BOTTOM].leftTop.position.x -= tmp;
                       predicCube->cube.local_surfaces[C_BOTTOM].leftBottom.position.x -= tmp;

                       predicCube->isActive = bool(numSpace);



                        break;
                    }

                    case MovableBlock::ArrowDirection::Right:
                    {
                        if ((*move)->mainTrans.pos.y < clickAreaPos.y)
                        {
                            continue;
                        }

                        IndexedPosition indexPos = InGameMapBehavior::GetIndexedPositionBasedInPosition((*move)->mainTrans.pos);

                        int nextTag;
                        int next = 1;
                        float numObj = 0;
                        int nextUnderTagEqualSpace = 0;
                        int nextUnderTag;

                        while (1)
                        {
                            if (indexPos.Y >= 0 && indexPos.Y < InGameMapBehavior::kNumBlockVertical - 1 &&
                                indexPos.X + next >= 0 && indexPos.X + next < InGameMapBehavior::kNumBlockHorizontal - 1)
                            {
                                nextTag = InGameController::currentIndexPosRecords[indexPos.Y][indexPos.X + next];
                                nextUnderTag = InGameController::currentIndexPosRecords[indexPos.Y + 1][indexPos.X + next];

                                if ((nextUnderTag != (int)TagChip::kBlock) && nextUnderTag != (int)TagChip::kMovableBlock)
                                {
                                    nextUnderTagEqualSpace++;
                                }

                                if (nextTag == (int)TagChip::kSpace || (nextTag == 9))
                                {
                                    numSpace++;

                                }
                                else if (nextTag == (int)TagChip::kGoal || nextTag == (int)TagChip::kPlayer || nextTag == (int)TagChip::kKey)
                                {
                                    numObj++;
                                }

                                else if (nextTag == (int)TagChip::kBlock)
                                {
                                    if (numObj > 0.0f && nextUnderTagEqualSpace > 0)
                                    {
                                        numSpace++;
                                    }

                                    break;
                                }


                                next++;

                            }
                            else
                            {
                                if (numObj > 0.0f && nextUnderTagEqualSpace > 0)
                                {
                                    numSpace++;
                                }


                                break;
                            }

                        }


                        Max(numSpace, card->moveValue);

                        float tmp = numSpace * ChipBehavior::kBlockHeight ;
                        predicCube->cube.local_surfaces[C_TOP].rightTop.position.x += tmp;
                        predicCube->cube.local_surfaces[C_TOP].rightBottom.position.x += tmp;
                        predicCube->cube.local_surfaces[C_BOTTOM].rightTop.position.x += tmp;
                        predicCube->cube.local_surfaces[C_BOTTOM].rightBottom.position.x += tmp;

                        predicCube->isActive = bool(numSpace);

                        break;

                    }


                    }


                }

            }

        }
    }

}





void GameScene::UpdateCard()
{
    cardManager->UpdateAll(currentCamera->viewProjectionMat);

    if (!directionalArrow->enabled && !movableField->enabled && (Map1::counter.count == 0 || Map1::counter.count >= 1))
    {
        // カードのホバー・浮き上がりなど
        cardManager->HandleHover(inGameMouse.get(), mainCamera->viewProjectionMat, 40.0f, 0.5f);

        // クリック処理
        cardManager->HandleClick(inGameMouse.get(), mainCamera->viewProjectionMat, 40.0f, 0.0f);
    }

    // 選択されたカードの値をdirectionalArrowに反映（コールバック式にしたい）
    Card* selected = cardManager->GetSelectedCard();
    if (selected)
    {
        directionalArrow->value = selected->GetMoveValue();
    }
}

void GameScene::UpdateMovableBlock()
{
    // MovableBlock全体の処理
   // 方向キーの入力を1回だけ処理
    MovableBlock::ArrowDirection inputDir;
    bool keyTriggered = false;

    // カードが1枚も選択されていない場合は矢印操作をスキップ
    bool cardSelected = false;
    cardTargeted = false;
    for (const auto& card : cardManager->cards) {
        if (card->isUsed) continue; // 使用済みカードは無視
        if (card->isSelected) {
            cardSelected = true;
            cardTargeted = true;
            break;
        }
    }

    if (directionalArrow->enabled || movableField->enabled)
    {
        isPerforming = true;
    }
    else
    {
        isPerforming = false;
    }

    if (NoviceLike::mouseState.rgbButtons[0] && !NoviceLike::preMouseState.rgbButtons[0] &&
        !directionalArrow->enabled && !movableField->enabled)
    {
        //=============================追加＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
        //いったんすべてのブロックのインデックスポジションを記録する
        for (auto& block : inGameController->stages[inGameController->currentStageNo]->movableBlocks)
        {
            //IndexMapRecordsに記録する
            InGameController::RecordIndexPosition(block->mainTrans.GetWorldPos(), (int)TagChip::kMovableBlock);
        }
        //=============================追加

        // x座標を1～10で走査
        if (cardSelected)
        {
            for (int i = 1; i <= 10; ++i)
            {
                // y=21, z=0の位置にマウスが当たったか
                Vector3 targetPosUp = { (float)i * ChipBehavior::kBlockWidth,
                    ((float)InGameMapBehavior::kNumBlockVertical - 11.5f) * ChipBehavior::kBlockHeight, -0.5f };

                if (inGameMouse->CollisionWithMouse3D(targetPosUp, arrowClickAreaManager->collisionRadius, mainCamera->viewProjectionMat))
                {
                    AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::arrow), false, 80);
                    // 矢印の始点をセット
                    directionalArrowPos.x = (float)i * ChipBehavior::kBlockWidth;

                    MovableBlock::ArrowDirection inputDir = MovableBlock::ArrowDirection::Up;



                    directionalArrow->jointCube.trans.defaultQuatenion.z_axis.deltaTheta = 0.0f;
                    directionalArrow->SetPosition({ directionalArrowPos.x,
                        ((float)InGameMapBehavior::kNumBlockVertical - 11.0f) * ChipBehavior::kBlockHeight,  (-0.6f)* ChipBehavior::kBlockHeight });

                    // 領域のリセット
                    movableField->Activate(inputDir, { directionalArrowPos.x ,
                        ((float)InGameMapBehavior::kNumBlockVertical - 11.0f) * ChipBehavior::kBlockHeight, -0.01f });
                    movableField->jointCube.trans.defaultQuatenion.z_axis.deltaTheta = -90.0f;

                    movableField->jointCube.local_surfaces[C_TOP].leftTop.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
                    movableField->jointCube.local_surfaces[C_TOP].leftBottom.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
                    movableField->jointCube.local_surfaces[C_BOTTOM].leftTop.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
                    movableField->jointCube.local_surfaces[C_BOTTOM].leftBottom.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;

                    // 矢印更新・ブロック移動処理
                    directionalArrow->direction = inputDir;
                    directionalArrow->enabled = true;
                    directionalArrow->arrowLength = 0.0f;

                    break; // 最初に当たった位置だけ処理する（複数反応防止）
                }

                // y=31, z=0の位置にマウスが当たったか
                Vector3 targetPosDown = { (float)i * ChipBehavior::kBlockWidth,
                    ((float)InGameMapBehavior::kNumBlockVertical - 0.5f) * ChipBehavior::kBlockHeight, -0.5f };

                if (inGameMouse->CollisionWithMouse3D(targetPosDown, arrowClickAreaManager->collisionRadius, mainCamera->viewProjectionMat))
                {
                    AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::arrow), false, 80);
                    // 矢印の始点をセット
                    directionalArrowPos.x = (float)i * ChipBehavior::kBlockWidth;

                    MovableBlock::ArrowDirection inputDir = MovableBlock::ArrowDirection::Down;
                    directionalArrow->jointCube.trans.defaultQuatenion.z_axis.deltaTheta = 180.0f;
                    directionalArrow->SetPosition({ directionalArrowPos.x,
                        ((float)InGameMapBehavior::kNumBlockVertical - 1.0f) * ChipBehavior::kBlockHeight,  (-0.6f)* ChipBehavior::kBlockHeight });

                    // 領域のリセット
                    movableField->Activate(inputDir, { directionalArrowPos.x ,
                        ((float)InGameMapBehavior::kNumBlockVertical - 11.0f) * ChipBehavior::kBlockHeight, -0.01f });
                    movableField->jointCube.trans.defaultQuatenion.z_axis.deltaTheta = -90.0f;

                    movableField->jointCube.local_surfaces[C_TOP].leftTop.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
                    movableField->jointCube.local_surfaces[C_TOP].leftBottom.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
                    movableField->jointCube.local_surfaces[C_BOTTOM].leftTop.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;
                    movableField->jointCube.local_surfaces[C_BOTTOM].leftBottom.position.x = -(float)(InGameMapBehavior::kNumBlockVertical - 22) * ChipBehavior::kBlockHeight;

                    // 矢印更新・ブロック移動処理
                    directionalArrow->direction = inputDir;
                    directionalArrow->enabled = true;
                    directionalArrow->arrowLength = 0.0f;

                    break; // 最初に当たった位置だけ処理する（複数反応防止）
                }

                // x=1, y=iの位置にマウスが当たったか
                Vector3 targetPosRight = { ChipBehavior::kBlockWidth / 2.0f, ((float)i + 20.0f) * ChipBehavior::kBlockHeight, -0.5f };

                if (inGameMouse->CollisionWithMouse3D(targetPosRight, arrowClickAreaManager->collisionRadius, mainCamera->viewProjectionMat))
                {
                    AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::arrow), false, 80);
                    // 矢印の始点をセット
                    directionalArrowPos.y = ((float)i + 20.0f) * ChipBehavior::kBlockHeight;

                    inputDir = MovableBlock::ArrowDirection::Right;
                    directionalArrow->jointCube.trans.defaultQuatenion.z_axis.deltaTheta = -90.0f;
                    directionalArrow->SetPosition({ 1.0f * ChipBehavior::kBlockWidth, directionalArrowPos.y, -(0.6f) * ChipBehavior::kBlockHeight });
                    // 領域のリセット＋有効化
                    movableField->Activate(inputDir, { 1.0f * ChipBehavior::kBlockWidth, directionalArrowPos.y, -0.01f });
                    movableField->jointCube.trans.defaultQuatenion.z_axis.deltaTheta = 0.0f;

                    movableField->jointCube.local_surfaces[C_TOP].rightTop.position.x = (float)(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;
                    movableField->jointCube.local_surfaces[C_TOP].rightBottom.position.x = (float)(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;
                    movableField->jointCube.local_surfaces[C_BOTTOM].rightTop.position.x = (float)(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;
                    movableField->jointCube.local_surfaces[C_BOTTOM].rightBottom.position.x = (float)(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;

                    // 矢印更新・ブロック移動処理
                    directionalArrow->direction = inputDir;
                    directionalArrow->enabled = true;
                    directionalArrow->arrowLength = 0.0f;

                    break; // 最初に当たった位置だけ処理する（複数反応防止）
                }

                Vector3 targetPosLeft = { (11.5f) * ChipBehavior::kBlockWidth, ((float)i + 20.0f) * ChipBehavior::kBlockHeight, -0.5f };

                if (inGameMouse->CollisionWithMouse3D(targetPosLeft, arrowClickAreaManager->collisionRadius, mainCamera->viewProjectionMat))
                {
                    AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::arrow), false, 80);
                    // 矢印の始点をセット
                    directionalArrowPos.y = ((float)i + 20.0f) * ChipBehavior::kBlockHeight;

                    inputDir = MovableBlock::ArrowDirection::Left;
                    directionalArrow->jointCube.trans.defaultQuatenion.z_axis.deltaTheta = 90.0f;
                    directionalArrow->SetPosition({ ((float)InGameMapBehavior::kNumBlockHorizontal - 1.0f) * ChipBehavior::kBlockHeight,
                        directionalArrowPos.y, (-0.6f) * ChipBehavior::kBlockHeight });

                    // 領域のリセット＋有効化
                    movableField->Activate(inputDir, { ChipBehavior::kBlockWidth, directionalArrowPos.y, -0.01f });
                    movableField->jointCube.trans.defaultQuatenion.z_axis.deltaTheta = 0.0f;

                    movableField->jointCube.local_surfaces[C_TOP].rightTop.position.x = ((float)(InGameMapBehavior::kNumBlockHorizontal - 2)) * ChipBehavior::kBlockWidth;
                    movableField->jointCube.local_surfaces[C_TOP].rightBottom.position.x = (float)(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;
                    movableField->jointCube.local_surfaces[C_BOTTOM].rightTop.position.x = (float)(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;
                    movableField->jointCube.local_surfaces[C_BOTTOM].rightBottom.position.x = (float)(InGameMapBehavior::kNumBlockHorizontal - 2) * ChipBehavior::kBlockWidth;

                    // 矢印更新・ブロック移動処理
                    directionalArrow->direction = inputDir;
                    directionalArrow->enabled = true;
                    directionalArrow->arrowLength = 0.0f;

                    break; // 最初に当たった位置だけ処理する（複数反応防止）
                }
            }

            // ブロックデータと座標を渡す
            directionalArrow->SetMoveData(
                inGameController->stages[inGameController->currentStageNo]->movableBlocks,
                directionalArrowPos
            );
        }
    }

    if (!cardSelected)
    {
        for (auto& arrowClickAreas : arrowClickAreaManager->arrowClickAreas)
        {
            arrowClickAreas->chara.trans.defaultQuatenion.z_axis.deltaTheta -= 1.0f;
            arrowClickAreas->isAuraEnabled = false;
        }

    } else
    {
        for (auto& arrowClickAreas : arrowClickAreaManager->arrowClickAreas)
        {
            arrowClickAreas->chara.trans.defaultQuatenion.z_axis.deltaTheta -= 5.0f;
            arrowClickAreas->isAuraEnabled = true;
        }
    }

    // マウスがホバーしているだけでSEを鳴らす
    bool hoverDetected = false;

    for (int i = 1; i <= 10; ++i)
    {
        struct HoverData
        {
            Vector3 pos;
            MovableBlock::ArrowDirection dir;
            Vector3 arrowStartPos;
        };

        std::vector<HoverData> hoverChecks =
        {
            // 上
            {
                { (float)i * ChipBehavior::kBlockWidth,
                  ((float)InGameMapBehavior::kNumBlockVertical - 11.5f) * ChipBehavior::kBlockHeight, -0.6f },
                MovableBlock::ArrowDirection::Up,
                { (float)i * ChipBehavior::kBlockWidth,
                  ((float)InGameMapBehavior::kNumBlockVertical - 11.0f) * ChipBehavior::kBlockHeight, -0.6f }
            },
            // 下
            {
                { (float)i * ChipBehavior::kBlockWidth,
                  ((float)InGameMapBehavior::kNumBlockVertical - 0.5f) * ChipBehavior::kBlockHeight, -0.6f },
                MovableBlock::ArrowDirection::Down,
                { (float)i * ChipBehavior::kBlockWidth,
                  ((float)InGameMapBehavior::kNumBlockVertical - 1.0f) * ChipBehavior::kBlockHeight, -0.6f }
            },
            // 右
            {
                { ChipBehavior::kBlockWidth / 2.0f,
                  ((float)i + 20.0f) * ChipBehavior::kBlockHeight, -0.6f },
                MovableBlock::ArrowDirection::Right,
                { 1.0f * ChipBehavior::kBlockWidth,
                  ((float)i + 20.0f) * ChipBehavior::kBlockHeight, -0.6f }
            },
            // 左
            {
                { (11.5f) * ChipBehavior::kBlockWidth,
                  ((float)i + 20.0f) * ChipBehavior::kBlockHeight, -0.6f },
                MovableBlock::ArrowDirection::Left,
                { ((float)InGameMapBehavior::kNumBlockHorizontal - 1.0f) * ChipBehavior::kBlockHeight,
                  ((float)i + 20.0f) * ChipBehavior::kBlockHeight, -0.6f }
            },
        };

        // カーソルをホバーしている箇所
        for (const auto& data : hoverChecks)
        {
            if (inGameMouse->CollisionWithMouse3D(data.pos, arrowClickAreaManager->collisionRadius, mainCamera->viewProjectionMat))
            {
                hoverDetected = true;

                // predictiveArrow 表示
                predictiveArrow->SetDirectionAndPosition(data.dir, data.arrowStartPos);
                predictiveField->ActivatePredictive(data.dir, data.arrowStartPos, mainCamera->viewProjectionMat);

                // フィールド中心計算
                Vector3 fieldCenter;
                switch (data.dir)
                {
                case MovableBlock::ArrowDirection::Up:
                case MovableBlock::ArrowDirection::Down:
                {
                    float startX = predictiveField->jointCube.trans.pos.x;
                    float endX = (float)(InGameMapBehavior::kNumBlockHorizontal - 1) * ChipBehavior::kBlockWidth;
                    float centerX = (endX - startX) / 2.0f + startX;
                    float centerY = (((float)InGameMapBehavior::kNumBlockVertical - 20.0f) / 2 + 20.0f) * ChipBehavior::kBlockHeight;
                    fieldCenter = { centerX, centerY, -1.05f };
                    break;
                }
                case MovableBlock::ArrowDirection::Left:
                case MovableBlock::ArrowDirection::Right:
                {
                    float startY = predictiveField->jointCube.trans.pos.y;
                    float endY = (float)(InGameMapBehavior::kNumBlockVertical - 1) * ChipBehavior::kBlockHeight;
                    float centerY = (endY - startY) / 2.0f + startY;
                    float centerX = ((float)InGameMapBehavior::kNumBlockHorizontal / 2.0f) * ChipBehavior::kBlockWidth;
                    fieldCenter = { centerX, centerY, -1.05f };
                    break;
                }
                }

                // 選択されているカードのmoveValueを使用
                Card* selectedCard = cardManager->GetSelectedCard();
                if (selectedCard)
                {
                    int moveValue = (int)selectedCard->GetMoveValue();

                    predictiveCardUI->Set(moveValue, fieldCenter, data.dir);
                    predictiveCardUI->isVisible = true;

                    // SE 再生判定
                    if (!arrowClickAreaManager->isArrowHovered || Distance(data.pos, arrowClickAreaManager->lastHoveredArrowPos) > 1.0f)
                    {
                        AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::anker), false, 30);
                        arrowClickAreaManager->lastHoveredArrowPos = data.pos;
                        arrowClickAreaManager->isArrowHovered = true;
                    }
                }

              

                break;
            }
        }

        if (hoverDetected) break;
    }

    // マウスと衝突しているアンカーだけ ON に
    for (auto& area : arrowClickAreaManager->arrowClickAreas)
    {
        area->auraJointCube.color = { 0, 255, 255, 200 };
    }
    for (auto& area : arrowClickAreaManager->arrowClickAreas)
    {
        if (inGameMouse->CollisionWithMouse3D(area->position, arrowClickAreaManager->collisionRadius, mainCamera->viewProjectionMat))
        {
        
            // オーラの色を黄色に
            area->auraJointCube.color = { 255, 255, 0, 230 };
            area->auraJointCube.SynchronizeColor();

            // 1つだけ点灯させたいなら break;
            break;
        }
    }

    // ホバーしていないなら状態リセット
    if (!hoverDetected)
    {
        arrowClickAreaManager->isArrowHovered = false;

    }
    if (!hoverDetected || directionalArrow->enabled || movableField->enabled)
    {
        predictiveArrow->visible = false;
        predictiveArrow->Initialize();
        predictiveField->enabled = false;
        predictiveField->easingTime = 0;
        predictiveCardUI->isVisible = false;
        predictiveCardUI->easingTime = 0;
    }

    // 矢印の更新
    directionalArrow->Update(mainCamera->viewProjectionMat);
    // 領域の更新
    movableField->Update(mainCamera->viewProjectionMat);
    // 矢印クリックエリアの更新
    arrowClickAreaManager->Update(currentCamera->viewProjectionMat);

    // 予測矢印の更新
    predictiveArrow->Update(mainCamera->viewProjectionMat);

	predictiveCardUI->Update(mainCamera->viewProjectionMat);
}

void GameScene::ShrinkBlocks()
{
    float const dst_commonScale = 0.8f;

    if (cardTargeted)
    {
        shrinkCounter.Add();

        commonScale = Easing::EaseOutElastic(1.0f, dst_commonScale, shrinkCounter.count);
    }

    else
    {
        if (shrinkCounter.count > 0.0f)
        {
            shrinkCounter.count -= shrinkCounter.inv_Time;
        }

        else
        {
            shrinkCounter.count = 0.0f;
        }
        
        commonScale = Easing::EaseOutBack(dst_commonScale,1.0f, (1.0f - shrinkCounter.count));

    }


    for (auto itr = inGameController->stages[inGameController->currentStageNo]->blocks.begin();
        itr != inGameController->stages[inGameController->currentStageNo]->blocks.end(); ++itr)
    {
        IndexedPosition index = InGameMap::GetIndexedPositionBasedInPosition((*itr)->trans.pos);

        if (index.Y == 0 || index.Y == 11)
        {
            if (index.X >= 0 && index.X <= InGameMap::kNumBlockHorizontal - 1)
            {
                (*itr)->trans.scale = { commonScale ,commonScale ,commonScale };

            }
        }

        else if (index.Y > 0 && index.Y < 11)
        {
            if (index.X == 0 || index.X == 11)
            {
                (*itr)->trans.scale = { commonScale ,commonScale ,commonScale };
            }
        }

    }

}

void GameScene::UpdateParticle()
{
    Vector3 pos = { goal->mainTrans.pos.x, goal->mainTrans.pos.y + 1.5f, goal->mainTrans.pos.z };
    goalEmitter->SetPosition(pos);
    if (goal->state == Goal::STATE::CLOSE)
    {
        particleSystem->enableGoalParticles_ = true;
    }
    else
    {
		particleSystem->enableGoalParticles_ = false;
    }
    pos = { key->mainTrans.pos.x, key->mainTrans.pos.y + 1.5f, key->mainTrans.pos.z };
    keyEmitter->SetPosition(pos);
    pos = { key->mainTrans.pos.x, key->mainTrans.pos.y, key->mainTrans.pos.z };
    getKeyEmitter->SetPosition(pos);
	if (key->isActive && !player->isGetKey)
	{
		particleSystem->enableKeyParticles_ = true;
	}
    else
    {
        particleSystem->enableKeyParticles_ = false;
    }
    if (key->isActive && player->isGetKey)
    {
        // まだパーティクルが発生していないときのみトリガー
        if (particleSystem->getKeyParticleTimer_ <= 0) {
            particleSystem->enableGetKeyParticles_ = true;
            particleSystem->getKeyParticleTimer_ = particleSystem->kGetKeyParticleDuration;
        }
    }
    else
    {
        // 条件が成立していなければ強制的にパーティクルを無効化
        particleSystem->enableGetKeyParticles_ = false;
        particleSystem->getKeyParticleTimer_ = 0;
    }

    // タイマーで自動的に非表示
    if (particleSystem->getKeyParticleTimer_ > 0) {
        particleSystem->getKeyParticleTimer_--;

        if (particleSystem->getKeyParticleTimer_ <= 0) {
            particleSystem->enableGetKeyParticles_ = false;
            particleSystem->getKeyParticleTimer_ = 0;
        }
    }

    pos = { goal->mainTrans.pos.x, goal->mainTrans.pos.y, goal->mainTrans.pos.z };
    clearEmitter->SetPosition(pos);
    if (goal->state == Goal::STATE::OPEN)
    {
        particleSystem->enableClearParticles_ = true;
    }
    else
    {
        particleSystem->enableClearParticles_ = false;
    }

	pos = { player->mainTrans.pos.x, player->mainTrans.pos.y + 2.0f, player->mainTrans.pos.z };
    playerIdleEmitter->SetPosition(pos);
   
}