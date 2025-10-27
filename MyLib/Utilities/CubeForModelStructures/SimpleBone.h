#pragma once
#include "../PrimitiveStructures/QuadMesh/QuadBehavior/Cube/DisjointedCube/DisjointedCube.h"



struct SimpleBone
{
	static inline bool showBone = true;
	DisjointedCube jointPart;

	void Initialize(Vector3 pos_, Vector3 size_, Vector3 targetDir_ = { 0,0,1});
	void Update(Matrix4 vpMat_);
	void Draw();
	void Debug();

};

