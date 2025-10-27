#include "VectorAndMatrix.h"
#include <algorithm>

//[ Matrix3 ]
Matrix3 Matrix3::operator+=(Matrix3 other)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int k = 0; k < 3; ++k)
		{
			m[i][k] += other.m[i][k];
		}
	}

	return *this;
}
Matrix3 Matrix3 ::operator-=(Matrix3 other)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int k = 0; k < 3; ++k)
		{
			m[i][k] -= other.m[i][k];
		}
	}

	return *this;
}

Matrix3 ::Matrix3(float zero_)
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
};

Matrix3::Matrix3(float a0, float a1, float a2, float a3, float a4, float a5,
	float a6, float a7, float a8)
{
	m[0][0] = a0; m[0][1] = a1; m[0][2] = a2;
	m[1][0] = a3; m[1][1] = a4; m[1][2] = a5;
	m[2][0] = a6; m[2][1] = a7; m[2][2] = a8;
}

Matrix3 Matrix3::GetMultiply(Matrix3* const other)
{
	Matrix3 ret_mat;

	ret_mat.m[0][0] = m[0][0] * other->m[0][0] + m[0][1] * other->m[1][0] + m[0][2] * other->m[2][0];
	ret_mat.m[0][1] = m[0][0] * other->m[0][1] + m[0][1] * other->m[1][1] + m[0][2] * other->m[2][1];
	ret_mat.m[0][2] = m[0][0] * other->m[0][2] + m[0][1] * other->m[1][2] + m[0][2] * other->m[2][2];
	ret_mat.m[1][0] = m[1][0] * other->m[0][0] + m[1][1] * other->m[1][0] + m[1][2] * other->m[2][0];
	ret_mat.m[1][1] = m[1][0] * other->m[0][1] + m[1][1] * other->m[1][1] + m[1][2] * other->m[2][1];
	ret_mat.m[1][2] = m[1][0] * other->m[0][2] + m[1][1] * other->m[1][2] + m[1][2] * other->m[2][2];
	ret_mat.m[2][0] = m[2][0] * other->m[0][0] + m[2][1] * other->m[1][0] + m[2][2] * other->m[2][0];
	ret_mat.m[2][1] = m[2][0] * other->m[0][1] + m[2][1] * other->m[1][1] + m[2][2] * other->m[2][1];
	ret_mat.m[2][2] = m[2][0] * other->m[0][2] + m[2][1] * other->m[1][2] + m[2][2] * other->m[2][2];

	return ret_mat;
}

//[ Matrix4 ]
Matrix4 Matrix4::operator+=(Matrix4 other)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int k = 0; k < 4; ++k)
		{
			m[i][k] += other.m[i][k];
		}
	}

	return *this;
}
Matrix4 Matrix4::operator-=(Matrix4 other)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int k = 0; k < 4; ++k)
		{
			m[i][k] -= other.m[i][k];
		}
	}

	return *this;
}


Matrix4::Matrix4(float zero_)
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	zero_;
};

Matrix4::Matrix4(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9,
	float a10, float a11, float a12, float a13, float a14, float a15)
{
	m[0][0] = a0; m[0][1] = a1; m[0][2] = a2; m[0][3] = a3;
	m[1][0] = a4; m[1][1] = a5; m[1][2] = a6; m[1][3] = a7;
	m[2][0] = a8; m[2][1] = a9; m[2][2] = a10; m[2][3] = a11;
	m[3][0] = a12; m[3][1] = a13; m[3][2] = a14; m[3][3] = a15;
}

Matrix4 Matrix4::Multiply(Matrix4 const& other)
{
	Matrix4 ret_mat;

	ret_mat.m[0][0] = m[0][0] * other.m[0][0] + m[0][1] * other.m[1][0] +
		m[0][2] * other.m[2][0] + m[0][3] * other.m[3][0];
	ret_mat.m[0][1] = m[0][0] * other.m[0][1] + m[0][1] * other.m[1][1] +
		m[0][2] * other.m[2][1] + m[0][3] * other.m[3][1];
	ret_mat.m[0][2] = m[0][0] * other.m[0][2] + m[0][1] * other.m[1][2] +
		m[0][2] * other.m[2][2] + m[0][3] * other.m[3][2];
	ret_mat.m[0][3] = m[0][0] * other.m[0][3] + m[0][1] * other.m[1][3] +
		m[0][2] * other.m[2][3] + m[0][3] * other.m[3][3];

	ret_mat.m[1][0] = m[1][0] * other.m[0][0] + m[1][1] * other.m[1][0] +
		m[1][2] * other.m[2][0] + m[1][3] * other.m[3][0];
	ret_mat.m[1][1] = m[1][0] * other.m[0][1] + m[1][1] * other.m[1][1] +
		m[1][2] * other.m[2][1] + m[1][3] * other.m[3][1];
	ret_mat.m[1][2] = m[1][0] * other.m[0][2] + m[1][1] * other.m[1][2] +
		m[1][2] * other.m[2][2] + m[1][3] * other.m[3][2];
	ret_mat.m[1][3] = m[1][0] * other.m[0][3] + m[1][1] * other.m[1][3] +
		m[1][2] * other.m[2][3] + m[1][3] * other.m[3][3];

	ret_mat.m[2][0] = m[2][0] * other.m[0][0] + m[2][1] * other.m[1][0] +
		m[2][2] * other.m[2][0] + m[2][3] * other.m[3][0];
	ret_mat.m[2][1] = m[2][0] * other.m[0][1] + m[2][1] * other.m[1][1] +
		m[2][2] * other.m[2][1] + m[2][3] * other.m[3][1];
	ret_mat.m[2][2] = m[2][0] * other.m[0][2] + m[2][1] * other.m[1][2] +
		m[2][2] * other.m[2][2] + m[2][3] * other.m[3][2];
	ret_mat.m[2][3] = m[2][0] * other.m[0][3] + m[2][1] * other.m[1][3] +
		m[2][2] * other.m[2][3] + m[2][3] * other.m[3][3];

	ret_mat.m[3][0] = m[3][0] * other.m[0][0] + m[3][1] * other.m[1][0] +
		m[3][2] * other.m[2][0] + m[3][3] * other.m[3][0];
	ret_mat.m[3][1] = m[3][0] * other.m[0][1] + m[3][1] * other.m[1][1] +
		m[3][2] * other.m[2][1] + m[3][3] * other.m[3][1];
	ret_mat.m[3][2] = m[3][0] * other.m[0][2] + m[3][1] * other.m[1][2] +
		m[3][2] * other.m[2][2] + m[3][3] * other.m[3][2];
	ret_mat.m[3][3] = m[3][0] * other.m[0][3] + m[3][1] * other.m[1][3] +
		m[3][2] * other.m[2][3] + m[3][3] * other.m[3][3];

	return ret_mat;

}
float Matrix4::GetDeterminant4x4()
{
	const float a11 = m[0][0];
	const float a12 = m[0][1];
	const float a13 = m[0][2];
	const float a14 = m[0][3];

	const float a21 = m[1][0];
	const float a22 = m[1][1];
	const float a23 = m[1][2];
	const float a24 = m[1][3];

	const float a31 = m[2][0];
	const float a32 = m[2][1];
	const float a33 = m[2][2];
	const float a34 = m[2][3];

	const float a41 = m[3][0];
	const float a42 = m[3][1];
	const float a43 = m[3][2];
	const float a44 = m[3][3];


	return a11 * a22 * a33 * a44 + a11 * a23 * a34 * a42 + a11 * a24 * a32 * a43
		+ a12 * a21 * a34 * a43 + a12 * a23 * a31 * a44 + a12 * a24 * a33 * a41
		+ a13 * a21 * a32 * a44 + a13 * a22 * a34 * a41 + a13 * a24 * a31 * a42
		+ a14 * a21 * a33 * a42 + a14 * a22 * a31 * a43 + a14 * a23 * a32 * a41
		- a11 * a22 * a34 * a43 - a11 * a23 * a32 * a44 - a11 * a24 * a33 * a42
		- a12 * a21 * a33 * a44 - a12 * a23 * a34 * a41 - a12 * a24 * a31 * a43
		- a13 * a21 * a34 * a42 - a13 * a22 * a31 * a44 - a13 * a24 * a32 * a41
		- a14 * a21 * a32 * a43 - a14 * a22 * a33 * a41 - a14 * a23 * a31 * a42;

}//4x4行列の行列式を返す関数
Matrix4 Matrix4::GetInversed()
{
	Matrix4 ret_mat;

	const float a11 = m[0][0];
	const float a12 = m[0][1];
	const float a13 = m[0][2];
	const float a14 = m[0][3];

	const float a21 = m[1][0];
	const float a22 = m[1][1];
	const float a23 = m[1][2];
	const float a24 = m[1][3];

	const float a31 = m[2][0];
	const float a32 = m[2][1];
	const float a33 = m[2][2];
	const float a34 = m[2][3];

	const float a41 = m[3][0];
	const float a42 = m[3][1];
	const float a43 = m[3][2];
	const float a44 = m[3][3];

	ret_mat.m[0][0] = a22 * a33 * a44 + a23 * a34 * a42 + a24 * a32 * a43 - a22 * a34 * a43 - a23 * a32 * a44 - a24 * a33 * a42;
	ret_mat.m[0][1] = a12 * a34 * a43 + a13 * a32 * a44 + a14 * a33 * a42 - a12 * a33 * a44 - a13 * a34 * a42 - a14 * a32 * a43;
	ret_mat.m[0][2] = a12 * a23 * a44 + a13 * a24 * a42 + a14 * a22 * a43 - a12 * a24 * a43 - a13 * a22 * a44 - a14 * a23 * a42;
	ret_mat.m[0][3] = a12 * a24 * a33 + a13 * a22 * a34 + a14 * a23 * a32 - a12 * a23 * a34 - a13 * a24 * a32 - a14 * a22 * a33;
	ret_mat.m[1][0] = a21 * a34 * a43 + a23 * a31 * a44 + a24 * a33 * a41 - a21 * a33 * a44 - a23 * a34 * a41 - a24 * a31 * a43;
	ret_mat.m[1][1] = a11 * a33 * a44 + a13 * a34 * a41 + a14 * a31 * a43 - a11 * a34 * a43 - a13 * a31 * a44 - a14 * a33 * a41;
	ret_mat.m[1][2] = a11 * a24 * a43 + a13 * a21 * a44 + a14 * a23 * a41 - a11 * a23 * a44 - a13 * a24 * a41 - a14 * a21 * a43;
	ret_mat.m[1][3] = a11 * a23 * a34 + a13 * a24 * a31 + a14 * a21 * a33 - a11 * a24 * a33 - a13 * a21 * a34 - a14 * a23 * a31;
	ret_mat.m[2][0] = a21 * a32 * a44 + a22 * a34 * a41 + a24 * a31 * a42 - a21 * a34 * a42 - a22 * a31 * a44 - a24 * a32 * a41;
	ret_mat.m[2][1] = a11 * a34 * a42 + a12 * a31 * a44 + a14 * a32 * a41 - a11 * a32 * a44 - a12 * a34 * a41 - a14 * a31 * a42;
	ret_mat.m[2][2] = a11 * a22 * a44 + a12 * a24 * a41 + a14 * a21 * a42 - a11 * a24 * a42 - a12 * a21 * a44 - a14 * a22 * a41;
	ret_mat.m[2][3] = a11 * a24 * a32 + a12 * a21 * a34 + a14 * a22 * a31 - a11 * a22 * a34 - a12 * a24 * a31 - a14 * a21 * a32;
	ret_mat.m[3][0] = a21 * a33 * a42 + a22 * a31 * a43 + a23 * a32 * a41 - a21 * a32 * a43 - a22 * a33 * a41 - a23 * a31 * a42;
	ret_mat.m[3][1] = a11 * a32 * a43 + a12 * a33 * a41 + a13 * a31 * a42 - a11 * a33 * a42 - a12 * a31 * a43 - a13 * a32 * a41;
	ret_mat.m[3][2] = a11 * a23 * a42 + a12 * a21 * a43 + a13 * a22 * a41 - a11 * a22 * a43 - a12 * a23 * a41 - a13 * a21 * a42;
	ret_mat.m[3][3] = a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - a11 * a23 * a32 - a12 * a21 * a33 - a13 * a22 * a31;

	float det = GetDeterminant4x4();

	if (det != 0.0f)
	{
		float inv_det = 1.0f / det;

		for (int i = 0; i < 4; i++)
		{
			for (int k = 0; k < 4; k++)
			{
				ret_mat.m[i][k] *= inv_det;
			}
		}
	}

	return ret_mat;

}//4x4行列の逆行列を取得
//[ Vector2 ]
float Vector2::GetMagnitutde()
{
	return sqrtf(u * u + v * v);
}
Vector2 Vector2::GetNormalized()
{
	float mag = GetMagnitutde();
	if (mag == 0.0f) return { 0,0 };
	return { u / mag,v / mag };
}
float Vector2::GetCos(Vector2 other_)
{
	//正規化する
	Vector2 normalized1 = GetNormalized();
	Vector2 normalized2 = other_.GetNormalized();
	//cos(🍆角)
	return normalized1.u * normalized2.u + normalized1.v * normalized2.v;;

}

float Vector2::GetDot(Vector2 other_)
{
	return u * other_.u + v * other_.v;

}



//[ Vector3 ]
Vector3 Vector3::GetMultiply(Matrix4 src_mat_)
{
	Vector3 ret;

	ret.x = x * src_mat_.m[0][0] + y * src_mat_.m[1][0] + z * src_mat_.m[2][0] + src_mat_.m[3][0];
	ret.y = x * src_mat_.m[0][1] + y * src_mat_.m[1][1] + z * src_mat_.m[2][1] + src_mat_.m[3][1];
	ret.z = x * src_mat_.m[0][2] + y * src_mat_.m[1][2] + z * src_mat_.m[2][2] + src_mat_.m[3][2];


	return ret;
}

float Vector3::GetMagnitutde()
{
	return sqrtf(x * x + y * y + z * z);
}

//Cosを計算して返す
float Vector3::GetCos(Vector3 other_)
{
	float ret_cosTheta = 0.0f;

	//正規化する
	Vector3 normalized1 = GetNormalized();
	Vector3 normalized2 = other_.GetNormalized();
	//cos(🍆角)
	ret_cosTheta =
		normalized1.x * normalized2.x +
		normalized1.y * normalized2.y +
		normalized1.z * normalized2.z;

	return ret_cosTheta;
}

// 正規化ベクトルを返す
Vector3 Vector3::GetNormalized()
{
	float mag = GetMagnitutde();
	float inv_mag = 0;

	if (mag != 0)
		inv_mag = 1.0f / mag;

	Vector3 ret_normalizeVec = { x * inv_mag, y * inv_mag, z * inv_mag };

	return ret_normalizeVec;
}

//外積
Vector3 Vector3::GetCross(Vector3 other)
{
	Vector3 ret;

	ret.x = y * other.z - z * other.y;
	ret.y = z * other.x - x * other.z;
	ret.z = x * other.y - y * other.x;

	return ret;
}

Vector3 Vector3::GetPerpendiculer()
{
	Vector3 ret_vec;

	if (x != 0.0f || y != 0.0f)
	{
		ret_vec = { -y ,x,0.0f };
	}

	else
	{
		ret_vec = { 0.0f ,-z,y };
	}

	return ret_vec;
}
float Vector3::GetDot(Vector3 other_)
{
	return 	x * other_.x + y * other_.y + z * other_.z;
}

float Vector3::Length() const {
	return std::sqrt(x * x + y * y + z * z);
}
// 正規化されたベクトルを返す
Vector3 Vector3::Normalize() const {
	float len = Length();
	if (len == 0.0f) {
		return Vector3(0, 0, 0);  // ゼロ除算回避
	}
	return Vector3(x / len, y / len, z / len);
}


Vector3 operator+(Vector3 const& lVec, Vector3 const& rVec2)
{
	Vector3 ret_vec = { lVec.x + rVec2.x,lVec.y + rVec2.y ,lVec.z + rVec2.z };

	return ret_vec;
}

bool operator==(Vector3 const& lVec, Vector3  const& rVec2)
{
	bool ret = false;
	if (lVec.x == rVec2.x && lVec.y == rVec2.y && lVec.z == rVec2.z) ret = true;
	else ret = false;
	
	return ret;
	
}
//[ Vector4 ]
Vector4<float> Vector4<float>::operator+=(float const addNum)
{
	x += addNum;
	y += addNum;
	z += addNum;

	return *this;
}
Vector4<float> Vector4<float>::operator=(float const num)
{
	x = num;
	y = num;
	z = num;

	return *this;
}
//大きさをゲット
float Vector4<float>::GetMagnitutde()
{
	return sqrtf(x * x + y * y + z * z);
}
//正規化ベクトルを返す
Vector4<float> Vector4<float>::GetNormalized()
{
	float mag = GetMagnitutde();
	float inv_mag = 0;

	if (mag != 0) inv_mag = 1.0f / mag;

	Vector4<float> ret_normalizeVec = { x * inv_mag ,y * inv_mag,z * inv_mag,1.0f };

	return ret_normalizeVec;
}

Vector4<float> Vector4<float>::GetNormalizedForQuaternion()
{
	float mag = sqrtf(x * x + y * y + z * z + w * w);
	float inv_mag = 0;

	if (mag != 0) inv_mag = 1.0f / mag;

	Vector4<float> ret_normalizeVec = { x * inv_mag ,y * inv_mag,z * inv_mag,w * inv_mag };

	return ret_normalizeVec;
}

//正規化された内積を計算して返す
float Vector4<float>::GetCos(Vector4<float> other_)
{
	float ret_cosTheta = 0.0f;

	//正規化する
	Vector4<float> normalized1 = GetNormalized();
	Vector4<float> normalized2 = other_.GetNormalized();
	//cos(🍆角)
	ret_cosTheta =
		normalized1.x * normalized2.x +
		normalized1.y * normalized2.y +
		normalized1.z * normalized2.z;

	return ret_cosTheta;
}

float Vector4<float>::Dot(Vector4<float> other_)
{
	return 	x * other_.x + y * other_.y + z * other_.z;
}
float Vector4<float>::GetCosForQuaternion(Vector4<float> other_)
{
	//cos(🍆角)
	float ret_cosTheta =
		x * other_.x +
		y * other_.y +
		z * other_.z +
		w * other_.w;


	return ret_cosTheta;
}

//差分単位ベクトルを返す
Vector4<float> Vector4<float>::GetNormalizedDiffVector4(Vector4<float> dst_pos, Vector4<float> my_pos)
{
	Vector4<float> diff;

	//差分を計算
	diff = dst_pos - my_pos;

	return diff.GetNormalized();
}
//マトリックスとベクタの積
Vector4<float> Vector4<float>::GetMultiply(Matrix4 src_mat)
{
	Vector4<float> ret_Vector4;

	ret_Vector4.x = x * src_mat.m[0][0] + y * src_mat.m[1][0] +
		z * src_mat.m[2][0] + w * src_mat.m[3][0];

	ret_Vector4.y = x * src_mat.m[0][1] + y * src_mat.m[1][1] +
		z * src_mat.m[2][1] + w * src_mat.m[3][1];

	ret_Vector4.z = x * src_mat.m[0][2] + y * src_mat.m[1][2] +
		z * src_mat.m[2][2] + w * src_mat.m[3][2];

	ret_Vector4.w = x * src_mat.m[0][3] + y * src_mat.m[1][3] +
		z * src_mat.m[2][3] + w * src_mat.m[3][3];

	return ret_Vector4;
}
//クロス積を返す
Vector4<float> Vector4<float>::GetCross(Vector4<float> other)
{
	Vector4<float> ret_Vector4;

	ret_Vector4.x = y * other.z - z * other.y;
	ret_Vector4.y = z * other.x - x * other.z;
	ret_Vector4.z = x * other.y - y * other.x;

	return ret_Vector4;
}

Vector4<float> Vector4<float>::GetPerpendiculer(Vector4<float> point_)
{
	Vector4<float> ret_vec;

	if (point_.x != 0.0f || point_.y != 0.0f)
	{
		ret_vec = { -point_.y ,point_.x,0.0f,1.0f };
	}

	else
	{
		ret_vec = { 0.0f ,-point_.z,point_.y,1.0f };
	}

	return ret_vec;
}

Vector4<float> GetQuaternion(Vector3 v_, float deltaTheta_)
{
	float const degreeConverter = 3.141592653589f / 180.0f;
	float radian = deltaTheta_ * degreeConverter;
	Vector3 buff = v_.GetNormalized();
	Vector4<float> quaternion = { buff.x * sinf(radian*0.5f),buff.y * sinf(radian * 0.5f)
		,buff.z * sinf(radian * 0.5f) ,cosf(radian * 0.5f) };

	return quaternion;
}

Vector4<float> CompositeQuaternion(Vector4<float> q_,Vector4<float> p_)
{
	//Matrix4 compositeMatrix =
	//{
	//	q_.w , -q_.z, q_.y, q_.x,
	//	q_.z , q_.w, -q_.x, q_.y,
	//	-q_.y , q_.x, q_.w, q_.z,
	//	-q_.x , -q_.y, -q_.z, q_.w,
	//};

	Matrix4 compositeMatrix =
	{
		q_.w , q_.z, -q_.y, -q_.x,
		-q_.z , q_.w, q_.x, -q_.y,
		q_.y , -q_.x, q_.w, -q_.z,
		q_.x , q_.y, q_.z, q_.w,
	};


	return p_.GetMultiply(compositeMatrix);
}



Vector3 operator*(Vector3 const& dst_vec, float const multipleNum)
{
	return { dst_vec.x * multipleNum,dst_vec.y * multipleNum ,dst_vec.z * multipleNum };
}

Vector3  operator-(Vector3  const& lVec, Vector3  const& rVec2)
{
	return { lVec.x - rVec2.x,lVec.y - rVec2.y ,lVec.z - rVec2.z };
}

Vector2 operator+(Vector2 const& lVec_, Vector2 const& rVec_)
{
	return { lVec_.u + rVec_.v,lVec_.u + rVec_.v };
}

Matrix4 operator*(Matrix4 const& dst_mat, float multipleNum)
{
	Matrix4 ret_mat;

	for (int i = 0; i < 4; ++i)
	{
		for (int k = 0; k < 4; ++k)
		{
			ret_mat.m[i][k] = dst_mat.m[i][k] * multipleNum;
		}
	}

	return ret_mat;
}

Vector2 operator-(Vector2  const& lVec_, Vector2  const& rVec_)
{
	return { lVec_.u - rVec_.u,lVec_.v - rVec_.v };

}
Vector2 operator*(Vector2 const& dst_vec_, float const multipleNum)
{
	return { dst_vec_.u * multipleNum,dst_vec_.v * multipleNum };
}


Matrix4 Get_VPMat(Matrix4 cameraMat, Vector3 cameraPos , ProjectionType type_)
{
	Matrix4 ret_mat;

	//ビュー行列作成
	Matrix4 viewMat = cameraMat.GetInversed();
	//射影行列
	Matrix4 projectionMat;

	//平行投影
	if (type_ == kParallel)
	{
		projectionMat = Get_Orthographic3D(
			cameraPos.x - (WINDOW_W * 0.5f), cameraPos.x + (WINDOW_W * 0.5f),
			cameraPos.y + (WINDOW_H * 0.5f), cameraPos.y - (WINDOW_H * 0.5f),
			0.1f, 1000.0f);
	}

	//中心投影
	else if (type_ == kPerspective)
	{
		float aspectRatio = WINDOW_W / WINDOW_H;
		projectionMat = Get_PerspectiveFOV(Torima::torimaFOVy, aspectRatio);
	}

	//viewMat⇔PerspectiveMat
	ret_mat = viewMat.Multiply(projectionMat);

	return  ret_mat;
}

Matrix4 Get_PerspectiveFOV(float fovY_, float aspectRatio_, float nearClip_ , float farClip_ )
{
	const float cotTheta{ 1.0f / tanf(fovY_ * 0.5f) };
	const float inv_frustumHeight{ 1.0f / (farClip_ - nearClip_) };

	return Matrix4
	{
		cotTheta / aspectRatio_ , 0.0f, 0.0f, 0.0f,
		0.0f, cotTheta, 0.0f, 0.0f,
		0.0f, 0.0f, farClip_ * inv_frustumHeight, 1.0f,
		0.0f, 0.0f, -nearClip_ * farClip_ * inv_frustumHeight, 0.0f,
	};
}
Matrix4 Get_ViewportTransformation3D(
	const float left_ , const float top_ ,
	const float windowWidth_ , const float windowHeight_ ,
	const float minDepth_ , const float maxDepth_ )
{
	return Matrix4
	{
		windowWidth_ * 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -windowHeight_ * 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, maxDepth_ - minDepth_, 0.0f,
		left_ + windowWidth_ * 0.5f, top_ + windowHeight_ * 0.5f, minDepth_, 1.0f,
	};
}
Matrix4 Get_Orthographic3D(
	const float l_, const float r_,
	const float t_, const float b_,
	const float zn_, const float zf_)
{
	const float inv_W{ 1.0f / (r_ - l_) };
	const float inv_H{ 1.0f / (t_ - b_) };
	const float inv_D{ 1.0f / (zf_ - zn_) };

	return Matrix4
	{
		2.0f * inv_W, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f * inv_H, 0.0f, 0.0f,
		0.0f, 0.0f, inv_D, 0.0f,
		-(l_ + r_) * inv_W, -(t_ + b_) * inv_H, -zn_ * inv_D, 1.0f,
	};
}

Matrix4 GetQuaternionRotateMat(Vector4<float> quaternion_)
{
	Vector4 <float> c_ = quaternion_;

	float a00 = (c_.x * c_.x) - (c_.y * c_.y) - (c_.z * c_.z) + (c_.w * c_.w);
	float a01 = (c_.x * c_.y - c_.z * c_.w) * 2.0f;
	float a02 = (c_.x * c_.z + c_.y * c_.w) * 2.0f;

	float a10 = (c_.x * c_.y + c_.z * c_.w) * 2.0f;
	float a11 = -(c_.x * c_.x) + (c_.y * c_.y) - (c_.z * c_.z) + (c_.w * c_.w);
	float a12 = (c_.y * c_.z - c_.x * c_.w) * 2.0f;

	float a20 = (c_.x * c_.z - c_.y * c_.w) * 2.0f;
	float a21 = (c_.y * c_.z + c_.x * c_.w) * 2.0f;
	float a22 = -(c_.x * c_.x) - (c_.y * c_.y) + (c_.z * c_.z) + (c_.w * c_.w);

	//return Matrix4
	//{
	//	a00, a01, a02, 0.0f,
	//	a10, a11, a12, 0.0f,
	//	a20, a21, a22, 0.0f,
	//	0.0f,0.0f,0.0f,1.0f,
	//};

	return Matrix4
	{
		a00, a10, a20, 0.0f,
		a01, a11, a21, 0.0f,
		a02, a12, a22, 0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};

}


Matrix4 GetScaleMat(Vector3 scale_)
{
	return
	{
		scale_.x,0.0f,0.0f,0.0f,
		0.0f,scale_.y,0.0f,0.0f,
		0.0f,0.0f,scale_.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix4 GetTranslateMat(Vector3 translate_)
{
	//TranslateMat
	return	
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		translate_.x,translate_.y,translate_.z,1.0f
	};

}

Matrix4 Get_SQrTMat3D(const Vector3& scale_, Vector4<float> quaternion_,const Vector3& translate_)
{
	Matrix4 ret;

	//SRT行列の作成
	Matrix4 scaleMat = GetScaleMat(scale_);
	Matrix4 rotateMat = GetQuaternionRotateMat(quaternion_);
	//TranslateMat
	Matrix4 translateMat = GetTranslateMat(translate_);

	//S⇔R
	ret = scaleMat.Multiply(rotateMat);
	//SR⇔T
	ret = ret.Multiply(translateMat);

	return ret;		//SRT

}

Matrix4 Get_SRTMat3D(const Vector3& scale_, const Vector3& rotateTheta_,const Vector3& translate_)
{
	Matrix4 ret_mat;

	//回転角をradianに変換
	static float const degreeConverter = 3.141592653589f / 180.0f;
	Vector3 rotateRad = rotateTheta_ * degreeConverter;

	//3つの回転軸に対応した行列を作成
	//角度を三角関数に代入して返すラムダ式
	auto substituteRad = [](float rotateRad_) {
		Vector3 ret_vec = { cosf(rotateRad_),sinf(rotateRad_),1.0f};
		return ret_vec; };

	//x軸
	Vector3 circularFunc = substituteRad(rotateRad.x);

	Matrix4 rotateMatX(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, circularFunc.x, circularFunc.y, 0.0f,
		0.0f, -circularFunc.y, circularFunc.x, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	//y軸
	circularFunc = substituteRad(rotateRad.y);
	Matrix4 rotateMatY =
	{
		circularFunc.x,0.0f,-circularFunc.y,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		circularFunc.y,0.0f,circularFunc.x,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	//z軸
	circularFunc = substituteRad(rotateRad.z);
	Matrix4 rotateMatZ =
	{
		circularFunc.x,circularFunc.y,0.0f,0.0f,
		-circularFunc.y,circularFunc.x,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	//3つの回転マトリックスを合成する
	Matrix4 concentratedRotateMat = rotateMatX.Multiply(rotateMatY.Multiply(rotateMatZ));

	//SRT行列の作成
	//ScaleMatrix
	Matrix4 scaleMat =
	{
		scale_.x,0.0f,0.0f,0.0f,
		0.0f,scale_.y,0.0f,0.0f,
		0.0f,0.0f,scale_.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	//TranslateMat
	Matrix4 translateMat =
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		translate_.x,translate_.y,translate_.z,1.0f
	};

	//S⇔R
	ret_mat = scaleMat.Multiply(concentratedRotateMat);
	//SR⇔T
	ret_mat = ret_mat.Multiply(translateMat);

	return ret_mat;		//SRT

}//SRT行列の作成・取得

Matrix4 Get_STRMat3D(const Vector3& scale_, const Vector3& movementTheta_,const Vector3& translate_)
{
	Matrix4 ret_mat;

	//回転角をradianに変換
	static float const degreeConverter = Torima::kPi / 180.0f;
	Vector3 rotateRad = movementTheta_ * degreeConverter;

	//3つの回転軸に対応した行列を作成
	//角度を三角関数に代入して返すラムダ式
	auto substituteRad = [](float movementTheta_) {
		Vector3 ret_vec = { cosf(movementTheta_),sinf(movementTheta_),1.0f};
		return ret_vec; };

	//x軸
	Vector3 circularFunc = substituteRad(rotateRad.x);
	Matrix4 movementMatX =
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,circularFunc.x,circularFunc.y,0.0f,
		0.0f,-circularFunc.y,circularFunc.x,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	//y軸
	circularFunc = substituteRad(rotateRad.y);
	Matrix4 movementMatY =
	{
		circularFunc.x,0.0f,-circularFunc.y,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		circularFunc.y,0.0f,circularFunc.x,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	//z軸
	circularFunc = substituteRad(rotateRad.z);
	Matrix4 movementMatZ =
	{
		circularFunc.x,circularFunc.y,0.0f,0.0f,
		-circularFunc.y,circularFunc.x,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	//3つの回転マトリックスを合成する
	Matrix4 concentratedRotateMat = movementMatX.Multiply(movementMatY.Multiply(movementMatZ));

	//SRT行列の作成
	//ScaleMatrix
	Matrix4 scaleMat =
	{
		scale_.x,0.0f,0.0f,0.0f,
		0.0f,scale_.y,0.0f,0.0f,
		0.0f,0.0f,scale_.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	//TranslateMat
	Matrix4 translateMat =
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		translate_.x,translate_.y,translate_.z,1.0f
	};

	//S⇔T
	ret_mat = scaleMat.Multiply(translateMat);
	//ST⇔R
	ret_mat = ret_mat.Multiply(concentratedRotateMat);

	return ret_mat;		//STR

}

Matrix3 Get_SRTMat2D(const float& delta_scaleX_, const float& delta_scaleY_, const float& delta_rotateTheta_,
	const Vector3& delta_pos_)
{
	Matrix3 ret_mat;

	//回転角をradianに変換
	static float const degreeConverter = Torima::kPi / 180.0f;
	float delta_rotateRad = delta_rotateTheta_ * degreeConverter;
	float cosT = cosf(delta_rotateRad);
	float sinT = sinf(delta_rotateRad);


	Matrix3 rotateMat =
	{
		cosT,sinT,0.0f,
		-sinT,cosT,0.0f,
		0.0f,0.0f,1.0f,
	};

	//SRT行列の作成
	//ScaleMatrix
	Matrix3 scaleMat =
	{
		delta_scaleX_,0.0f,0.0f,
		0.0f,delta_scaleY_,0.0f,
		0.0f,0.0f,1.0f
	};

	//TranslateMat
	Matrix3 translateMat =
	{
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		delta_pos_.x,delta_pos_.y,1.0f
	};

	//S⇔R
	ret_mat = scaleMat.GetMultiply(&rotateMat);
	//ST⇔R
	ret_mat = ret_mat.GetMultiply(&translateMat);

	return ret_mat;		//STR

}//SRT行列の作成・取得


Vector4<float> GetPerpendiculer(Vector4<float> point_)
{
	Vector4<float> ret_vec;

	if (point_.x != 0.0f || point_.y != 0.0f)
	{
		ret_vec = { -point_.y ,point_.x,0.0f,1.0f };
	}

	else
	{
		ret_vec = { 0.0f ,-point_.z,point_.y ,1.0f };
	}

	return ret_vec;
}

Vector3 ConvertToRectangularCoordinate(Vector3& sphereCoord_)
{
	Vector3 ret =
	{
		sphereCoord_.x * sinf(sphereCoord_.z) * sinf(sphereCoord_.y),
		sphereCoord_.x * cosf(sphereCoord_.z),
		sphereCoord_.x * sinf(sphereCoord_.z) * cosf(sphereCoord_.y)
	};

	return ret;
}


