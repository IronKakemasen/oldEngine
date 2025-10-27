#pragma once
#include "Mesh.h"
#include <memory>

struct PrimitiveMeshData
{
	std::unique_ptr<Mesh> triangleMesh;
	std::unique_ptr<Mesh> quadMesh;
	std::unique_ptr<Mesh> lineMesh;
	std::unique_ptr<Mesh> sphereMesh;

	//[ Draw用の変数 ]
	//TRIANGLE
	UINT kMaxTriangle = 512;
	UINT indexTriangle = 0;
	uint8_t const kNumVertexPerTriangle = 3;
	//QUAD	
	UINT kMaxQuad = 4000;
	UINT indexQuad = 0;
	uint8_t const kNumIndexPerQuad = 6;
	uint8_t const kNumVertexPerQuad = 4;
	//LINE
	UINT kMaxLine = 256;
	UINT indexLine = 0;
	uint8_t const kNumVertexPerLine = 2;
	//Sphere
	UINT kMaxSphere = 4;
	UINT indexSphere = 0;
	float const kSubdivision = 18;
	int const kNumVertexPerSphere = (int)kSubdivision * (int)kSubdivision * 4;
	int const kNumIndexPerSphere = (int)kSubdivision * (int)kSubdivision * 6;

};