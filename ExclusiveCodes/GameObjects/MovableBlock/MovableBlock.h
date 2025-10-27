#pragma once
#include "../../../MyLib/Utilities/GameObjectStructures/GameObject/GameObject.h"
#include "../../Characters/SampleCharactor/SampleCharacter.h"


class MovableBlock: public GameObject
{
public:
	enum class ArrowDirection 
	{
		Up,
		Down,
		Left,
		Right,
		None,
	};

	// キューブのキャラクタ
	SampleCharacter chara;

	// ゲームオブジェクトの初期化
	virtual void Initialize()override;
	// ゲームオブジェクトの更新処理
	virtual void Update(Matrix4 vpMat_)override;
	// ゲームオブジェクトの描画
	virtual void Draw()override;
	// 衝突反応関数をセットする
	virtual void SetCollisionBackTable()override;
	// 設定した衝突反応関数( collisionBackActivationMap[].func )の更新処理
	virtual void CollisionBackUpdate()override;

	bool CanMoveByArrow(ArrowDirection dir, Vector3 value) const;


	// 移動関数
	void Move(ArrowDirection dir, float distance);

	void SetPosition(const Vector3& pos) 
	{
		mainTrans.pos = pos;
		targetPosition = pos; 
	}

	MovableBlock();

	Vector3 prePos;
	float moveSpeed = 0.08f; 

	Vector3 startPosition;
	Vector3 targetPosition;
	float easingTime = 0.0f;
	float easingDuration = 2.0f; 
	bool isMoving = false;
};

