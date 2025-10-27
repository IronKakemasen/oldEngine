#pragma once
#include "../SceneBehavior.h"
#include "../../PrimitiveStructures/TriangleMesh/Tetrahedra/DisjointedTetrahedra/DisjointedTetrahedra.h"
#include "../../PrimitiveStructures/QuadMesh/QuadBehavior/Cube/DisjointedCube/DisjointedCube.h"
#include "../../PrimitiveStructures/UnitSphereMesh/UnitSphere/UnitSphere.h"
#include "../../ModellObjectStructures/ModelObject.h"
#include "../../CatmullStructures/CatmullSample.h"


#include <vector>
#include <memory>

class SimpleDebugCamera; class SphericalDebugCamera;
struct CameraBehavior; class TransformDebugMouse;
struct MouseBehavior; class Sun;

class DebugScene final:public SceneBehavior
{
public:


	SimpleDebugCamera* simpleDebugCamera = nullptr;
	SphericalDebugCamera* sphericalDebugCamera = nullptr;
	CameraBehavior* cameraBehavior = nullptr;
	TransformDebugMouse* transformDebugMouse = nullptr;
	MouseBehavior* mouseBehavior = nullptr;
	Sun* sun = nullptr;
	

	DebugScene();

public:

	virtual void Update() override;
	virtual void Render() override;
	virtual void Initialize() override;
	virtual void Debug() override;



	void Set(SimpleDebugCamera* simpleDebugCamera_, SphericalDebugCamera* sphericalDebugCamera_,
		TransformDebugMouse* debugMouse_,Sun* sun_, CameraBehavior* cameraBehavior_, MouseBehavior* mouseBehavior_,
		MapModels* mapModels_);
	CameraBehavior* GetCameraBehavior();
	MouseBehavior* GetMouseBehavior();

};

