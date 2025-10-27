#include "SimpleBone.h"

void SimpleBone::Initialize(Vector3 pos_, Vector3 size_, Vector3 targetDir_)
{
	//上面 = 0 , 底面 = 1 , 側面(左) = 2 , 側面(右) = 3 , 側面(奥) = 4 , 側面(手前) = 5
	int tex1[] = { 5,5,5,5,5,5 };
	jointPart.Initialize(size_.x*0.5f, size_.y * 0.5f, size_.z * 0.5f, pos_, tex1);
	//向く方向を設定
	jointPart.trans.targetDir = targetDir_;
}

void SimpleBone::Update(Matrix4 vpMat_)
{
	jointPart.Update(vpMat_);
}

void SimpleBone::Draw()
{
	if (showBone)
	{
		jointPart.Draw();
	}
}

void SimpleBone::Debug()
{

}

