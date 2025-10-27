#pragma once
#include "../CubeBehavior.h"

//基本こっち。更新処理で頂点変更可能タイプ
//UV親子関係 : 未
//初期化設定 : 必...Initialize()
struct JointedCube :CubeBehavior
{
	bool do_purge = false;

	JointedCube();

	virtual void Update(Matrix4 vpMat_)override;
	virtual void Draw(int enableLighting_ = true)override;
};

