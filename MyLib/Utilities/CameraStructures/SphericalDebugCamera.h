#pragma once
#include "CameraBehavior.h"

class SphericalDebugCamera :public CameraBehavior
{	
private:
	//球面座標
	Vector3 sphericalCoordinates;
	Vector3 centerPos;

public:

	virtual void Update() override;
	virtual void Initialize() override;
	virtual void Debug() override;

	SphericalDebugCamera();

};

