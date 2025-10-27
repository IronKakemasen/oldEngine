#include "MapTextures.h"
#include "../../NoviceLike/NoviceLike.h"

void MapTextures::LoadTextureFiles()
{
    mapContainer[TagTexture::kWhite2x2] = NoviceLike::LoadTextureFile("Resource/TestImage/white2x2.png");
    mapContainer[TagTexture::kEye] = NoviceLike::LoadTextureFile("Resource/TestImage/eye.png");
    mapContainer[TagTexture::kCursorImage] = NoviceLike::LoadTextureFile("Resource/TestImage/cursorImage.png");
    mapContainer[TagTexture::kMetalic] = NoviceLike::LoadTextureFile("Resource/TestImage/metalic.png");
    mapContainer[TagTexture::kStone] = NoviceLike::LoadTextureFile("Resource/TestImage/stone.png");
    mapContainer[TagTexture::kNumbers] = NoviceLike::LoadTextureFile("Resource/Images/numbers160x160.png");
    mapContainer[TagTexture::kTestGoal] = NoviceLike::LoadTextureFile("Resource/TestImage/goal128x128.png");
    mapContainer[TagTexture::kPlayer] = NoviceLike::LoadTextureFile("Resource/TestImage/player128x128.png");
    mapContainer[TagTexture::kN_nani] = NoviceLike::LoadTextureFile("Resource/TestImage/nNani.png");
    mapContainer[TagTexture::kGear] = NoviceLike::LoadTextureFile("Resource/Images/gear128x128.png");
    mapContainer[TagTexture::kResetButton] = NoviceLike::LoadTextureFile("Resource/Images/reset128x128.png");
    mapContainer[TagTexture::kTestKey] = NoviceLike::LoadTextureFile("Resource/TestImage/key128x128.png");
    mapContainer[TagTexture::kCardNumbers] = NoviceLike::LoadTextureFile("Resource/Images/card_numbers_3456x512.png");
    mapContainer[TagTexture::kCardBack] = NoviceLike::LoadTextureFile("Resource/Images/card_back_394x512.png");

    mapContainer[TagTexture::kMovableBlock] = NoviceLike::LoadTextureFile("Resource/Images/movableBlock128x128.png");
    mapContainer[TagTexture::kNormalBlock] = NoviceLike::LoadTextureFile("Resource/Images/normalBlock128x128.png");
    mapContainer[TagTexture::kPredictiveBlock] = NoviceLike::LoadTextureFile("Resource/Images/predictiveBlock128x128.png");

    
    mapContainer[TagTexture::kParticle0] = NoviceLike::LoadTextureFile("Resource/Images/particle0.png");

    //文字ボタン
    mapContainer[TagTexture::kBackToTitleButton] = NoviceLike::LoadTextureFile("Resource/Images/backToTitle256x64.png");
    mapContainer[TagTexture::kStageSelectButton] = NoviceLike::LoadTextureFile("Resource/Images/stageSelect256x64.png");
    mapContainer[TagTexture::kReturnToGameButton] = NoviceLike::LoadTextureFile("Resource/Images/returnToGame256x64.png");
    mapContainer[TagTexture::kRKey] = NoviceLike::LoadTextureFile("Resource/Images/r64x64.png");
    mapContainer[TagTexture::kSelectStar] = NoviceLike::LoadTextureFile("Resource/Images/selectStar.png");
    mapContainer[TagTexture::kStageFont] = NoviceLike::LoadTextureFile("Resource/Images/stageFont.png");


    mapContainer[TagTexture::kA] = NoviceLike::LoadTextureFile("Resource/Images/a64x64.png");
    mapContainer[TagTexture::kD] = NoviceLike::LoadTextureFile("Resource/Images/d64x64.png");
    mapContainer[TagTexture::kW] = NoviceLike::LoadTextureFile("Resource/Images/w64x64.png");

    mapContainer[TagTexture::kGoalString] = NoviceLike::LoadTextureFile("Resource/Images/goal128x64.png");
    mapContainer[TagTexture::kKeyString] = NoviceLike::LoadTextureFile("Resource/Images/key128x64.png");

    mapContainer[TagTexture::kClearString] = NoviceLike::LoadTextureFile("Resource/Images/clear800x256.png");
    mapContainer[TagTexture::kCursorImage] = NoviceLike::LoadTextureFile("Resource/Images/mouse394x512.png");
    mapContainer[TagTexture::kPress] = NoviceLike::LoadTextureFile("Resource/Images/mouseClick394x512.png");
    mapContainer[TagTexture::kFrame] = NoviceLike::LoadTextureFile("Resource/Images/frame800x256.png");

}

MapTextures::MapTextures()
{
    LoadTextureFiles();
}

