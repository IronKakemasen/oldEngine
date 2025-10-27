#pragma once
#include "../TetrahedraBehavior.h"

struct DisjointedTetrahedra :TetrahedraBehavior
{
	DisjointedTetrahedra();

	virtual void Update(Matrix4 vpMat_)override;
	virtual void Draw(int enableLighting_ = true)override;
	void Debug();
};

