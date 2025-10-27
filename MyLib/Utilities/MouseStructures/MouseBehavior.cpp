#include "MouseBehavior.h"
#include "../CollisionDetectionsStructures/CollisionDetection.h"


MouseBehavior* MouseControll::ChangeMouse(MouseBehavior* transformDebug_)
{
	if (NoviceLike::keys[DIK_LSHIFT] && NoviceLike::keys[DIK_X] && !NoviceLike::preKeys[DIK_X])
	{
		MouseBehavior::switcher = !MouseBehavior::switcher;
	}

	MouseBehavior* mouse[1]
	{
		transformDebug_,
	};

	return mouse[MouseBehavior::switcher];

}


void MouseBehavior::RayCast(Matrix4 vpMat_)
{
	Matrix4 vpvMatrix = vpMat_.Multiply(Get_ViewportTransformation3D());
	Matrix4 inv_vpvMatrix = vpvMatrix.GetInversed();

	Vector4<float>  mousePosNear = { (float)NoviceLike::mousePos.x,(float)NoviceLike::mousePos.y,0.0f,1.0f };
	Vector4<float> mousePosFar = { (float)NoviceLike::mousePos.x,(float)NoviceLike::mousePos.y,1.0f,1.0f };

	mousePosNear = mousePosNear.GetMultiply(inv_vpvMatrix);
	mousePosFar = mousePosFar.GetMultiply(inv_vpvMatrix);

	mousePosNear = mousePosNear * (1.0f / mousePosNear.w);
	mousePosFar = mousePosFar * (1.0f / mousePosFar.w);

	Vector4<float> tmp = (mousePosFar - mousePosNear).GetNormalized();
	mouseRay = { tmp.x,tmp.y,tmp.z };

}

void MouseBehavior::SetMousePos()
{
	//マウスの座標を取得
	preMousePos = { (float)NoviceLike::mousePos.x ,(float)NoviceLike::mousePos.y };
	NoviceLike::SetMousePos(&NoviceLike::mousePos);
	ScreenToClient(pDxCommon->hwnd, &NoviceLike::mousePos);
}

bool MouseBehavior::CollisionWithMouse3D(Vector3 dst_world_,float dst_radius_,Matrix4 vpMat_)
{
	Vector2 dst_screen = ConvertToScreen(dst_world_, vpMat_);
	
	return CollisionDetections::Col2D::CircleCollision(mouse_radius,
		{ (float)NoviceLike::mousePos.x,(float)NoviceLike::mousePos.y,0.0f },
		dst_radius_,{ dst_screen.u,dst_screen.v,0.0f });
}

bool MouseBehavior::CollisionWithMouse2D(Vector2 dst_screen_, float dst_radius_, Matrix4 vpMat_)
{
	return CollisionDetections::Col2D::CircleCollision(mouse_radius,
		{ (float)NoviceLike::mousePos.x,(float)NoviceLike::mousePos.y,0.0f },
		dst_radius_, { dst_screen_.u,dst_screen_.v,0.0f });
}

bool MouseBehavior::CollisionWithMouseAABB3D(Vector3 dst_world_, float width_, float height_, Matrix4 vpMat_,float mouseSize_ )
{
	Vector2 dst_screen = ConvertToScreen(dst_world_, vpMat_);

	Rect objRect;
	objRect.SetRectShape({ dst_screen.u,dst_screen.v,0.0f }, width_, height_);

	Rect mouseRect;
	mouseRect.SetRectShape({ (float)NoviceLike::mousePos.x,(float)NoviceLike::mousePos.y,0.0f }, mouseSize_, mouseSize_);

	return  CollisionDetections::Col2D::ObjectAABB(&objRect, &mouseRect);

}

bool MouseBehavior::CollisionWithMouseAABB2D(Vector3 dst_screen_, float width_, float height_, float mouseSize_ )
{
	Rect objRect;
	objRect.SetRectShape({ dst_screen_.x,dst_screen_.y,0.0f }, width_, height_);

	Rect mouseRect;
	mouseRect.SetRectShape({ (float)NoviceLike::mousePos.x,(float)NoviceLike::mousePos.y,0.0f }, mouseSize_, mouseSize_);

	return  CollisionDetections::Col2D::ObjectAABB(&objRect, &mouseRect);

}




