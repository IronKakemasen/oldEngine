#include "SampleCharacter.h"

SampleCharacter::SampleCharacter()
{
	int t[6];
	Fill(t, (int)TagTexture::kPlayer);
	cube.Initialize(1.2f, 1.2f, 1.2f, { 0,0,0 }, t);

}


void SampleCharacter::Update(Matrix4 vpMat_, int GameObjState_ , float stateCount )
{
	cube.Update(vpMat_);

}
	
void SampleCharacter::Initialize(Transform4* objectTrans_)
{
	cube.trans.BeChildren(objectTrans_);

}

void SampleCharacter::Draw(DrawMode mode_)
{
	cube.Draw();

}
