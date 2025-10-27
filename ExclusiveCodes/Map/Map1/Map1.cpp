#include "Map1.h"
#include"../../InGameController/InGameController.h"
#include "../../InGameMouse/InGameMouse.h"


void Map1::Update(Matrix4 vpMat_)
{
	Vector3 const dstPos = { -1.736f,31.1265068f,-1.6f };
	Vector3 const dstPos2 = { -3.26232581f,31.1265068f,-1.5f };
	Vector3 const dstPos3 = InGameMap::GetPositionBasedInIndex(0, 10);

	static int c = 0;

	static bool onlyOnce = true;

	//1きり
	if (counter.count < 1.0f)
	{
		counter.Add();
		if (counter.count >= 0.75f)
		{
			float n = (counter.count - 0.75f) * 4.0f;

			mask.color.w = Easing::EaseOutBack(0.0f, 220.0f, n);
		}
	}

	else
	{
		if (onlyOnce)
		{

			if (mouse->CollisionWithMouse3D(dstPos, 40, vpMat_))
			{
				if (NoviceLike::mouseState.rgbButtons[0] && !NoviceLike::preMouseState.rgbButtons[0])
				{
					onlyOnce = false;
					mask.color.w = 0.0f;
				}
			}

			else if (mouse->CollisionWithMouse3D(dstPos2, 40, vpMat_))
			{
				if (NoviceLike::mouseState.rgbButtons[0] && !NoviceLike::preMouseState.rgbButtons[0])
				{
					onlyOnce = false;
					mask.color.w = 0.0f;
				}
			}
		}
	}

	if (b)
	{
		if (mouse->CollisionWithMouse3D(dstPos3, 30, vpMat_))
		{
			if (NoviceLike::mouseState.rgbButtons[0] && !NoviceLike::preMouseState.rgbButtons[0])
			{
				b = false;
			}
		}

	
	}

	c++;
	int t = c % 60;
	if (t <= 30)
	{
		mouseSprite.texHandle = (int)TagTexture::kPress;
	}
	else
	{
		mouseSprite.texHandle = (int)TagTexture::kCursor;
	}

	float s = 1.0f + sinf(c * 0.02f) * 0.05f;

	area0.trans.scale = { s,s,s };
	area1.trans.defaultQuatenion.z_axis.deltaTheta = (float)c;

	for (auto itr = blocks.begin(); itr != blocks.end(); ++itr)
	{
		(*itr)->trans.SetWorldMatBasedQuaternion();
		(*itr)->chara.Update(vpMat_);
	}

	for (auto itr = movableBlocks.begin(); itr != movableBlocks.end(); ++itr)
	{
		(*itr)->Update(vpMat_);
	}
	
	
	area0.Update(vpMat_);
	area1.Update(vpMat_);
	mask.Update(vpMat_);

	mouseSprite.Update(vpMat_);


}

//マップの描画処理
void Map1::Draw()
{
	for (auto itr = blocks.begin(); itr != blocks.end(); ++itr)
	{
		(*itr)->chara.Draw();
	}

	for (auto itr = movableBlocks.begin(); itr != movableBlocks.end(); ++itr)
	{
		(*itr)->Draw();
	}

	if (InGameController::state != InGameController::State::kResult)
	{
		mouseSprite.Draw();

		if (b)
		{
			area1.Draw();
		}

		mask.Draw();
		NoviceLike::SetBlendMode(BlendMode::kBlendModeAdd);
		area0.Draw();
		NoviceLike::SetBlendMode(BlendMode::kBlendModeNormal);


	}

}

Map1::Map1()
{
	counter.Initialize(180.0f);

	mask.Initialize(30.0f, 30.0f, {}, TagTexture::kWhite2x2, { 0,0,0,0.0f });
	area0.Initialize(5.5f,4.0f,{},TagTexture::kPredictiveBlock, { 150,150,0,100 });
	area1.Initialize(1.0f, 1.0f, {}, TagTexture::kPredictiveBlock, { 200,0,0,200.0f});


	LoadMapChipCsv("./Resource/MapCsvFiles/Map1.csv");

	for (auto itr = movableBlocks.begin(); itr != movableBlocks.end(); ++itr)
	{
		(*itr)->Initialize();
	}

	mask.trans.pos = InGameMap::GetPositionBasedInIndex(5, 5);
	mask.trans.pos.z = -1.0f;


	mouseePos = InGameMap::GetPositionBasedInIndex(8, 0);
	mouseePos.y += 0.6f;
	mouseePos.x -= 1.0f;

	area1.trans.pos = InGameMap::GetPositionBasedInIndex(0, 10);
	area1.trans.pos.z -= 0.7f;
	area1.trans.pos.x -= 0.6f;
	//area1.trans.pos.y -= 1.0f;


	area0.trans.pos = mouseePos;
	area0.trans.pos.x -= 1.5f;
	area0.trans.pos.y += 2.5f;

	area0.trans.pos.z = -2.0f;

	float n = 0.25f;
	mouseSprite.InitializeSprite({}, 394* n, 512* n, (int)TagTexture::kCursor, { 120,120,120,255 });


	mouseSprite.world_followPos = &mouseePos;
	mouseSprite.offSet = Vector2{ -100.0f,50.0f};
	cardValues.emplace_back(5.0f);
	cardValues.emplace_back(5.0f);

}
