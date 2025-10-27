#pragma once
#include "../../../MyLib/Utilities/PrimitiveStructures/QuadMesh/QuadBehavior/Cube/JointedCube/JointedCube.h"
#include "../../../MyLib/Utilities/PrimitiveStructures/QuadMesh/QuadBehavior/Cube/DisjointedCube/DIsjointedCube.h"
class Card
{
public:    
	Card(float moveValue = 1.0f);

	// ゲームオブジェクトの初期化
	void Initialize(float moveVal);
	// ゲームオブジェクトの更新処理
	void Update(Matrix4 vpMat_);
	// ゲームオブジェクトの描画
	void Draw();
	

    float GetMoveValue() const { return moveValue; }

    void SetMoveValue(float value) { moveValue = value; }

	JointedCube jointCube;
	JointedCube AuraJointCube;
    float moveValue;

	bool isSelected = false;
	bool isUsed = false;
	bool isAura = false;

	Vector3 originalPos;      // 初期位置
	Vector3 currentPos;       // 現在の表示位置（lerpで更新）
	Vector3 targetPos;        // マウス当たり時の目標位置
	Vector3 hoverOffsetDir;   // 扇中心からの方向ベクトル

	float easingTime = 0.0f;           // 経過時間
	float easingDuration = 1.0f;       // 伸ばす時間

	float currentRotationY = 0.0f;     // 現在のY回転角度(度)
	float targetRotationY = 0.0f;      // 目標のY回転角度(度)
	bool isRotating = false;

	float baseRotationY = 0.0f;
	float baseRotationZ = 0.0f;

	// セレクトシーンの変数
	int row;
	int col;

	Matrix4 vpMat;
};

