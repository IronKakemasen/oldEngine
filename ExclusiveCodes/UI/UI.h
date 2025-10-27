#pragma once


#include"Button/Button.h"
#include"../Fade/Fade.h"

struct  InGameMouse;
class InGameController;


class UI {
public:

    enum Secene {
        TITLE,
        SELECT
    };
    UI(InGameController* inGameController);
    void Initialize();
    void Update(InGameMouse& inGameMouse, const Matrix4& vpMat);
    void Draw();
    //歯車の更新処理
    void GearRotateUpdate(const float& startTheta, const float& endTheta);
    //ポーズを切り替える
    void SwitchPauze();
    //時間のアップデート
    void TimerUpdate(float& timer);
    //Rボタンの更新処理
    void ResetButtonUpdate();

    //フェード
    Fade fade;
    //ボタンを描画しない
    bool isButtonActive = false;
    bool isShiftSelectScene = false;
    Secene sceneTag = TITLE;

    std::vector<std::unique_ptr<Button>> buttons;
    std::unique_ptr<Button> gearButton = {nullptr};
    std::unique_ptr<Button> resetButton = { nullptr };
    static inline bool isPause = false;
    static inline float moveTimer = 0.0f;
    static inline float resetTimer = 0.0f;
    InGameController* inGameController = nullptr;
    Sprite rSprite;
};
