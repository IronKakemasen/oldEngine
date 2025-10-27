#pragma once
#include "../CubeBehavior.h"

//更新処理で頂点変更不可タイプ。主にパーティクルかな
//UV親子関係 : 未
//初期化設定 : 必...Initialize()　
struct DisjointedCube:CubeBehavior
{
	DisjointedCube();
	virtual void Update(Matrix4 vpMat_)override;
	virtual void Draw(int enableLighting_ = true)override;

};
