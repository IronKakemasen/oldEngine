#pragma once
#include "../InGameMapBehavior.h"
#include "../Chip/NormalBlock/NormalBlock.h"



struct Map0:InGameMap
{
	CounterAndInvTime mapTImeCounter;
	CounterAndInvTime mugenCounter;

	Sprite aKeySprite;
	Sprite dKeySprite;
	Sprite jumpKeySprite;
	Sprite keyStringSprite;
	Sprite goalStringSprite;

	Vector3 aKeyPos;
	Vector3 dKeyPos;

	Vector3 jumpKeyPos;
	Vector3 keyPos;
	Vector3 goalPos;

	Vector3 commonScale = {};


	//マップの更新処理
	virtual void Update(Matrix4 vpMat_)override;
	//マップの描画処理
	virtual void Draw()override;

	Map0();
};

