#pragma once
#include <map>


//テクスチャのタグ
//textureHandleに代入する
enum class TagTexture
{

    kWhite2x2,
    kEye,
    kCursorImage,
    kMetalic,
    kStone,
    kNumbers,
    kTestGoal,
    kPlayer,
    kN_nani,
    kGear,
    kResetButton,//リセットボタン
    kTestKey,
    kCardNumbers,//カードの数字
    kCardBack,//カードの裏側

    kMovableBlock,//移動ブロック
    kNormalBlock,//ノーマルブロック
    kPredictiveBlock,//予測ブロック


    kParticle0,
    kBackToTitleButton,
    kStageSelectButton,
    kReturnToGameButton,
    kRKey,//Rのスプライト
    kSelectStar,
    kStageFont,

    kA,
    kD,
    kW,
    kGoalString,
    kKeyString,
    kClearString,
    kCursor,
    kPress,
    kFrame,//フレーム

};

struct MapTextures
{
    std::map<TagTexture, uint16_t> mapContainer;

    MapTextures();

private:
    void LoadTextureFiles();
};

