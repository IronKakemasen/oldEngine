#pragma once

#include "../Map/InGameMapBehavior.h"
#include "../../MyLib/Utilities/CollisionDetectionsStructures/CollisionDetection.h"
#include<memory>

class Player;
class UI;
class Matrix4;
class Goal;
class Key;

class GameCamera;

class ObjectManager;
struct  InGameMouse;

class MovableField;
class DirectionalArrow;
class CardManager;
class ArrowClickAreaManager;

class PredictiveCardUI;


///ゲームの審判
class InGameController
{
public:

    enum State
    {
        kEnter,
        kPlayable,
        kPause,
        kResult,
        kMaxState,
    };

    //現在のステージ番号
    static inline int currentStageNo = 0;
    //ステージの合計個数
    static int const kSumStage = 15;
    //すべてのマップのアドレスをいれるコンテナ
    static inline std::vector<InGameMap*> stages;

    static inline int currentIndexPosRecords[InGameMapBehavior::kNumBlockVertical][InGameMapBehavior::kNumBlockHorizontal];
    
    static inline bool isCleared[kSumStage];


    Player* player = nullptr;
    Goal* goal = nullptr;
    Key* key = nullptr;
    GameCamera* gameCamera = nullptr;

    MovableField* movableField = nullptr;
    DirectionalArrow* directionalArrow = nullptr;

    CardManager* cardManager = nullptr;
    ArrowClickAreaManager* arrowClickAreaManager = nullptr;

	PredictiveCardUI* predictiveCardUI = nullptr; 

    static inline  State state = State::kEnter;
    //遷移タイマー
    float transitionTimer = 0.0f;
    //エンタータイマー
    CounterAndInvTime enterTimer;
    //リザルトタイマー
    CounterAndInvTime resultTimer;
    Sprite clearSprite;
    Sprite clearFrameSprite;

    bool owari = false;
    bool goalSoundPlayed = false;

    static inline bool isClear = false;
    //ゲームオーバー
    static inline bool isGameOver = false;

    //ゲームセレクトから来ました
    static inline bool haveStartdeFromSelect = true;


    //uiのユニークポインタ
    std::unique_ptr<UI> ui;

    void Update(InGameMouse& inGameMouse, const Matrix4& vpMat);
    //ゲームをリセットする
    void ResetGame();
    void Initialize();
    //現在のステージナンバーに応じてプレイヤーやギミックの初期化を行う
    void SetObjectPosition();

    void RegisterMap(InGameMap* map_);
    void Draw();
    static void GetClearSignal();
    std::string GetCurrentStateInString();

    //インデックスマップをそのステージのマップのブロックとそれ以外で初期化する
    void ResetIndexPosRecords();
    //現在ステージのインデックスマップに自身のインデックスを登録する
    static void RecordIndexPosition(Vector3 dst_world_, int tagChip_);


    InGameController();

    ~InGameController();
};

