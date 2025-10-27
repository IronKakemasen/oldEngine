#pragma once
#include "../../QuadBehavior/QuadBehavior.h"

struct Sprite:QuadBehavior
{
	//スプライトのサイズ
	Vector2 size;
	//追従するワールド座標のポインタ
	Vector3* world_followPos = nullptr;
	//追従する座標からの相対座標
	Vector2 offSet = {};

	//頂点設定
	void InitializeSprite(Vector3 screen_pos_, float width_, float height_, int texHandle_ = 0, 
		Vector4<float> color_ = { 255,255,255,255 });
	void AdjustForSpriteSheet(float numSprites);


	virtual void Update(Matrix4 vpMat_ = 0)override;
	virtual void Draw(int enableLighting_ = false)override;

};

