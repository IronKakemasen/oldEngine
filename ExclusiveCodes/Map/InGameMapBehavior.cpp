#include "InGameMapBehavior.h"
#include < assert.h >
#include "Chip/ChipBehavior.h"
#include "../../MyLib/Utilities/GameObjectStructures/GameObjectManager/ObjectManager.h"


Vector3 InGameMapBehavior::GetPositionBasedInIndex(uint16_t y_, uint16_t x_)
{
	return { ChipBehavior::kBlockWidth * x_ + ChipBehavior::kBlockWidth * 0.5f ,
		ChipBehavior::kBlockHeight * (kNumBlockVertical - y_) - ChipBehavior::kBlockHeight * 0.5f , 0 };
}

void InGameMapBehavior::LoadMapChipCsv(std::string const& filePath_)
{
	ResetMapChipData(kNumBlockVertical, kNumBlockHorizontal);

	std::ifstream file;
	file.open(filePath_);
	assert(file.is_open());

	std::stringstream mapChipCsv;
	mapChipCsv << file.rdbuf();
	file.close();

	for (uint32_t i = 0; i < kNumBlockVertical; ++i)
	{
		std::string line;
		getline(mapChipCsv, line);

		std::istringstream line_stream(line);
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j)
		{
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word))
			{
				tagData[i][j] = static_cast<uint8_t>(mapChipTable[word]);
			}
		}
	}

	int y = 0;

	for (auto& row : tagData)
	{
		int x = 0;

		for (auto& element : row)
		{
			if (element == (int)TagChip::kBlock)
			{
				auto& block = blocks.emplace_back(std::make_unique<NormalBlock>());

				block->trans.pos.x = ChipBehavior::kBlockWidth * 0.5f + ChipBehavior::kBlockWidth * x;
				block->trans.pos.y = ChipBehavior::kBlockHeight * (kNumBlockVertical)-
					ChipBehavior::kBlockHeight * 0.5f - ChipBehavior::kBlockHeight * y;
				block->chara.cube.trans.BeChildren(&block->trans);
			}

			else if (element == (int)TagChip::kMovableBlock)
			{
				auto& movable = movableBlocks.emplace_back(std::make_unique<MovableBlock>());

				movable->mainTrans.pos.x = ChipBehavior::kBlockWidth * 0.5f + ChipBehavior::kBlockWidth * x;
				movable->mainTrans.pos.y = ChipBehavior::kBlockHeight * (kNumBlockVertical)-
					ChipBehavior::kBlockHeight * 0.5f - ChipBehavior::kBlockHeight * y;
			}

			x++;
		}

		y++;
	}


}

//ブロックの位置の初期化、ムーバブルブロックの初期化
void InGameMapBehavior::Initialize()
{
	int y = 0;
	int i = 0;
	int k = 0;

	for (auto& row : tagData)
	{
		int x = 0;

		for (auto& element : row)
		{
			if (element == (int)TagChip::kBlock)
			{ 
				blocks[i]->trans.pos.x = ChipBehavior::kBlockWidth * 0.5f + ChipBehavior::kBlockWidth * x;
				blocks[i]->trans.pos.y = ChipBehavior::kBlockHeight * (kNumBlockVertical)-
					ChipBehavior::kBlockHeight * 0.5f - ChipBehavior::kBlockHeight * y;
				++i;
			}

			else if (element == (int)TagChip::kMovableBlock)
			{
				movableBlocks[k]->mainTrans.pos.x = ChipBehavior::kBlockWidth * 0.5f + ChipBehavior::kBlockWidth * x;
				movableBlocks[k]->mainTrans.pos.y = ChipBehavior::kBlockHeight * (kNumBlockVertical)-
					ChipBehavior::kBlockHeight * 0.5f - ChipBehavior::kBlockHeight * y;
				movableBlocks[k]->Initialize();

				++k;
			}

			x++;
		}

		y++;
	}

	
}


void InGameMapBehavior::ResetMapChipData(uint32_t kNumBlockVertical_, uint32_t kNumBlockHorizontal_)
{
	tagData.clear();
	tagData.resize(kNumBlockVertical_);
	for (auto& horizontal_ : tagData)
	{
		horizontal_.resize(kNumBlockHorizontal_);
	}
}


IndexedPosition InGameMapBehavior::GetIndexedPositionBasedInPosition(Vector3 pos_)
{
	IndexedPosition indexedPosition =
	{
		uint16_t(pos_.x / ChipBehavior::kBlockWidth),
		uint16_t((InGameMapBehavior::kNumBlockVertical - 1) - uint16_t(pos_.y / ChipBehavior::kBlockHeight))
	};

	if (indexedPosition.X < 0)indexedPosition.X = 0;
	else if (indexedPosition.X > (kNumBlockHorizontal - 1))indexedPosition.X = (kNumBlockHorizontal - 1);

	if (indexedPosition.Y < 0)indexedPosition.Y = 0;
	else if (indexedPosition.Y > (kNumBlockVertical - 1))indexedPosition.Y = (kNumBlockVertical - 1);


	return indexedPosition;
}

Rect InGameMapBehavior::GetChipRect(uint16_t y_, uint16_t x_)
{
	Vector3 center = GetPositionBasedInIndex(y_, x_);
	Rect rect;
	rect.SetRectShape(center, ChipBehavior::kBlockWidth, ChipBehavior::kBlockHeight);

	return rect;
}

uint8_t InGameMapBehavior::GetTagChip(uint16_t y_, uint16_t x_)
{
	if (y_<0 || y_>kNumBlockVertical - 1)
	{
		return (uint8_t)TagChip::kSpace;
	}

	if (x_<0 || x_>kNumBlockHorizontal - 1)
	{
		return (uint8_t)TagChip::kSpace;
	}

	return tagData[y_][x_];
}

bool InGameMapBehavior::IsAround(IndexedPosition* dst_, IndexedPosition* sample_, float next_)
{
	if (dst_->X< (sample_->X - next_) || dst_->X >(sample_->X + next_))
	{
		return false;
	}

	if (dst_->Y< (sample_->Y - next_) || dst_->Y >(sample_->Y + next_))
	{
		return false;
	}

	return true;
}
