#include "InGameMouse.h"
#include "../../MyLib/Utilities/EasingFuctionStructures/EasingFunctions.h"

void InGameMouse::Update(Matrix4 vpMat_, Vector3 cameraPos_)
{
	counter.Add();

	if (counter.IsEnd())
	{
		inc++;
	}

	////マウスの座標を取得
	preMousePos = { (float)NoviceLike::mousePos.x ,(float)NoviceLike::mousePos.y };
	NoviceLike::SetMousePos(&NoviceLike::mousePos);
	ScreenToClient(pDxCommon->hwnd, &NoviceLike::mousePos);

	if (counter.count > 0.5f)
	{
		float n = (counter.count - 0.5f)*2.0f;
		mouseSprite.trans.defaultQuatenion.z_axis.deltaTheta =
			Easing::EaseOutCubic((inc - 1.0f) * degree, inc * degree, n);

	}


	mouseSprite.trans.pos = { (float)NoviceLike::mousePos.x ,(float)NoviceLike::mousePos.y,0.0f };
	mouseSprite.Update();

}

void InGameMouse::Debug()
{

}

void InGameMouse::Draw(Matrix4 vpMat_)
{
	mouseSprite.Draw();
}

InGameMouse::InGameMouse()
{
	mouseSprite.InitializeSprite({}, 32, 32, (int)TagTexture::kCursorImage);
	counter.Initialize(60);
}