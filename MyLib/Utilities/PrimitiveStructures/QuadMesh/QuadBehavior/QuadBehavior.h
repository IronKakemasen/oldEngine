#pragma once
#include "../../PrimitiveClass.h"


struct QuadBehavior :PrimitiveClassBehavior
{
	VertexData leftTop;
	VertexData rightTop;
	VertexData leftBottom;
	VertexData rightBottom;
	Vector3 purgeDir;
	float purgeRotateSpeed;

	static inline int count = -1;

	QuadBehavior();

	void Initialize(float width_, float height_, Vector3 world_pos_, TagTexture texHandle_, Vector4<float> color_ = {255,255,255,255});
	void Initialize(Vector3 local_leftTop_, Vector3 local_rightTop_, Vector3 local_leftBottom_, Vector3 local_rightBottom_,
		Vector3 world_pos_, TagTexture texHandle_, Vector4<float> color_ = { 255,255,255,255 });

	virtual void Update(Matrix4 vpMat_ = 0) = 0;
	virtual void Draw(int enableLighting_ = true) = 0;
	//頂点設定
	void SetNormal();
	//void SetTexcoord(Vector2 leftTop_ = { 0.0f,0.0f }, Vector2 rightTop_ = { 1.0f,0.0f }, 
	//	Vector2 rightBottom_ = { 1.0f,1.0f }, Vector2 leftBottom_ = {0.0f,1.0f});

	void SetTexcoord(Vector2 leftTop_ = { -0.5f,-0.5f }, Vector2 rightTop_ = { 0.5f,-0.5f },
		Vector2 rightBottom_ = { 0.5f,0.5f }, Vector2 leftBottom_ = { -0.5f,0.5f });

	void SetVertex(float width_, float height_);
	void SetVertex(Vector3 local_leftTop_, Vector3 local_rightTop_, Vector3 local_rightBottom_, Vector3 local_leftBottom_);
	void ResetTexcoord();
	void AdjustForSpriteSheet(float numSprites_);
	//virtual void Debug();

};


