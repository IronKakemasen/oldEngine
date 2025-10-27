#include "Quaternion.h"

Quaternion::Quaternion()
{

}

Quaternion::Quaternion(Vector3 axis_)
{
	axis = axis_;
}

Vector4<float> Quaternion::Get()
{
	return GetQuaternion(axis, deltaTheta);
}

QuaternionDefaultSet::QuaternionDefaultSet()
{
	x_axis.axis = VECTOR_RIGHT;
	y_axis.axis = VECTOR_UP;
	z_axis.axis = VECTOR_BEYOND;

}

Vector4<float> QuaternionDefaultSet::GetComposite()
{
	Vector4<float> ret;

	Vector4<float> rightQua = x_axis.Get();
	Vector4<float> upQua = y_axis.Get();
	Vector4<float> beyondQua = z_axis.Get();

	ret = CompositeQuaternion(upQua, rightQua);
	ret = CompositeQuaternion(ret, beyondQua);

	return ret;

}

Vector4<float> ConvertToQuaternion(Matrix4 m)
{
	float elem[4];
	elem[0] = m.m[0][0] - m.m[1][1] - m.m[2][2] + 1.0f;
	elem[1] = -m.m[0][0] + m.m[1][1] - m.m[2][2] + 1.0f;
	elem[2] = -m.m[0][0] - m.m[1][1] + m.m[2][2] + 1.0f;
	elem[3] = m.m[0][0] + m.m[1][1] + m.m[2][2] + 1.0f;

	int biggestIdx = 0;

	for (int i = 0; i < 4; i++)
	{
		if (elem[i] > elem[biggestIdx])
		{
			biggestIdx = i;
		}
	}

	if (elem[biggestIdx] < 0)
	{
		return Vector4<float>();
	}

	float q[4];
	float v = sqrtf(elem[biggestIdx]) * 0.5f;
	q[biggestIdx] = v;
	float mult = 0.25f / v;

	switch (biggestIdx)
	{
	case 0:
		q[1] = (m.m[1][0] + m.m[0][1]) * mult;
		q[2] = (m.m[0][2] + m.m[2][0]) * mult;
		q[3] = (m.m[2][1] - m.m[1][2]) * mult;
		break;
	case 1:
		q[0] = (m.m[1][0] + m.m[0][1]) * mult;
		q[2] = (m.m[2][1] + m.m[1][2]) * mult;
		q[3] = (m.m[0][2] - m.m[2][0]) * mult;
		break;
	case 2:
		q[0] = (m.m[0][2] + m.m[2][0]) * mult;
		q[1] = (m.m[2][1] + m.m[1][2]) * mult;
		q[3] = (m.m[1][0] - m.m[0][1]) * mult;
		break;
	case 3:
		q[0] = (m.m[2][1] - m.m[1][2]) * mult;
		q[1] = (m.m[0][2] - m.m[2][0]) * mult;
		q[2] = (m.m[1][0] - m.m[0][1]) * mult;
		break;
	}

	return Vector4<float> {q[0], q[1], q[2], q[3]};
}

Vector4<float> LookAt(Vector3 target_, Vector3 pos_)
{
	Vector3 z = target_.GetNormalized();
	Vector3 ver = { 0.0f, 1.0f, 0.0f};
	Vector3 x = (ver.GetCross(z)).GetNormalized();
	Vector3 y = (z.GetCross(x)).GetNormalized();

	Matrix4 m =
	{
		x.x,y.x,z.x,0.0f,
		x.y,y.y,z.y,0.0f,
		x.z,y.z,z.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return ConvertToQuaternion(m);
}
