#include "NormalBlockCharacter.h"

void NormalBlockCharacter::Update(Matrix4 vpMat_, int GameObjState_, float stateCount)
{
	cube.Update(vpMat_);

}

void NormalBlockCharacter::Initialize(Transform4* objectTrans_)
{
	cube.trans.BeChildren(objectTrans_);

}

void NormalBlockCharacter::Draw(DrawMode mode_)
{
	cube.Draw();

}

NormalBlockCharacter::NormalBlockCharacter()
{
	int t[6];
	Fill(t, (int)TagTexture::kNormalBlock);
	cube.Initialize(1.2f, 1.2f, 1.2f, { 0,0,0 }, t);

}

