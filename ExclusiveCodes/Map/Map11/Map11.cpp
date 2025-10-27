#include "Map11.h"
#include"../../InGameController/InGameController.h"

void Map11::Update(Matrix4 vpMat_)
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
void Map11::Draw()
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

Map11::Map11()
{
	LoadMapChipCsv("./Resource/MapCsvFiles/Map11.csv");

	for (auto itr = movableBlocks.begin(); itr != movableBlocks.end(); ++itr)
	{
		(*itr)->Initialize();
	}

	cardValues.emplace_back(5.0f);
	cardValues.emplace_back(3.0f);
	cardValues.emplace_back(2.0f);
	cardValues.emplace_back(2.0f);
	cardValues.emplace_back(2.0f);
}

