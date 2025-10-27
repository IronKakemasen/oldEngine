#include "./MyLib/Utilities/EssentialInstanceIncludes.h"
#include "./MyLib/Utilities/GlobalVariablesStructures/GlobalVariables.h"
#include "ExclusiveCodes/Scenes/TitleScene/TitleScene.h"
#include "ExclusiveCodes/Scenes/GameScene/GameScene.h"
#include "ExclusiveCodes/Scenes/EndScene/EndScene.h"
#include "ExclusiveCodes/Scenes/ModelDebugScene/ModelDebugScene.h"
#include "ExclusiveCodes/Scenes/StageSelectScene/StageSelectScene.h"

#include "ExclusiveCodes/Audio/AudioManager.h"
#include "ExclusiveCodes/Audio/AudioHandle.h"
#include "ExclusiveCodes/InGameMouse/InGameMouse.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    //＜DebugLayerのスイッチ＞
    EngineCoreInstances::dxCommon.debugLayerSwitch = false;
    //＜ポインタ破壊などを検知するためのもの＞
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_CHECK_ALWAYS_DF);


    // ↓↓↓↓↓↓↓↓↓↓↓↓[ 書き換え禁止 ]↓↓↓↓↓↓↓↓↓↓↓↓
    pDxCommon = &EngineCoreInstances::dxCommon;
    pLeakChecker = &EngineCoreInstances::leakChecker;
    //エンジンの初期化
    NoviceLike::Initialize((UINT)WINDOW_W, (UINT)WINDOW_H, L"2007_シフトトゥ");
    //[保存ファイル読みこみ]

    GlobalVariables::GetInstance()->LoadFiles();
    //不可欠アセットの初期化
    EssentialInstanceInitialize();
    //ランダムシードの決定
    srand(unsigned int(time(NULL)));
    // AudioManagerの初期化
    AudioManager::GetInstance().Initialize();
    // AudioHandleの初期化
    AudioHandle::Initialize();
    // ↑↑↑↑↑↑↑↑↑[ 書き換え禁止 ]↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑


    // ↓↓↓↓↓↓↓↓↓↓↓↓[ インスタンス ]↓↓↓↓↓↓↓↓↓↓↓↓

    // < SCENE >
    SceneBehavior* currentScene = nullptr;

    //ゲームシーンのインスタンス化
    std::unique_ptr<GameScene> gameScene(new GameScene(ResourceInstances::mapTextures.get(),
        ResourceInstances::mapModels.get()));

    //エンドシーンのインスタンス化
    std::unique_ptr<EndScene> endScene(new EndScene(ResourceInstances::mapTextures.get(),
        ResourceInstances::mapModels.get()));
    //モデルデバッグシーンのインスタンス化
    std::unique_ptr<ModelDebugScene> modelDebugScene(new ModelDebugScene(ResourceInstances::mapTextures.get(),
        ResourceInstances::mapModels.get()));
    //セレクトシーンのインスタンス化
    std::unique_ptr<StageSelectScene> stageSelectScene(new StageSelectScene(ResourceInstances::mapTextures.get(),
        ResourceInstances::mapModels.get()));
    //タイトルシーンのインスタンス化
    std::unique_ptr<TitleScene> titleScene(new TitleScene(ResourceInstances::mapTextures.get(),
        ResourceInstances::mapModels.get()));

    //ゲーム内共通マウス
    InGameMouse mouse;
    Matrix4 tmpMat;
    Vector3 tmpVec;

    
    ShowCursor(false);
    
    // ↑↑↑↑↑↑↑↑↑↑↑↑[ インスタンス ]↑↑↑↑↑↑↑↑↑↑↑↑

    //シーンの代入
   //currentScene = gameScene.get();
   //currentScene = gameScene.get();
   //currentScene = stageSelectScene.get();
   /* currentScene = modelDebugScene.get();*/
   //currentScene = titleScene.get();
   currentScene = stageSelectScene.get();

#ifdef _DEBUG
   currentScene = stageSelectScene.get();
#endif // _DEBUG




    MSG msg{};
    //ウィンドウのxが押されるまでループ
    while (msg.message != WM_QUIT)
    {
        //Windowにメッセージが来てたら最優先で処理させる
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //ゲームの処理
        else
        {
            NoviceLike::BeginFrame(NoviceLike::keys, &NoviceLike::mouseState);

            //カレントシーンの更新
            currentScene->Update();
            mouse.Update(tmpMat, tmpVec);

            //デバ

#ifdef _DEBUG
            currentScene->Debug();
#endif // !_DEBUG

            //描画処理
            currentScene->Render();
            mouse.Draw(tmpMat);


            currentScene = SceneController::GetInstance()->ChangeScene(currentScene);

            NoviceLike::EndFrame();

        }


    }

    // 音声データの開放
    AudioManager::GetInstance().Finalize();
    NoviceLike::Finalize();

    return 0;
}

