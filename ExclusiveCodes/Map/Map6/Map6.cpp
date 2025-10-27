#include "Map6.h"
#include"../../InGameController/InGameController.h"

void Map6::Update(Matrix4 vpMat_)
{
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



}

//マップの描画処理
void Map6::Draw()
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

Map6::Map6()
{
	LoadMapChipCsv("./Resource/MapCsvFiles/Map6.csv");

	for (auto itr = movableBlocks.begin(); itr != movableBlocks.end(); ++itr)
	{
		(*itr)->Initialize();
	}

	cardValues.emplace_back(3.0f);
	cardValues.emplace_back(2.0f);
	cardValues.emplace_back(1.0f);
	cardValues.emplace_back(3.0f);
	cardValues.emplace_back(6.0f);
	cardValues.emplace_back(1.0f);
	cardValues.emplace_back(1.0f);
	cardValues.emplace_back(1.0f);

}

