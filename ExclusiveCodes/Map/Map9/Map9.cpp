#include "Map9.h"
#include"../../InGameController/InGameController.h"

void Map9::Update(Matrix4 vpMat_)
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
void Map9::Draw()
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

Map9::Map9()
{
	LoadMapChipCsv("./Resource/MapCsvFiles/Map9.csv");

	for (auto itr = movableBlocks.begin(); itr != movableBlocks.end(); ++itr)
	{
		(*itr)->Initialize();
	}
	cardValues.emplace_back(1.0f);
	cardValues.emplace_back(2.0f);

}

