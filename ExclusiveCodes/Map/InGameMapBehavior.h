#pragma once
#include "../../MyLib/Utilities/BaseClassStructures/Transform/Transform.h"
#include "../../MyLib/Utilities/BaseClassStructures/PrimitiveShape/PrimitiveShape.h"
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "Chip/ChipBehavior.h"
#include "../GameObjects/MovableBlock/MovableBlock.h"
#include "../Map/Chip/NormalBlock/NormalBlock.h"
#include "../GameObjects/Card/Card.h"

struct InGameMapBehavior
{
	//チップ
	std::vector<std::vector<uint8_t>> tagData;
	std::vector<std::unique_ptr<NormalBlock>> blocks;
	std::vector<std::unique_ptr<MovableBlock>> movableBlocks;

	std::vector<float> cardValues;

public:

	static const uint32_t kNumBlockVertical = 32;
	static const uint32_t kNumBlockHorizontal = 12;

	InGameMapBehavior()
	{

	}

	//マップインデックスをもとにワールド座標を返す
	static Vector3 GetPositionBasedInIndex(uint16_t y_, uint16_t x_);
	//ワールド座標をもとにマップインデックスを返す
	static IndexedPosition GetIndexedPositionBasedInPosition(Vector3 pos_);
	//sampleのインデックスが、自身のいんでっくすの周囲にあるか返す
	static bool IsAround(IndexedPosition* dst_, IndexedPosition* sample_, float next_);

	//マップチップの矩形情報を得る
	Rect GetChipRect(uint16_t y_, uint16_t x_);
	//マップチップのタグを得る
	uint8_t GetTagChip(uint16_t y_, uint16_t x_);
	//CSVファイルからマップチップを読み込む
	void LoadMapChipCsv(std::string const& filePath_);
	//マップチップの全解放
	void ResetMapChipData(uint32_t kNumBlockVertical_, uint32_t kNumBlockHorizontal_);

	//マップの初期化()
	void Initialize();

	//マップの更新処理
	virtual void Update(Matrix4 vpMat_) = 0;
	//マップの描画処理
	virtual void Draw() = 0;

};

struct InGameMap :InGameMapBehavior
{
	//マップの更新処理
	virtual void Update(Matrix4 vpMat_)override {}
	//マップの描画処理
	virtual void Draw()override {}

};

