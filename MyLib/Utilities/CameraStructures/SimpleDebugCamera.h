#pragma once
#include "CameraBehavior.h"

class SimpleDebugCamera :public CameraBehavior
{
private:

public:

	//Constructor
	SimpleDebugCamera();
	//Destructor
	~SimpleDebugCamera(){};
	virtual void Update() override;
	virtual void Initialize() override;
	virtual void Debug() override;

};
