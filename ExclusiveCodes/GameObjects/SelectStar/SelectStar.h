#pragma once
#include "../../../MyLib/Utilities/PrimitiveStructures/QuadMesh/QuadBehavior/Cube/JointedCube/JointedCube.h"

class SelectStar
{
public:
	SelectStar();
	// ゲームオブジェクトの初期化
	void Initialize();
	// ゲームオブジェクトの更新処理
	void Update(Matrix4 vpMat_);
	// ゲームオブジェクトの描画
	void Draw();

	JointedCube jointCube;
	JointedCube auraJointCube;

	float easingTime = 0.0f;           // 経過時間
	float easingDuration = 1.0f;       // 伸ばす時間

	bool isAura = false;

};


