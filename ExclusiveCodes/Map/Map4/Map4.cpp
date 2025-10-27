#include "Map4.h"
#include"../../InGameController/InGameController.h"

void Map4::Update(Matrix4 vpMat_)
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
void Map4::Draw()
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

Map4::Map4()
{
	LoadMapChipCsv("./Resource/MapCsvFiles/Map4.csv");

	for (auto itr = movableBlocks.begin(); itr != movableBlocks.end(); ++itr)
	{
		(*itr)->Initialize();
	}


	cardValues.emplace_back(4.0f);
	cardValues.emplace_back(3.0f);

}

