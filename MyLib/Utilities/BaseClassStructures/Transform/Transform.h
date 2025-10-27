#pragma once
#include "../VectorAndMatrix/VectorAndMatrix.h"
#include"../Quaternion/Quaternion.h"


struct TransformationMatrix
{
	Matrix4 WVP;
	Matrix4 World;
};

struct UVTransform
{
	//マテリアル行列
	Matrix4 uvMat;
	Matrix4* parent_uvMat = nullptr;
	Vector2 uvPos = {0.0f,0.0f};
	Vector2 uvScale= { 1,1 };
	float uvRotate = 0;
	//UV行列の更新
	void SetUVMat();
};

struct Transform4:UVTransform
{
	//親のトランスフォーム
	Transform4* parent_trans = nullptr;
	//行列	
	TransformationMatrix mat;
	//ポジション
	Vector3 pos = { 0.0f,0.0f,0.0f };
	//スケール
	Vector3 scale = { 1.0f, 1.0f ,1.0f };
	//向いている方向
	Vector3 targetDir = { 0.0f,0.0f,1.0f };
	//3軸固定クォータニオン
	QuaternionDefaultSet defaultQuatenion;

	//WorldViewProjection行列の更新（描画しないのであれば呼ぶ必要ない）
	void SetWVP_Mat(Matrix4 vpMat_);
	//ワールドマトリックスを更新する
	void SetWorldMatBasedQuaternion();
	//ワールド座標を取得する
	Vector3 GetWorldPos();
	void BeChildren(Transform4* parent_);

	Matrix4 GetWMat();
	Matrix4 GetRTMat();
	void TranslatePosition(Vector3 const delta_pos_);


};






//使わない
struct Transform3
{
	Matrix3 mat;
	Vector3 position = { 0,0,0 };
	Vector3 scale = { 1,1,1 };
	Vector3 rotation = { 0,0,0 };
};


