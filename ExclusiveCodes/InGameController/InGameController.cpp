#include "InGameController.h"
#include "../../ExclusiveCodes/GameObjects/Player/Player.h"
#include "../../ExclusiveCodes/UI/UI.h"
#include "../../ExclusiveCodes/GameObjects/Goal/Goal.h"
#include "../../ExclusiveCodes/GameObjects/Key/Key.h"
#include "../../ExclusiveCodes/GameObjects/DirectionalArrow/DirectionalArrow.h"
#include "../../ExclusiveCodes/GameObjects/MovableField/MovableField.h"
#include "../../ExclusiveCodes/Camera/GameCamera/GameCamera.h"
#include "../InGameMouse/InGameMouse.h"
#include "../../ExclusiveCodes/GameObjects/Card/CardManager.h"
#include "../../ExclusiveCodes/GameObjects/DirectionalArrow/ArrowClickAreaManager.h"
#include "../Audio/AudioPlayer.h"
#include "../Audio/AudioHandle.h"
#include "../GameObjects/Card/PredictiveCardUI.h"
#include "../../MyLib/Utilities/SceneStructures/SceneBehavior.h"
#include "../Map/Map1/Map1.h"



void InGameController::Update(InGameMouse& inGameMouse, const Matrix4& vpMat)
{
    //気にする必要はない
    ResetIndexPosRecords();

    switch (state)
    {
    case InGameController::kEnter:

        //ステージセレクトから侵入したときのみ
        if (haveStartdeFromSelect)
        {
            float const inv_ = 1.0f / 30.0f;
            if (enterTimer.count == 0.0f) {
                ui->fade.ChangStatus(ui->fade.kFadeOut, inv_);
            }

            enterTimer.Add();



            if (enterTimer.IsEnd())
            {
                state = InGameController::kPlayable;
                //ゴールとキーをプレイできるように
                key->isPlayable = true;
                goal->isPlayable = true;
                haveStartdeFromSelect = false;
                gameCamera->status = GameCamera::Status::kFollow;
            }
        }

        else
        {
            if (transitionTimer == 0.0f) {
                ui->fade.ChangStatus(ui->fade.kFadeOut);
            }

            //deltaTimeで行う
            transitionTimer += 1.0f / 60.0f;

            if (transitionTimer > 1.0f) {
                //タイマーが一定時間経過したときプレイ出来るようにする
                state = InGameController::kPlayable;

            }
            else if (transitionTimer > 0.5f) {
                //ゴールとキーをプレイできるように
                key->isPlayable = true;
                goal->isPlayable = true;
            }


        }

        break;
    case InGameController::kPlayable:

        if (player->state == Player::STATE::GOAL) {

            AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::goal), false, 30);
            isClear = true;
            //プレイヤーがゴールしたらリザルトにうつる
            state = InGameController::kResult;
            //遷移タイマーの初期化
            transitionTimer = 0.0f;
        }

        //ゲームオーバー
        if (player->state == Player::STATE::DEAD) {

            if (player->stateCount[Player::STATE::DEAD] == 1.0f) {
                AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::reset), false, 100);

                ResetGame();
            }
       
        }

        if (ui->isPause) {
            //UIからPoseフラグを受け取ったポーズにする
            state = InGameController::kPause;
        }


        break;
    case InGameController::kPause:

        //ポーズから抜けたら
        if (!ui->isPause) {
            //UIからPoseフラグを受け取ったポーズにする
            state = InGameController::kPlayable;
        }

        //セレクトシーンに戻る

        break;

    case InGameController::kResult:

        gameCamera->status = GameCamera::Status::kZoomIn;

        //リザルト
        resultTimer.Add();

        clearSprite.Update();
        clearFrameSprite.Update();
        
        if (resultTimer.count >= 0.4f&& resultTimer.count < 0.7f)
        {
            float n = (resultTimer.count - 0.4f) * 3.3333333f;
            //clearSprite.trans.scale = Easing::EaseOutElastic(Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.75f,0.75f,0.75f }, n);
            clearSprite.trans.uvScale.u = Easing::EaseOutExpo(0.0f, 0.95f, n);
            clearSprite.trans.uvScale.v = Easing::EaseOutExpo(0.0f, 0.95f, n);
            clearSprite.color.z = Easing::Lerp(255.0f, 100.0f, n);

            if (!goalSoundPlayed)
            {
                AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::clear), false, 100);
                goalSoundPlayed = true;
            }
        }

        else
        {
            float const uvSpeed = 0.001f;
            clearSprite.trans.uvPos.u += uvSpeed;
        }

        if (resultTimer.count >= 1.0f)
        {
            //なんらかのアクションがあったらフェードインをオンにする
            if (!owari)
            {
                if (NoviceLike::IsKeyTrigger(DIK_A) || NoviceLike::IsKeyTrigger(DIK_W) || NoviceLike::IsKeyTrigger(DIK_D) ||
                    NoviceLike::IsKeyTrigger(DIK_S) || NoviceLike::mouseState.rgbButtons[0] || NoviceLike::IsKeyTrigger(DIK_SPACE))
                {
                    isCleared[currentStageNo] = true;

                    ui->fade.ChangStatus(ui->fade.kFadeIn);
					AudioPlayer::GetInstance().PlayAudio(AudioHandle::Get(AudioID::decision), false, 100);
                    owari = true;
                }
            }

            //もはや終わり
            else if (owari && ui->fade.timer_ >= 1.0f)
            {
                resultTimer.IsEnd();
                SceneController::ReceiveSignal(SceneController::TagScene::kSelect);
                owari = false;
                clearSprite.trans.uvPos.u = 0.385f;
            }
        }




        //if (transitionTimer < 1.0f) {
        //    //deltaTimeで行う
        //    transitionTimer += 1.0f / 60.0f;
        //} else {
        //    //遷移タイマーの初期化

        //    ++currentStageNo;
        //ResetGame();

        break;

    };

    ui->Update(inGameMouse, vpMat);

    //次のステージに行く
    //タイトルに戻る

}
    

void InGameController::ResetGame()
{
    reinterpret_cast<Map1*>(stages[1])->b = true;
    Initialize();
};


void InGameController::Initialize()
{
    if (gameCamera) {
        gameCamera->Initialize();
    }

    if (haveStartdeFromSelect)
    {
        gameCamera->status = GameCamera::Status::kZoomOut;
    }

    else
    {
        gameCamera->status = GameCamera::Status::kFollow;
    }

    stages[currentStageNo]->Initialize();

    //移動可能領域の初期化
    movableField->Initialize();
    //矢印の初期化
    directionalArrow->Initialize(movableField);


    // =========================================================================

    cardManager->cards.clear();

    cardManager->Initialize(
        stages[currentStageNo]->cardValues,
        -2.5f,
        (InGameMapBehavior::kNumBlockVertical - 6) * ChipBehavior::kBlockHeight);
    // =========================================================================

    arrowClickAreaManager->Initialize();

    // movableField にイージング終了時コールバックを設定
    movableField->onEasingFinished = [this]()
    {
        cardManager->UseSelectedCard();
    };


    //クリアフラグを偽に
    isClear = false;
    //ゲームオーバーを偽に
    isGameOver = false;
    //トランジションタイマーを0
    transitionTimer = 0.0f;

    goalSoundPlayed = false;

    //現在ステージに合わせてプレイヤー、キー、ゴールの座標を設定する
    SetObjectPosition();

    //GameObjectの初期化
    player->Initialize();
    player->mainTrans.SetWorldMatBasedQuaternion();
    goal->Initialize();
    goal->mainTrans.SetWorldMatBasedQuaternion();
    key->Initialize();
    key->mainTrans.SetWorldMatBasedQuaternion();

    ui->Initialize();

    predictiveCardUI->Initialize();
    //入場時の状態
    state = InGameController::State::kEnter;


};


void InGameController::Draw()
{
    if (state == State::kResult && resultTimer.count >= 0.4f)
    {
        clearSprite.Draw();
        clearFrameSprite.Draw();
    }

    ui->Draw();

}

InGameController::InGameController() {

    //現在ステージ


    //UIのインスタンスを生成
    ui = std::make_unique<UI>(this);

    enterTimer.Initialize(120.0f);
    resultTimer.Initialize(200.0f);

    clearSprite.InitializeSprite({ 640.0f,200 ,0.0f },800 ,256 * 0.75f,(int)TagTexture::kClearString);
    clearFrameSprite.InitializeSprite({ 640.0f,200 ,0.0f }, 800 , 256 , (int)TagTexture::kFrame);

    clearSprite.trans.uvPos.u = 0.385f;
};

InGameController::~InGameController()
{

};

void InGameController::RegisterMap(InGameMap* map_)
{
    //マップ０の追加
    auto& stage = stages.emplace_back(map_);
}


void InGameController::GetClearSignal()
{
    isClear = true;

}

void InGameController::RecordIndexPosition(Vector3 dst_world_, int tagChip_)
{
    IndexedPosition indexPos = InGameMapBehavior::GetIndexedPositionBasedInPosition(dst_world_);
    currentIndexPosRecords[indexPos.Y][indexPos.X] = tagChip_;
}

void InGameController::ResetIndexPosRecords()
{
    int y = 0;

    for (auto& row : stages[currentStageNo]->tagData)
    {
        int x = 0;

        for (auto& element : row)
        {
            if (element == (int)TagChip::kBlock)
            {
                currentIndexPosRecords[y][x] = (int)element;
            }

            else
            {
                currentIndexPosRecords[y][x] = (int)TagChip::kSpace;

            }

            ++x;
        }

        ++y;
    }
}

void InGameController::SetObjectPosition()
{
    int y = 0;

    for (auto& row : stages[currentStageNo]->tagData)
    {
        int x = 0;
        for (auto& element : row)
        {
            if (element == (int)TagChip::kPlayer)
            {
                player->mainTrans.pos = InGameMap::GetPositionBasedInIndex(y, x);
            }

            else if (element == (int)TagChip::kKey)
            {
                key->mainTrans.pos = InGameMap::GetPositionBasedInIndex(y, x);
            }

            else if (element == (int)TagChip::kGoal)
            {
                goal->mainTrans.pos = InGameMap::GetPositionBasedInIndex(y, x);
            }

            ++x;
        }

        y++;
    }

}

std::string InGameController::GetCurrentStateInString()
{

    std::string ret_state[(int)State::kMaxState]
    {
        "kEnter","kPlayable","kPose","kResult"
    };

    return ret_state[(int)state];
}
