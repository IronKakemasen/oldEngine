#include"UI.h"
#include "../../MyLib/Utilities/EasingFuctionStructures/EasingFunctions.h"
#include "../../MyLib/Utilities/MouseStructures/MouseBehavior.h"
#include "../InGameMouse/InGameMouse.h"
#include "../Audio/AudioHandle.h"
#include "../Audio/AudioPlayer.h"
#include"../InGameController/InGameController.h"
#include "../../MyLib/Utilities/SceneStructures/SceneBehavior.h"


UI::UI(InGameController* inGameController) {

    this->inGameController = inGameController;

    //フェードの初期化 UIの中にfedeを作ってしまってごめん
    fade.textureHandle_ = (int)TagTexture::kWhite2x2;
    fade.endAlpha_ = 196.0f;

    for (uint32_t i = 0; i < 3; ++i) {
        //ボタンのインスタンスを生成
        Vector3 position = { 640.0f,256.0f + 96.0f * i,0.0f };
        auto& button = buttons.emplace_back(std::make_unique<Button>(256.0f, 64.0f, position, false));
    }

    //ボタンごとに関数を設定する
    for (uint32_t i = 0; i < buttons.size(); ++i) {

        auto& button = buttons[i];
        if ((i) == 0) {
            //タイトルに戻る
            button->SetTextureHandle((int)TagTexture::kBackToTitleButton);
            button->onTrigger = [inGameController, this]() {
                AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::decision), false, 100);

                isPause = false;
                isShiftSelectScene = true;
                sceneTag = TITLE;
                //移動タイマーを初期化
                moveTimer = 0.0f;
                fade.ChangStatus(fade.kFadeIn);
                fade.endAlpha_ = 255.0f;
                };

        }

        //ここをステージセレクトに書き換える
        if ((i) == 1)
        {

            //セレクト
            button->SetTextureHandle((int)TagTexture::kStageSelectButton);
            button->onTrigger = [this]()
                {
                    AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::decision), false, 100);
                    isPause = false;
                    isShiftSelectScene = true;
                    sceneTag = SELECT;
                    //移動タイマーを初期化
                    moveTimer = 0.0f;
                    fade.ChangStatus(fade.kFadeIn);
                    fade.endAlpha_ = 255.0f;
           
                };
        }

        if ((i) == 2) {
            button->SetTextureHandle((int)TagTexture::kReturnToGameButton);
            button->onTrigger = [this]() {
                AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::decision), false, 100);
                isPause = false;
                //移動タイマーを初期化
                moveTimer = 0.0f;
                fade.ChangStatus(fade.kFadeOut);
                fade.endAlpha_ = 128.0f;
                };
        }

    }

    Vector3 gearButtonPos = { 1280.0f - 64.0f, 64.0f,0.0f };
    gearButton = std::make_unique<Button>(64.0f, 64.0f, gearButtonPos, true);
    gearButton->SetTextureHandle((int)TagTexture::kGear);
    gearButton->baseColor = { 255,255,255,128 };
    gearButton->hoverColor = { 255,255,255,255 };
    gearButton->baseColor = { 255,255,255,255 };

    gearButton->onTrigger = [this]() {

        //ポーズを切り替える
        SwitchPauze();
        //音を鳴らす
        AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::pause), false, 100);
        fade.endAlpha_ = 128.0f;

        if (isPause) {
            //ポーズだった時FadeIn
            fade.ChangStatus(fade.kFadeIn);    
            //ボタンを描画する
            isButtonActive = true;

            for (auto& button : buttons) {
                button->Initialize();
            }
        } else {
            //ポーズだった時FadeOut
            fade.ChangStatus(fade.kFadeOut);
            //ボタンを描画しない
            isButtonActive = false;
        }

        };


    Vector3 resetButtonPos = { 1280.0f - 156.0f, 64.0f, 0.0f };
    resetButton = std::make_unique<Button>(64.0f, 64.0f, resetButtonPos, true);
    resetButton->SetTextureHandle((int)TagTexture::kResetButton);
    resetButton->baseColor = { 255,255,255,128 };
    resetButton->hoverColor = { 255,255,255,255 };
    resetButton->baseColor = { 255,255,255,255 };

    resetButton->onTrigger = [inGameController, this]() {

        if (inGameController->state == InGameController::State::kPlayable) {
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::reset), false, 50);

            inGameController->ResetGame();
            resetTimer = 0.0f;
            fade.endAlpha_ = 128.0f;

        }

        };

    rSprite.InitializeSprite({ 1280 - 156,64,0 }, 32.0f, 32.0f, (int)TagTexture::kRKey);
}

void UI::Initialize() {

    ////ポーズを偽に
    isPause = false;
    isShiftSelectScene = false;
    //移動タイマーを初期化
    moveTimer = 0.0f;


};

//時間のアップデート
void  UI::TimerUpdate(float& timer) {

    if (timer == 1.0f) { return; }

    timer += 1.0f / 60.0f;

    if (timer > 1.0f) {

        timer = 1.0f;

    }

};

//Rボタンの更新処理
void UI::ResetButtonUpdate() {

    rSprite.Update();

    resetButton->sprite.trans.defaultQuatenion.z_axis.deltaTheta = Easing::EaseOutCubic(0.0f, 360.0f, resetTimer);
    resetButton->sprite.trans.SetWorldMatBasedQuaternion();

    TimerUpdate(resetTimer);
};


//ギアの更新処理
void UI::GearRotateUpdate(const float& startTheta, const float& endTheta) {

    gearButton->sprite.trans.defaultQuatenion.z_axis.deltaTheta = Easing::EaseOutBack(startTheta, endTheta, moveTimer);
    gearButton->sprite.trans.SetWorldMatBasedQuaternion();

};


//ポーズを切り替える
void  UI::SwitchPauze() {
    //ポーズに移行する
    isPause = isPause ? false : true;
    //移動タイマーの初期化
    moveTimer = 0.0f;
};

//
void UI::Update(InGameMouse& inGameMouse, const Matrix4& vpMat) {

    //ギアボタンの更新処理
    gearButton->Update(inGameMouse, vpMat);
    //リセットボタン
    resetButton->Update(inGameMouse, vpMat);

    if (inGameController->state == InGameController::State::kPlayable) {
        if (NoviceLike::IsKeyTrigger(DIK_R)) {
            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::reset), false, 50);
            inGameController->ResetGame();
            resetTimer = 0.0f;
        }
}


    //リセットボタンの更新処理
    ResetButtonUpdate();


    //タイマーを更新　ここをミヤジさんのやつに切り替えたい
    TimerUpdate(moveTimer);

    if (isButtonActive) {

        for (auto& button : buttons) {

            button->Update(inGameMouse, vpMat);


        }

    }

    if (isPause) {
        //ポーズの時
        GearRotateUpdate(0.0f, 180.0f);



    } else {
        GearRotateUpdate(180.0f, 0.0f);

        if (isButtonActive) {

            if (moveTimer > 0.5f) {

                //ボタンを描画しない
                isButtonActive = false;

                if (isShiftSelectScene) {
                    isShiftSelectScene = false;
                    if (sceneTag == TITLE) {
                        SceneController::ReceiveSignal(SceneController::TagScene::kTitle);
                    }else if(sceneTag == SELECT){
                        SceneController::ReceiveSignal(SceneController::TagScene::kSelect);
                    }
             

                }


            }

        }


    }



    //フェードの更新処理
    fade.Update();

};

void UI::Draw() {

    //フェードを描画
    fade.Draw();

    if (isButtonActive) {
        //ボタンを描画する
        for (auto& button : buttons) {
            button->Draw();
        }
    }


    //ギアのボタン描画
    gearButton->Draw();
    //リセットボタンの描画処理
    resetButton->Draw();
    //RSpeite
    rSprite.Draw();

};