#pragma once

#pragma once
#include <algorithm> 
#include <string>
#include <map>
#include "../../BaseClassStructures/Transform/Transform.h"
#include "../../BaseClassStructures/Physic/Physics.h"
#include "../../CounterStructures/CounterAndTime.h"
#include "../../BaseClassStructures/PrimitiveShape/PrimitiveShape.h"
#include "../../CollisionDetectionsStructures/CollisionDetection.h"
#include "../../BenriFunctionStructures/BenriFunctions.h"
#include "../../PrimitiveStructures/QuadMesh/QuadBehavior/Quad/Quad.h"
#include "../../PrimitiveStructures/QuadMesh/QuadBehavior/Sprite/Sprite.h"
#include "../../EasingFuctionStructures/EasingFunctions.h"




struct ObjectID
{
	//例を用意しています
	enum class Tag
	{
		KPlayer = 0,
		kMovable = 1,
		kKey = 2,
		kGoal = 3,


		kDoudemoii,




		kCount,
	};

	//タグ
	Tag tag = Tag ::kDoudemoii;
	//自身の衝突ナンバー
	uint32_t collisionAttribute = 0;
	//衝突可能相手のナンバー
	uint32_t collisionMask = 0;
	//名前
	std::string name = "none";
};


struct ObjectBehavior
{
	//ゲームオブジェクトの合計生成数。
	static inline int objectNumber = -1;

	//オブジェクトのトランスフォーム
	Transform4 mainTrans;
	//キューブコライダー
	CubeShape cubeCollider;
	//サークルコライダー
	//CircleShape circleColl;
	//ゲームオブジェクトのイズアクティブ。これがオフの時は、更新処理も描画もされない
	bool isActive = false;
	//ゲームオブジェクトを一意に設定するためのもの
	ObjectID objectID;


	//各衝突相手に対して衝突後の処理（バック）を設定するための箱
	std::map<ObjectID::Tag, CollisionBackSet> collisionBackActivationMap;
	//衝突相手の情報。衝突すると代入される
	ObjectBehavior* collidedObj_ = nullptr;


	//ゲームオブジェクトの更新処理
	virtual void Update(Matrix4 vpMat_) = 0;
	//ゲームオブジェクトの初期化
	virtual void Initialize() = 0;
	//ゲームオブジェクトの描画
	virtual void Draw() = 0;

	//衝突反応関数をセットする
	virtual void SetCollisionBackTable() = 0;
	//設定した衝突反応関数( collisionBackActivationMap[].func )の更新処理
	virtual void CollisionBackUpdate() = 0;

	//引数にタグに応じて、ObjectIDを設定する。
	void SetObjectID(ObjectID::Tag tag_);
	//（いじる必要なし）衝突した時、衝突相手のタグをもとに設定した関数をアクティブ化する。
	void OnTriggerEnter(ObjectID::Tag tag_);

	ObjectBehavior() = default;

};

class GameObject :public ObjectBehavior
{

public:
	//ゲームオブジェクトの更新処理
	virtual void Update(Matrix4 vpMat_) override{}
	//ゲームオブジェクトの初期化
	virtual void Initialize() override{}
	//ゲームオブジェクトの描画
	virtual void Draw() override{}
	//衝突反応関数をセットする
	virtual void SetCollisionBackTable() override{}
	//設定した衝突反応関数( collisionBackActivationMap[].func )の更新処理
	virtual void CollisionBackUpdate()override{}

	GameObject(){}
};
