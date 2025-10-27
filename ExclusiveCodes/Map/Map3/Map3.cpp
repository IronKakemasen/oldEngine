#include "Map3.h"
#include"../../InGameController/InGameController.h"

void Map3::Update(Matrix4 vpMat_)
{
	for (auto itr = blocks.begin(); itr != blocks.end(); ++itr)
	{
		(*itr)->trans.SetWorldMatBasedQuaternion();
		(*itr)->chara.Update(vpMat_);
	}

	for (auto itr = movableBlocks.begin(); itr != movableBlocks.end(); ++itr)
	{
		(*itr)->Update(vpMat_);
	}

}

//マップの描画処理
void Map3::Draw()
{
	for (auto itr = blocks.begin(); itr != blocks.end(); ++itr)
	{
		(*itr)->chara.Draw();
	}

	for (auto itr = movableBlocks.begin(); itr != movableBlocks.end(); ++itr)
	{
		(*itr)->Draw();
	}


}

Map3::Map3()
{
	LoadMapChipCsv("./Resource/MapCsvFiles/Map3.csv");

	for (auto itr = movableBlocks.begin(); itr != movableBlocks.end(); ++itr)
	{
		(*itr)->Initialize();
	}
	cardValues.emplace_back(1.0f);
	cardValues.emplace_back(1.0f);
	cardValues.emplace_back(1.0f);

}

