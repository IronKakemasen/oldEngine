#pragma once
#include "../PrimitiveStructures/QuadMesh/QuadBehavior/Cube/DisjointedCube/DisjointedCube.h"
#include <vector>
#include <memory>

struct CatmullBehavior
{
	//オブジェクトの基準座標
	Transform4 trans;
	//制御点
	std::vector<std::unique_ptr<DisjointedCube>> controllPoints;
	//補間点
	std::vector<Vector3> points;

	//制御点の個数(実際には+3)
	float numCorners = 4;
	//張力（0.5できゃとまる曲線）
	float tension = 0.5f;
	float radius = 4.0f;
	float dt = 0.25f;
	float kNumModel = (1.0f / dt) * (numCorners + 3.0f);

	virtual void Update(Matrix4 vpMat_) = 0;
	virtual void Draw() = 0;
	virtual void Initialize() = 0;
	virtual void ControllPoints(Vector3& edPoint, float rate, int No) = 0;
	Vector3 GetComputedNum(size_t index_, float t_);
	void Set(float dt_, float numCorners_, float radius_, Vector3 setPos_);
	CatmullBehavior() = default;



	//virtual ~CatmullBehavior() = 0;
};

