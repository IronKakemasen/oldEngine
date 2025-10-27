#pragma once
#include "../ChipBehavior.h"
#include "../../../Characters/NormalBlockCharacter/NormalBlockCharacter.h"

struct NormalBlock:Chip
{
	NormalBlockCharacter chara;

	NormalBlock();

	virtual void Update(Matrix4 vpMat_)override;
	virtual void Draw()override;

};

