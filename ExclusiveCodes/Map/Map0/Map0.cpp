#include "Map0.h"
#include"../../InGameController/InGameController.h"

void Map0::Update(Matrix4 vpMat_)
{
	mapTImeCounter.Add();
	float commonRotate = 0.0f;

	if (mapTImeCounter.count >= 0.5f)
	{
		float n = (mapTImeCounter.count- 0.5f) * 2.0f;

		commonScale = Easing::EaseOutBack(Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 1.0f,1.0f,1.0f }, n);
	}

	if (mapTImeCounter.count >= 1.0f)
	{
		mugenCounter.Add();


		if (mugenCounter.count >= 0.0f && mugenCounter.count < 0.25f)
		{
			commonRotate = -45.0f;
		}
		else if (mugenCounter.count >= 0.25f && mugenCounter.count < 0.5f)
		{
			commonRotate = 0.0f;

		}

		else if (mugenCounter.count >= 0.5f && mugenCounter.count < 0.75f)
		{
			commonRotate = 45.0f;

		}
		else
		{
			commonRotate = 0.0f;

		}

		mugenCounter.IsEnd();

	}

	aKeySprite.trans.scale = commonScale;
	dKeySprite.trans.scale = commonScale;
	jumpKeySprite.trans.scale = commonScale;
	keyStringSprite.trans.scale = commonScale;
	goalStringSprite.trans.scale = commonScale;

	aKeySprite.trans.defaultQuatenion.z_axis.deltaTheta = commonRotate;
	dKeySprite.trans.defaultQuatenion.z_axis.deltaTheta = commonRotate;
	jumpKeySprite.trans.defaultQuatenion.z_axis.deltaTheta = commonRotate;
	keyStringSprite.trans.defaultQuatenion.z_axis.deltaTheta = commonRotate;
	goalStringSprite.trans.defaultQuatenion.z_axis.deltaTheta = commonRotate;




	//ムーバ
	for (auto itr = blocks.begin(); itr != blocks.end(); ++itr)
	{
		(*itr)->trans.SetWorldMatBasedQuaternion();
		(*itr)->chara.Update(vpMat_);
	}

	//ムーバ
	for (auto itr = movableBlocks.begin(); itr != movableBlocks.end(); ++itr)
	{
		(*itr)->Update(vpMat_);
	}

	aKeySprite.Update(vpMat_);
	dKeySprite.Update(vpMat_);
	jumpKeySprite.Update(vpMat_);

	keyStringSprite.Update(vpMat_);
	goalStringSprite.Update(vpMat_);



	if (InGameController::state == InGameController::kResult)
	{
		mapTImeCounter.count = 0.0f;
		mugenCounter.count = 0.0f;
		commonScale = { 0,0,0 };
	}

}

//マップの描画処理
void Map0::Draw()
{
	for (auto itr = blocks.begin(); itr != blocks.end(); ++itr)
	{
		(*itr)->chara.Draw();
	}

	for (auto itr = movableBlocks.begin(); itr != movableBlocks.end(); ++itr)
	{
		(*itr)->Draw();
	}

	if (InGameController::state == InGameController::kPlayable)
	{
		aKeySprite.Draw();
		dKeySprite.Draw();

		jumpKeySprite.Draw();
		keyStringSprite.Draw();
		goalStringSprite.Draw();
	}



}

Map0::Map0()
{
	mugenCounter.Initialize(420.0f);
	mapTImeCounter.Initialize(150.0f);

	LoadMapChipCsv("./Resource/MapCsvFiles/map0.csv");

	int y = 0;

	for (auto& row : tagData)
	{
		int x = 0;
		for (auto& element : row)
		{
			if (element == (int)TagChip::kPlayer)
			{
				aKeyPos = InGameMap::GetPositionBasedInIndex(y, x);
				dKeyPos = InGameMap::GetPositionBasedInIndex(y, x);
			}
			
			else if (element == (int)TagChip::kGoal)
			{
				goalPos = InGameMap::GetPositionBasedInIndex(y, x);
			}

			else if (element == (int)TagChip::kKey)
			{
				keyPos = InGameMap::GetPositionBasedInIndex(y, x);
			}


			++x;
		}

		y++;
	}

	jumpKeyPos = InGameMap::GetPositionBasedInIndex(5, 4);



	aKeyPos.x -= 1.0f;
	dKeyPos.x += 1.0f;

	aKeyPos.y += 1.6f;
	dKeyPos.y += 1.6f;


	keyPos.y += 1.0f;
	goalPos.y += 1.0f;

	keyPos.z -= 0.8f;
	goalPos.z -= 0.8f;



	for (auto itr = movableBlocks.begin(); itr != movableBlocks.end(); ++itr)
	{
		(*itr)->Initialize();
	}
	

	aKeySprite.InitializeSprite({}, 32, 32,(int)TagTexture::kA, {255,255,255,255});
	aKeySprite.world_followPos = &aKeyPos;

	dKeySprite.InitializeSprite({}, 32, 32, (int)TagTexture::kD, { 255,255,255,255 });
	dKeySprite.world_followPos = &dKeyPos;

	jumpKeySprite.InitializeSprite({}, 32, 32, (int)TagTexture::kW);
	jumpKeySprite.world_followPos = &jumpKeyPos;

	keyStringSprite.InitializeSprite({}, 64, 32, (int)TagTexture::kKeyString);
	keyStringSprite.world_followPos = &keyPos;

	goalStringSprite.InitializeSprite({}, 64, 32, (int)TagTexture::kGoalString);
	goalStringSprite.world_followPos = &goalPos;



}

