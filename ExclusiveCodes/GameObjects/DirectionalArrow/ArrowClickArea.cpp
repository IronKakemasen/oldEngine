#include "ArrowClickArea.h"



ArrowClickArea::ArrowClickArea(Vector3 pos, MovableBlock::ArrowDirection dir)
    : position(pos), direction(dir)
{

	chara.Initialize(pos, MapModels::GetModelMusic(TagModel::kChain));


	////ジョイントキューブの初期化
	int tex[6] = { 1,1,1,1,1,1 };
	//jointCube.Initialize(0.1f, 0.1f, 0.1f, { -3.0f,0.5f,0.0f }, tex);
	////jointCube.drawMode = DrawMode::kWireFrame;
	//jointCube.texHandle = (int)TagTexture::kTestGoal;
	//jointCube.color = { 255,255,0,255 };
	//jointCube.SynchronizeTexAndColor();
	//jointCube.trans.pos = pos;

	//オーラジョイントキューブの初期化
	auraJointCube.Initialize(3.0f, 3.0f, 0.01f, pos, tex);
	//カードのUVを変化させる
	auraJointCube.texHandle = (int)TagTexture::kParticle0;
	auraJointCube.color = { 0, 255, 255, 200 };
	auraJointCube.SynchronizeTexAndColor();

}

void ArrowClickArea::Update(const Matrix4& vpMat_)
{
	chara.Update(vpMat_);

	auraEasingTime += 1.0f / 60.0f;
	if (auraEasingTime > auraEasingDuration) {
		auraEasingTime -= auraEasingDuration;
	}

	auraJointCube.Update(vpMat_);
}

void ArrowClickArea::Draw()
{
	//ジョイントキューブの描画
	chara.Draw();

	if (isAuraEnabled)
	{
		NoviceLike::SetBlendMode(kBlendModeAdd);

		for (int i = 0; i < 3; ++i)
		{
			float offset = i * 0.03f;
			float loopTime = fmodf((auraEasingTime / auraEasingDuration) + offset, 1.0f);

			float scaleXY = Easing::Lerp(1.0f, 1.25f, loopTime);
			float alpha = Easing::Lerp(0.8f, 0.0f, loopTime);

			auraJointCube.trans.scale = { scaleXY, scaleXY, 0.2f };

			auraJointCube.color.w = static_cast<uint8_t>(alpha * 255);
			auraJointCube.SynchronizeColor();

			auraJointCube.Draw();
		}

		NoviceLike::SetBlendMode(kBlendModeNormal);
	}
}