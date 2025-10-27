#pragma once

#include "../Quad/Quad.h"

#define C_TOP 0
#define C_BOTTOM 1
#define C_LEFT 2
#define C_RIGHT 3
#define C_BEYOND 4
#define C_FRONT 5

struct CubeBehavior :PrimitiveClassBehavior
{
	static inline int count = -1;
	Vector3 size;

	//上面 = 0 , 底面 = 1 , 側面(左) = 2 , 側面(右) = 3 , 側面(奥) = 4 , 側面(手前) = 5
	Quad local_surfaces[6];

	void SetSurface(float width_, float height_, float depth_, int* texHandles);

	CubeBehavior();

	virtual void Update(Matrix4 vpMat_) = 0;
	virtual void Draw(int enableLighting_ = true) = 0;

	void Initialize(float width_, float height_, float depth_, Vector3 local_pos_,
		int* texHandles, Vector4<float> commonColor_ = { 255,255,255,255 });
	void ConnectVertexes();
	float GetBotttomMinPos();
	void SetNormal();
	void UV_Oyakoka();
	void SynchronizeTexAndColor();
	void UseAsSpecial();
	void SynchronizeColor();
	void Purge();
};
