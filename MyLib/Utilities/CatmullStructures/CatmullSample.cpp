#include "CatmullSample.h"
#include "../BenriFunctionStructures/BenriFunctions.h"

void CatmullSample::Update(Matrix4 vpMat_)
{
	t += inv_;

	trans.SetWorldMatBasedQuaternion();
	trans.SetWVP_Mat(vpMat_);

	for (auto itr = controllPoints.begin(); itr != controllPoints.end(); ++itr)
	{
		(*itr).get()->trans.SetWorldMatBasedQuaternion();
	}

	int k = 0;
	for (size_t i = 1; i <= numCorners; i++)
	{
		//配置する点の数
		for (float t = dt; t < 1.0f; t += dt, ++k)
		{
			//終点
			points[k] = GetComputedNum(i, t);
			//controllPointss(endPoint, t,(int)i);
		}
	}
}

void CatmullSample::Draw()
{
	float const alpha = 30.0f;
	Vector3 tmp = { tmpCoe.x * sinf(t*2.0f),tmpCoe.y * cos(t),tmpCoe.x * cos(t * 2.0f) };
	Min(tmp.x, 30.0f);
	Min(tmp.y, 30.0f);
	Min(tmp.z, 30.0f);

	float cnt = 0;
	for (auto itr = points.begin(); itr != points.end(); ++itr)
	{
		cnt++;
		float rate = cnt / (float)points.size();
		Vector4<float> col = { 0.0f,tmp.y * rate ,tmp.x ,alpha };
		NoviceLike::DrawLine((*itr), (*itr) + Vector3{ 0,radius,0 },
			0, col, trans.mat, trans.uvMat);
	}

	cnt = 0;
	for (auto itr = points.begin(); itr != points.end(); ++itr)
	{
		cnt++;
		float rate = cnt / (float)points.size();
		Vector4<float> col = { tmp.x * rate ,0.0f ,tmp.y ,alpha };
		NoviceLike::DrawLine((*itr), {0,0,0},
			0, col, trans.mat, trans.uvMat);
	}

	cnt = 0;
	for (auto itr = points.begin(); itr != points.end(); ++itr)
	{
		cnt++;
		float rate = cnt / (float)points.size();
		Vector4<float> col = { tmp.x ,tmp.y * rate ,0.0f ,alpha };
		NoviceLike::DrawLine((*itr) + Vector3{ 0,radius,0 }, Vector3{ 0,radius,0 },
			0, col, trans.mat, trans.uvMat);
	}


}


CatmullSample::CatmullSample()
{
	Set(0.01f, 4, 0.25f, { 0.0f,1.225f,0.0f});
	float divAng = 360.0f / numCorners;

	//制御点の初期化
	for (int i = -1; i <= numCorners + 1; i++)
	{
		float radian = GetRadian(45.0f + divAng * (float)i);
		Vector3 tmp = { sinf(radian) * radius ,0.0f,-cosf(radian) * radius };

		controllPoints.emplace_back(std::make_unique<DisjointedCube>());
		int tex[6];
		Fill(tex, 0);
		controllPoints[i + 1]->Initialize(0.25f, 0.25f, 0.25f, tmp, tex);
		controllPoints[i + 1]->trans.parent_trans = &trans;
	}

	for (int i = 0; i < kNumModel; ++i)
	{
		points.emplace_back();
	}
}

void CatmullSample::ControllPoints(Vector3& edPoint, float rate, int No)
{
	No;
	float kPi = 3.14159265359f;
	edPoint.y += cosf(kPi * rate);
}

void CatmullSample::Initialize()
{

}
