#include "Sprite.h"


void Sprite::InitializeSprite(Vector3 screen_pos_, float width_, float height_, int texHandle_, Vector4<float> color_)
{
	SetVertex(width_, -height_);
	SetNormal();
	SetTexcoord();
	

	texHandle = texHandle_;
	trans.pos = screen_pos_;
	color = color_;
}

void Sprite::Draw(int enableLighting_)
{
	NoviceLike::DrawQuad(leftTop, rightTop, leftBottom, rightBottom, texHandle, color, trans.mat, 0, kSolid, trans.uvMat);
}

void Sprite::Update(Matrix4 vpMat_)
{
	if (world_followPos)
	{
		Vector2 scr = ConvertToScreen(*world_followPos, vpMat_);
		trans.pos = { offSet.u + scr.u,offSet.v + scr.v,0.0f };
	}

	trans.SetUVMat();
	trans.SetWorldMatBasedQuaternion();
	trans.SetWVP_Mat(Get_Orthographic3D(0.0f, WINDOW_W, 0.0f, WINDOW_H));

}

void Sprite::AdjustForSpriteSheet(float numSprites)
{
	float const rate = 1.0f / numSprites;
	float const half = rate * 0.5f;

	SetTexcoord({ -half,-0.5f }, { half,-0.5f }, { half,0.5f }, { -half,0.5f });
	trans.uvPos = { half,0.5f };
}
