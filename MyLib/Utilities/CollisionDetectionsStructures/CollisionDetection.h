#pragma once
#include "../BaseClassStructures/VectorAndMatrix/VectorAndMatrix.h"
#include "../BaseClassStructures/PrimitiveShape/PrimitiveShape.h"
#include <functional>

//なにこれ？
struct CollisionMapInfo
{
	//天井にぶつかっているか
	bool isCeiling = false;
	//接地判定
	bool isGround = false;
	//壁とぶつかったか
	bool isWall = false;
	//動くブロックの上に載っているか
	bool isOnMovableBlock = false;


	Vector3 deltaPos = { 0.0f,0.0f,0.0f };
};

struct CollisionBackSet
{
	bool isActive = false;
	std::function<bool()> func;
};

namespace CollisionDetections
{
	namespace Col2D
	{
		bool SegmentAndCircle(float radius_, Vector2 circlePos_, Vector2 segStart_, Vector2 segEnd_);
		//2つのオブジェクトが衝突したとき、thisObjのポジションを補正する（めり込み補正）
		bool FixThisObjPosWhenCollided(Vector3* thisPos_, Vector3 thisPrePos_,
			float thisWidth_, float thisHeight_, Vector3& thisVelocity_, CollisionMapInfo& colMapInfo_,
			Vector3* otherPos_, Vector3 otherPrePos_,
			float otherWidth_, float otherHeight_,bool senyou_ = false);
		bool ObjectAABB(Rect* const thisRect_, Rect* const otherRect_);

		bool CircleCollision(float lRad_, Vector3 lPos_, float rRad_, Vector3 rPos_);
	}

	namespace Col3D
	{
		bool ObjectAABB3D(Rect3D* const thisRect_, Rect3D* const otherRect_);
		bool GetCubeCollisionResult(Vector3 const& thisVec_, CubeShape const& thisShape_,
			Vector3 const& otherVec_, CubeShape const& otherShape_);
	}


};

