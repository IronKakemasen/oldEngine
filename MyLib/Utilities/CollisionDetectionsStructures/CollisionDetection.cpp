#include "CollisionDetection.h"

//2D-----------------------------------------------------------------------------------------------------------------------------------
bool CollisionDetections::Col2D::SegmentAndCircle(float radius_, Vector2 circlePos_, Vector2 segStart_, Vector2 segEnd_)
{
	bool ret = false;

	Vector2 seg = segEnd_ - segStart_;
	Vector2 s2c = circlePos_ - segStart_;
	Vector2 e2c = circlePos_ - segEnd_;
	Vector2 e_seg = seg.GetNormalized();

	float dot = s2c.GetDot(seg);
	float inv_mag = 1.0f / (seg.GetMagnitutde() * seg.GetMagnitutde());
	Vector2 proj = seg * inv_mag * dot;
	Vector2 nearPoint = { segStart_.u + proj.u,segStart_.v + proj.v };
	float length = (circlePos_ - nearPoint).GetMagnitutde();

	if (length < radius_)
	{
		float cos1 = s2c.GetCos(seg);
		float cos2 = e2c.GetCos(seg);

		// 二つの内積の掛け算結果が0以下なら当たり
		if (cos1 * cos2 <= 0.0f)
		{
			ret = true;

		}

		if (s2c.GetMagnitutde() < radius_ || e2c.GetMagnitutde() < radius_)
		{
			ret = true;
		}
	}

	return ret;
}


bool CollisionDetections::Col2D::ObjectAABB(Rect* const thisRect_, Rect* const otherRect_)
{
	bool ret = false;

	if ((thisRect_->left < otherRect_->right) && (thisRect_->right > otherRect_->left))
	{
		if ((thisRect_->bottom < otherRect_->top) && (thisRect_->top > otherRect_->bottom))
		{
			ret = true;
		}
	}

	return ret;
}

bool CollisionDetections::Col2D::CircleCollision(float lRad_, Vector3 lPos_, float rRad_, Vector3 rPos_)
{
	bool ret = false;

	float mag = (lPos_ - rPos_).GetMagnitutde();
	if (mag < (lRad_ + rRad_))ret = true;


	return ret;
}

//2つのオブジェクトが衝突したとき、thisObjのポジションを補正する（めり込み補正）
bool CollisionDetections::Col2D::FixThisObjPosWhenCollided(Vector3* thisPos_, Vector3 thisPrePos_,
	float thisWidth_, float thisHeight_, Vector3& thisVelocity_, CollisionMapInfo& colMapInfo_,
	Vector3* otherPos_, Vector3 otherPrePos_,
	float otherWidth_, float otherHeight_, bool senyou_)
{
	bool ret = false;

	//移動前の矩形
	Rect pre_thisRect;
	pre_thisRect.SetRectShape(thisPrePos_, thisWidth_, thisHeight_);
	Rect pre_otherRect;
	pre_otherRect.SetRectShape(otherPrePos_, otherWidth_, otherHeight_);

	//移動後の矩形
	Rect thisRect;
	thisRect.SetRectShape(*thisPos_, thisWidth_, thisHeight_);
	Rect otherRect;
	otherRect.SetRectShape(*otherPos_, otherWidth_, otherHeight_);

	if (ObjectAABB(&thisRect, &otherRect))
	{
		//右からぶつかる
		if (pre_thisRect.left >= pre_otherRect.right)
		{
			if (pre_thisRect.bottom < otherRect.top && pre_thisRect.bottom > otherRect.bottom - otherWidth_)
			{
				float delta = otherRect.right - thisRect.left;

				if (delta >= 0)
				{
					thisPos_->x += delta + delta * 0.01f;
					//改めてプレイヤーの四つ角せってい
					thisRect.SetRectShape(*thisPos_, thisWidth_, thisHeight_);

					ret = true;
					colMapInfo_.isWall = true;
				}
			}
		}

		//左からぶつかる
		if (pre_thisRect.right <= pre_otherRect.left)
		{
			if (pre_thisRect.bottom < otherRect.top && pre_thisRect.bottom > otherRect.bottom - otherHeight_)
			{
				float delta = thisRect.right - otherRect.left;

				if (delta >= 0)
				{
					thisPos_->x -= delta + delta * 0.01f;
					//改めてプレイヤーの四つ角せってい
					thisRect.SetRectShape(*thisPos_, thisWidth_, thisHeight_);

					ret = true;
					colMapInfo_.isWall = true;

				}
			}
		}
		else
		{

		}

		int j = 0;

		if (
			(thisVelocity_.x == 0) ||
			(thisVelocity_.x <0 && pre_thisRect.left > otherRect.left - otherWidth_ && pre_thisRect.left < otherRect.right) ||
			(thisVelocity_.x > 0 && pre_thisRect.right < otherRect.right + otherWidth_ && pre_thisRect.right > otherRect.left)

			)
		{
			//プレイヤーがオブジェクトの上にいた場合
			if (pre_thisRect.bottom >= pre_otherRect.top)
			{
				float delta = otherRect.top - thisRect.bottom;
				if (delta >= 0)
				{
					thisVelocity_.y = 0.0f;

					thisPos_->y += delta + delta * 0.001f;
					//改めてプレイヤーの四つ角せってい
					thisRect.SetRectShape(*thisPos_, thisWidth_, thisHeight_);
					ret = true;

					colMapInfo_.isGround = true;

					if (senyou_)
					{
						colMapInfo_.isOnMovableBlock = true;
					}

				}
			}

			//プレイヤーがオブジェクトの下にいた場合
			else if (pre_thisRect.top <= pre_otherRect.bottom)
			{
				float delta = thisRect.top - otherRect.bottom;

				if (delta >= 0)
				{
					thisPos_->y -= delta ;
					//改めてプレイヤーの四つ角せってい
					thisRect.SetRectShape(*thisPos_, thisWidth_, thisHeight_);

					ret = true;
					colMapInfo_.isCeiling = true;

				}
			}
			else
			{
				j = 4;
			}
		}

	}

	return ret;
}


//3D-----------------------------------------------------------------------------------------------------------------------------------
bool CollisionDetections::Col3D::ObjectAABB3D(Rect3D* const thisRect_, Rect3D* const otherRect_)
{
	bool ret = false;

	if ((thisRect_->left < otherRect_->right) && (thisRect_->right > otherRect_->left))
	{
		if ((thisRect_->bottom < otherRect_->top) && (thisRect_->top > otherRect_->bottom))
		{
			if ((thisRect_->front < otherRect_->back) && (thisRect_->back > otherRect_->front))
			{
				ret = true;
			}

		}
	}
	return ret;
}

bool CollisionDetections::Col3D::GetCubeCollisionResult(Vector3 const& thisVec_, CubeShape const& thisShape_,
	Vector3 const& otherVec_, CubeShape const& otherShape_)
{
	Vector3 tmp_a = { thisVec_.x,thisVec_.y,thisVec_.z };
	Vector3 tmp_b = { otherVec_.x,otherVec_.y,otherVec_.z };

	//移動前の矩形
	Rect3D thisRect;
	thisRect.SetCube(tmp_a, thisShape_.width, thisShape_.height, thisShape_.depth);
	Rect3D otherRect;
	otherRect.SetCube(tmp_b, otherShape_.width, otherShape_.height, otherShape_.depth);

	return ObjectAABB3D(&thisRect, &otherRect);
}
