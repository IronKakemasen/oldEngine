#pragma once
#include "MouseBehavior.h"
#include "../PrimitiveStructures/TriangleMesh/Tetrahedra/DisjointedTetrahedra/DisjointedTetrahedra.h"

struct TransData
{
	Transform4* trans = nullptr;
	VertexData* anotherVertex = nullptr;
	std::unique_ptr<DisjointedTetrahedra> tetra;
	bool getFocusd = false;
	DrawMode drawMode = DrawMode::kSolid;
	Vector3 GetRegisterdVectorWorld();

};


class TransformDebugMouse :public MouseBehavior
{

private:
	
	bool haveInitiative = true;
	bool showResponse = false;
	bool dragStart = false;
	bool vertexEdit = false;
	std::pair<float*, float> draggingTarget;

public:
	static inline std::map<std::string, TransData > touchableTransDatas;
	static inline bool isUpdating = false;

	virtual void Update(Matrix4 vpMat_, Vector3 cameraPos_)override;
	virtual void Debug()override;
	virtual void Draw(Matrix4 vpMat_)override;

	static void ReleaseRegistration(VertexData* anotherVertex_);
	static void Register(std::string name_, Transform4* trans_, float size_, VertexData* another_ = nullptr);
	TransformDebugMouse();
	void IsCollision(Matrix4 vpMat_, Vector3  cameraPos_);
	void SetMouseColor();

};



//struct MouseController
//{
//	static inline std::map<std::string, TransData > touchableTransDatas;
//	static inline bool isUpdating = false;
//
//
//	Vector2 preMousePos;
//	bool haveInitiative = true;
//	bool showResponse = false;
//	Vector4<float> mouseRay;
//	float mouse_radius = 1.0f;
//	Sprite mouseSprite;
//	bool dragStart = false;
//	std::pair<float*, float> draggingTarget;
//	bool mouseStatus = MouseStatus::kEdit;
//
//
//	static void ReleaseRegistration(Vector4<float>* anotherLocal_pos_);
//	static void Register(std::string name_, Transform4* trans_, float size_, Vector4<float>* another_ = nullptr);
//	MouseController();
//	void Update(Matrix4 vpMat_, Vector4<float> cameraPos_);
//	void Debug();
//	void RayCast(Matrix4 vpMat_);
//	void Draw(Matrix4 vpMat_);
//	void IsCollision(Matrix4 vpMat_, Vector4<float> cameraPos_);
//	void SetMouseColor();
//};
