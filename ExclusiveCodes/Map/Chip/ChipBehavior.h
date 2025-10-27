#pragma once
#include <map>
#include <string>
#include "../../../MyLib/Utilities/BaseClassStructures/PrimitiveShape/PrimitiveShape.h"
#include "../../../MyLib/Utilities/BaseClassStructures/Transform/Transform.h"


enum class TagChip
{
	//何もなし
	kSpace = 0,
	//ただのブロック
	kBlock = 1,
	kPlayer = 2,
	kMovableBlock = 3,
	kKey = 4,
	kGoal = 5,
};

namespace
{
	std::map<std::string, TagChip> mapChipTable =
	{
		{"0", TagChip::kSpace  },
		{"1", TagChip::kBlock},
		{"2", TagChip::kPlayer  },
		{"3", TagChip::kMovableBlock},
		{"4", TagChip::kKey  },
		{"5", TagChip::kGoal},

	};
}

//マップチップ上のインデックス
struct IndexedPosition
{
	uint16_t X;
	uint16_t Y;
};

struct ChipBehavior
{
	static constexpr float kBlockWidth = 1.2f;
	static constexpr float kBlockHeight = 1.2f;

	RectShape rectShape;
	uint8_t tagChip = (int)TagChip::kSpace;
	Transform4 trans;
	Vector3 prePos = { 0.0f,0.0f,0.0f };

	ChipBehavior();

	virtual void Update(Matrix4 vpMat_) = 0;
	virtual void Draw() = 0;


};

struct Chip:ChipBehavior
{
	virtual void Update(Matrix4 vpMat_)override	{}
	virtual void Draw()override{ }

};




