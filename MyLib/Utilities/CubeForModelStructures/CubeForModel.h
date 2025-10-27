#pragma once

#include "../PrimitiveStructures/QuadMesh/QuadBehavior/Cube/JointedCube/JointedCube.h"
#include "SimpleBone.h"


struct CubeForModel
{
	JointedCube model;
	SimpleBone bone;

	CubeForModel();
	void Update(Matrix4 vpMat_);
	void Draw(DrawMode mode_ = kWireFrame);
	void Initialize(std::string name_, Vector3 size_, Vector3 local_,bool register_ = false);
	void ConnectCubeToAnotherSurface(CubeSurface to_, CubeForModel* other_);
	void ReleaseSurfaceFromRegistration(CubeSurface surface_);
	Vector3 GetVertexLocal(Vector3 local_);
	void Oyako(CubeForModel* parent_);


};

