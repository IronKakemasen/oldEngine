#pragma once
#include "../BaseClassStructures/Transform/Transform.h"
#include "../CounterStructures/CounterAndTime.h"
#include "../BenriFunctionStructures/BenriFunctions.h"
#include "../EasingFuctionStructures/EasingFunctions.h"


struct CameraBehavior
{
	Transform4 trans;
	Matrix4 viewProjectionMat;


	//デバッグカメラ切り替え専用（いつか修正）
	inline static bool switcher = true;

	//viewProjection行列の作成
	Matrix4 CreateViewProjectionMatrix();
	inline Matrix4* GetViewProjectionMat(){return &viewProjectionMat;};

	inline Transform4* GetTrans(){return &trans;};
	void SendCameraInfomation();

	virtual void Update() = 0;
	virtual void Initialize() = 0;
	virtual void Debug() = 0;
};

