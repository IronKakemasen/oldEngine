#include "NormalBlock.h"

NormalBlock::NormalBlock()
{
	tagChip = (int)TagChip::kBlock;
	chara.Initialize(&trans);
}

void NormalBlock::Update(Matrix4 vpMat_)
{
	chara.Update(vpMat_);

}

void NormalBlock::Draw()
{
	chara.Draw();
}
