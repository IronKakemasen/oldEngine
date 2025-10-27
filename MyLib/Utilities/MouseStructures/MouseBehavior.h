#pragma once
#include "../PrimitiveStructures/QuadMesh/QuadBehavior/Sprite/Sprite.h"

enum MouseStatus
{
	kWatch,
	kEdit,
};

struct MouseBehavior
{
	static inline int drawMode = (int)DrawMode::kSolid;
	inline static bool switcher = true;
	bool mouseStatus = MouseStatus::kEdit;
	Vector3  mouseRay;
	Vector2 preMousePos;
	Sprite mouseSprite;
	float mouse_radius = 1.0f;

	virtual void Update(Matrix4 vpMat_, Vector3 cameraPos_) = 0;
	virtual void Debug() = 0;
	virtual void Draw(Matrix4 vpMat_) = 0;

	void RayCast(Matrix4 vpMat_);
	void SetMousePos();
	bool CollisionWithMouse3D(Vector3 dst_world_, float dst_radius_, Matrix4 vpMat_);
	bool CollisionWithMouse2D(Vector2 dst_screen_, float dst_radius_, Matrix4 vpMat_);

	bool CollisionWithMouseAABB3D(Vector3 dst_world_, float width_, float height_,Matrix4 vpMat_ ,float mouseSize_ = 1.0f);
	bool CollisionWithMouseAABB2D(Vector3 dst_screen_, float width_, float height_, float mouseSize_ = 1.0f);


};

namespace MouseControll
{
	MouseBehavior* ChangeMouse(MouseBehavior* transformDebug_);
};



