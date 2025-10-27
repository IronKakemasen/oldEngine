#pragma once
#include "../Triangle/Triangle.h"
#include "../../../ResourceStructures/MapTextures.h"

#define CENTROID _Equilateraltriangle{0.5f,0.70710678118f}

struct TetrahedraBehavior :PrimitiveClassBehavior
{
	// 底面 = 0 , 側面(左) = 1 , 側面(右) = 2 , 奥 = 3 
	Triangle local_surfaces[4];

	static inline int count = -1;

	TetrahedraBehavior();
	
	virtual void Update(Matrix4 vpMat_) = 0;
	virtual void Draw(int enableLighting_ = true) = 0;
	
	void SynchronizeTexAndColor();

	void SetSurface(TagTexture texTag_, Vector4<float> color_ = { 255,255,255,255 },
		Vector3 local_leftPos_ = { 0.8164966f, 0.3333333f, -0.4714045f }, Vector3 local_topPos_ = { 0.0f, 0.3333333f, 0.942809f },
		Vector3 local_rightPos_ = { 0.0f,-1.0f,0.0f }, Vector3 furtherPos_ = { -0.8164966f, 0.3333333f, -0.4714045f });
	
	void Initialize(Vector3 pos_,TagTexture texTag_, Vector4<float> color_ = { 255,255,255,255 },
		Vector3 local_leftPos_ = { 0.8164966f, 0.3333333f, -0.4714045f },
		Vector3 local_topPos_ = { 0.0f, 0.3333333f, 0.942809f },
		Vector3 local_rightPos_ = { 0.0f,-1.0f,0.0f },
		Vector3 furtherPos_ = { -0.8164966f, 0.3333333f, -0.4714045f });

	void UV_Oyakoka();

};
