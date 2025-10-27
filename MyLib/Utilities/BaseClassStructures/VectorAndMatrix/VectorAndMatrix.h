#pragma once
#include <math.h>
#include <cmath>

#pragma warning(disable : 4201)

namespace Torima
{
	float constexpr kPi = 3.141592653589f;
	float constexpr windowWidth = 1280.0f;
	float constexpr windowHeight = 720.0f;
	float constexpr torimaFOVy = 90.0f * kPi / 180.0f;
	float constexpr torimaFOVx = 121.0f * kPi / 180.0f;
}

#define VECTOR_UP Vector3({0.0f,1.0f,0.0f})
#define VECTOR_DOWN Vector3({0.0f,-1.0f,0.0f})
#define VECTOR_RIGHT Vector3({1.0f,0.0f,0.0f})
#define VECTOR_LEFT Vector3({-1.0f,0.0f,0.0f})
#define VECTOR_BEYOND Vector3({0.0f,0.0f,1.0f})
#define VECTOR_NONE Vector3({0.0f,0.0f,0.0f})
#define WINDOW_W 1280.0f
#define WINDOW_H 720.0f


enum ProjectionType
{
	kPerspective,
	kParallel
};

enum AffineMatrixType
{
	kSRT,
	kSTR
};


//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//Matrix3x3　　　　 　　　　　　　　　　　　　　　　　　　　□
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
struct Matrix3
{
	float m[3][3] =
	{
		{1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{0.0f,0.0f,1.0f},
	};

	Matrix3 operator+=(Matrix3 other);
	Matrix3 operator-=(Matrix3 other);

	Matrix3() {};

	Matrix3(float zero_);
	Matrix3(float a0, float a1, float a2, float a3, float a4, float a5,
		float a6, float a7, float a8);

	Matrix3 GetMultiply(Matrix3* const other);
};


//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//Matrix4x4　　　　 　　　　　　　　　　　　　　　　　　　　□
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
class Matrix4
{

public:

	float m[4][4] =
	{
		{1.0f,0.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f,0.0f},
		{0.0f,0.0f,1.0f,0.0f},
		{0.0f,0.0f,0.0f,1.0f},
	};

	Matrix4 operator+=(Matrix4 other);
	Matrix4 operator-=(Matrix4 other);

	Matrix4() {};

	Matrix4(float zero_);
	Matrix4(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9,
		float a10, float a11, float a12, float a13, float a14, float a15);

	//マトリックス同士の積
	Matrix4 Multiply(Matrix4 const& other);
	//****************************************
	//https://suzulang.com/inverse-matrix-44/
	//****************************************
	//4x4行列の行列式を返す関数
	float GetDeterminant4x4();
	//4x4行列の逆行列を取得
	Matrix4 GetInversed();

};



//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//Vector2  　　　　 　　　　　　　　　　　　　　　　　　　　□
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
struct Vector2
{
	float u = 0.0f; float v = 0.0f;

	Vector2() :u(0), v(0) {}
	Vector2(float entries[2]) : u(entries[0]), v(entries[1]) {}
	Vector2(float u_, float v_) : u(u_), v(v_) {}
	float GetMagnitutde();
	Vector2 GetNormalized();
	float GetCos(Vector2 other_);
	float GetDot(Vector2 other_);

};

//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//Vector3　　　　 　　　　　　　　　　　　　　　　　　　　  □
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
struct Vector3
{
	float x = 0;
	float y = 0;
	float z = 0;

	Vector3() {}
	Vector3(float entries[3]) : x(entries[0]), y(entries[1]), z(entries[2]) {}
	Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

	Vector3 GetMultiply(Matrix4 src_mat);
	// 大きさをゲット
	float GetMagnitutde();
	// 正規化ベクトルを返す
	Vector3 GetNormalized();
	//cosΘを計算して返す
	float GetCos(Vector3 other_);
	//内積を計算して返す
	float GetDot(Vector3 other_);
	Vector3 GetCross(Vector3 other);
	Vector3 GetPerpendiculer();
	// ベクトルの長さを計算する
	float Length() const;

	// 正規化されたベクトルを返す
	Vector3 Normalize() const;

};


//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//Vector4  　　　　 　　　　　　　　　　　　　　　　　　　　□
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
template<typename T>
class Vector4
{

public:

	T x = 0;
	T y = 0;
	T z = 0;
	T w = 1;

	//============Constractor==============
	Vector4() {};
	Vector4(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) {}
	Vector4(T entries[4]) : x(entries[0]), y(entries[1]), z(entries[2]), w(entries[3]) {}
	//=====================================

	Vector4<float> operator+=(float const addNum);
	Vector4<float> operator=(float const num);

	//大きさをセット
	float GetMagnitutde();
	float GetCos(Vector4<float> other_);
	//クロス積を返す
	Vector4<float> GetCross(Vector4<float> other);
	//正規化ベクトルを返す
	Vector4<float> GetNormalized();
	//cosを計算して返す
	float Dot(Vector4<float> other_);

	Vector4<float> GetNormalizedForQuaternion();
	float GetCosForQuaternion(Vector4<float> other_);
	//差分単位ベクトルを返す
	Vector4<float> GetNormalizedDiffVector4(Vector4<float> dst_pos, Vector4<float> my_pos);
	//マトリックスとベクタの積
	Vector4<float> GetMultiply(Matrix4 src_mat);
	//法線ベクトル
	Vector4<float> GetPerpendiculer(Vector4<float> point_);


};

//==================================Operand============================================================
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓Operand↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
template<typename T>
Vector4<T> operator+(Vector4<T> const& lVec, Vector4<T> const& rVec2)
{
	Vector4<T> ret_vec = { lVec.x + rVec2.x,lVec.y + rVec2.y ,lVec.z + rVec2.z ,1 };

	return ret_vec;
}

template<typename T>
Vector4<T> operator-(Vector4<T> const& lVec, Vector4<T> const& rVec2)
{
	Vector4<T> ret_vec = { lVec.x - rVec2.x,lVec.y - rVec2.y ,lVec.z - rVec2.z ,1 };

	return ret_vec;
}

template<typename T>
Vector4<T> operator*(Vector4<T> const& dst_vec, T const multipleNum)
{
	Vector4<T> ret_vec = { dst_vec.x * multipleNum,dst_vec.y * multipleNum ,dst_vec.z * multipleNum ,1 };

	return ret_vec;
}

Matrix4 operator*(Matrix4 const& dst_mat, float multipleNum);
Vector3 operator*(Vector3 const& dst_vec, float const multipleNum);
Vector3 operator+(Vector3 const& lVec, Vector3 const& rVec2);
Vector3 operator-(Vector3  const& lVec, Vector3  const& rVec2);
bool operator==(Vector3  const& lVec, Vector3  const& rVec2);
Vector2 operator-(Vector2  const& lVec_, Vector2  const& rVec_);
Vector2 operator*(Vector2 const& dst_vec_, float const multipleNum);
Vector2 operator+(Vector2 const& lVec_, Vector2 const& rVec_);



//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑Operator↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
//==================================Operator============================================================


//SRT行列の作成・取得
Matrix4 Get_SRTMat3D(const Vector3& scale_, const Vector3& rotateTheta_,
	const Vector3& translate_);
//STR行列の作成・取得
Matrix4 Get_STRMat3D(const Vector3& scale_, const Vector3& movementTheta_,
	const Vector3& translate_);
//正射影行列3D
Matrix4 Get_Orthographic3D(
	const float l_, const float r_,
	const float t_, const float b_,
	const float zn_ = 0.1f, const float zf_ = 100.0f);
//ビューポート変換3D
Matrix4 Get_ViewportTransformation3D(
	const float left_ = 0.0f, const float top_ = 0.0f,
	const float windowWidth_ = WINDOW_W, const float windowHeight_ = WINDOW_H,
	const float minDepth_ = 0.0f, const float maxDepth_ = 1.0f);
//透視投影行列
Matrix4 Get_PerspectiveFOV(float fovY_, float aspectRatio_, float nearClip_ = 0.1f, float farClip_ = 1000.0f);
//VP行列
Matrix4 Get_VPMat(Matrix4 cameraMat, Vector3 cameraPos = {0,0,0},ProjectionType type_ = kPerspective );
//STR行列の作成・取得
Matrix3 Get_SRTMat2D(const float& delta_scaleX_, const float& delta_scaleY_, const float& delta_rotateTheta_,
	const Vector3& delta_pos_);
Matrix4 GetQuaternionRotateMat(Vector4<float> quaternion_);
Vector4<float> GetQuaternion(Vector3 v_, float deltaTheta_);
Matrix4 Get_SQrTMat3D(const Vector3& scale_, Vector4<float> quaternion_,const Vector3& translate_);
Vector4<float> CompositeQuaternion(Vector4<float> p_, Vector4<float> q_);
Matrix4 GetScaleMat(Vector3 scale_);
Matrix4 GetTranslateMat(Vector3 translate_);
Vector4<float> GetPerpendiculer(Vector4<float> point_);
Vector3 ConvertToRectangularCoordinate(Vector3& sphereCoord_);
