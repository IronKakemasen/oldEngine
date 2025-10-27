#pragma once
#include "../InGameMapBehavior.h"
#include "../Chip/NormalBlock/NormalBlock.h"



struct Map7:InGameMap
{

	//マップの更新処理
	virtual void Update(Matrix4 vpMat_)override;
	//マップの描画処理
	virtual void Draw()override;

	Map7();
};

