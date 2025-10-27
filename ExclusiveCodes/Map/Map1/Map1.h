#pragma once
#include "../InGameMapBehavior.h"
#include "../Chip/NormalBlock/NormalBlock.h"

struct InGameMouse;

struct Map1:InGameMap
{
	Quad area0;
	Quad area1;

	Quad mask;


	Sprite mouseSprite;
	Vector3 mouseePos;

	static inline CounterAndInvTime counter;


	InGameMouse* mouse = nullptr;

	bool b = true;



	//マップの更新処理
	virtual void Update(Matrix4 vpMat_)override;
	//マップの描画処理
	virtual void Draw()override;

	Map1();
};

