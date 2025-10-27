#include "CubeForModel.h"
#include "../MouseStructures/TransformDebugMouse.h"


Vector3 CubeForModel::GetVertexLocal(Vector3 local_)
{
	Matrix4 a = model.trans.GetWMat().GetInversed();
	return local_.GetMultiply(a);
}


void CubeForModel::Initialize(std::string name_, Vector3 size_, Vector3 local_, bool register_ )
{
	bone.Initialize(local_, size_);

	int tex[] = { 0,0,0,0,0,0 };
	model.Initialize(size_.x, size_.y, size_.z, { 0,-size_.y * 0.5f ,0 }, tex);
	//ボーンとキューブをペアレント化
	model.trans.parent_trans = &bone.jointPart.trans;

	if (register_)
	{
		//６面体の[ 上面 ]の４頂点を登録
		TransformDebugMouse::Register("[ " + name_ + " ]: " + "TOP_LT", &model.trans, 0.25f, &model.local_surfaces[C_TOP].leftTop);
		TransformDebugMouse::Register("[ " + name_ + " ]: " + "TOP_RT", &model.trans, 0.25f, &model.local_surfaces[C_TOP].rightTop);
		TransformDebugMouse::Register("[ " + name_ + " ]: " + "TOP_RB", &model.trans, 0.25f, &model.local_surfaces[C_TOP].rightBottom);
		TransformDebugMouse::Register("[ " + name_ + " ]: " + "TOP_LB", &model.trans, 0.25f, &model.local_surfaces[C_TOP].leftBottom);

		//６面体の[ 底面 ]の４頂点を登録
		TransformDebugMouse::Register("[ " + name_ + " ]: " + "BOTTOM_LT", &model.trans, 0.25f, &model.local_surfaces[C_BOTTOM].leftTop);
		TransformDebugMouse::Register("[ " + name_ + " ]: " + "BOTTOM_RT", &model.trans, 0.25f, &model.local_surfaces[C_BOTTOM].rightTop);
		TransformDebugMouse::Register("[ " + name_ + " ]: " + "BOTTOM_RB", &model.trans, 0.25f, &model.local_surfaces[C_BOTTOM].rightBottom);
		TransformDebugMouse::Register("[ " + name_ + " ]: " + "BOTTOM_LB", &model.trans, 0.25f, &model.local_surfaces[C_BOTTOM].leftBottom);

		//ボーンを干渉可能に
		TransformDebugMouse::Register("[ " + name_ + " ]: " + "JOINT", &bone.jointPart.trans, 0.05f);
	}
}

CubeForModel::CubeForModel()
{

}

void CubeForModel::Update(Matrix4 vpMat_)
{
	bone.Update(vpMat_);
	model.Update(vpMat_);
}

void CubeForModel::Draw(DrawMode mode_)
{
	model.drawMode = mode_;
	model.Draw(true);
	//bone.Draw();
}

void CubeForModel::ConnectCubeToAnotherSurface(CubeSurface to_, CubeForModel* other_)
{
	switch (to_)
	{
	case CubeSurface::kBottom:

		model.local_surfaces[C_BOTTOM].leftTop.position =
			GetVertexLocal(other_->model.local_surfaces[C_TOP].leftBottom.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_BOTTOM].rightTop.position =
			GetVertexLocal(other_->model.local_surfaces[C_TOP].rightBottom.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_BOTTOM].rightBottom.position =
			GetVertexLocal(other_->model.local_surfaces[C_TOP].rightTop.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_BOTTOM].leftBottom.position =
			GetVertexLocal(other_->model.local_surfaces[C_TOP].leftTop.position.GetMultiply(other_->model.trans.GetWMat()));

		break;
	case CubeSurface::kTop:

		model.local_surfaces[C_TOP].leftTop.position =
			GetVertexLocal(other_->model.local_surfaces[C_BOTTOM].leftBottom.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_TOP].rightTop.position =
			GetVertexLocal(other_->model.local_surfaces[C_BOTTOM].rightBottom.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_TOP].rightBottom.position =
			GetVertexLocal(other_->model.local_surfaces[C_BOTTOM].rightTop.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_TOP].leftBottom.position =
			GetVertexLocal(other_->model.local_surfaces[C_BOTTOM].leftTop.position.GetMultiply(other_->model.trans.GetWMat()));

		break;

	case CubeSurface::kLeft:

		model.local_surfaces[C_TOP].leftTop.position =
			GetVertexLocal(other_->model.local_surfaces[C_TOP].rightTop.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_TOP].leftBottom.position =
			GetVertexLocal(other_->model.local_surfaces[C_TOP].rightBottom.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_BOTTOM].leftBottom.position =
			GetVertexLocal(other_->model.local_surfaces[C_BOTTOM].rightBottom.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_BOTTOM].leftTop.position =
			GetVertexLocal(other_->model.local_surfaces[C_BOTTOM].rightTop.position.GetMultiply(other_->model.trans.GetWMat()));

		break;

	case CubeSurface::kRight:

		model.local_surfaces[C_TOP].rightTop.position =
			GetVertexLocal(other_->model.local_surfaces[C_TOP].leftTop.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_TOP].rightBottom.position =
			GetVertexLocal(other_->model.local_surfaces[C_TOP].leftBottom.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_BOTTOM].rightBottom.position =
			GetVertexLocal(other_->model.local_surfaces[C_BOTTOM].leftBottom.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_BOTTOM].rightTop.position =
			GetVertexLocal(other_->model.local_surfaces[C_BOTTOM].leftTop.position.GetMultiply(other_->model.trans.GetWMat()));


		break;
	case CubeSurface::kFront:

		model.local_surfaces[C_TOP].leftBottom.position =
			GetVertexLocal(other_->model.local_surfaces[C_TOP].leftTop.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_TOP].rightBottom.position =
			GetVertexLocal(other_->model.local_surfaces[C_TOP].rightTop.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_BOTTOM].leftTop.position =
			GetVertexLocal(other_->model.local_surfaces[C_BOTTOM].leftBottom.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_BOTTOM].rightTop.position =
			GetVertexLocal(other_->model.local_surfaces[C_BOTTOM].rightBottom.position.GetMultiply(other_->model.trans.GetWMat()));

		break;
	case CubeSurface::kBeyond:

		model.local_surfaces[C_TOP].leftTop.position =
			GetVertexLocal(other_->model.local_surfaces[C_TOP].leftBottom.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_TOP].rightTop.position =
			GetVertexLocal(other_->model.local_surfaces[C_TOP].rightBottom.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_BOTTOM].leftBottom.position =
			GetVertexLocal(other_->model.local_surfaces[C_BOTTOM].leftTop.position.GetMultiply(other_->model.trans.GetWMat()));

		model.local_surfaces[C_BOTTOM].rightBottom.position =
			GetVertexLocal(other_->model.local_surfaces[C_BOTTOM].rightTop.position.GetMultiply(other_->model.trans.GetWMat()));

		break;
	}
}

void CubeForModel::ReleaseSurfaceFromRegistration(CubeSurface surface_)
{
	switch (surface_)
	{
	case CubeSurface::kBottom:

		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_BOTTOM].leftBottom);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_BOTTOM].rightBottom);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_BOTTOM].rightTop);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_BOTTOM].leftTop);


		break;
	case CubeSurface::kTop:

		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_TOP].leftTop);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_TOP].rightTop);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_TOP].rightBottom);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_TOP].leftBottom);


		break;
	case CubeSurface::kRight:

		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_TOP].rightTop);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_TOP].rightBottom);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_BOTTOM].rightBottom);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_BOTTOM].rightTop);

		break;
	case CubeSurface::kLeft:

		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_TOP].leftTop);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_TOP].leftBottom);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_BOTTOM].leftBottom);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_BOTTOM].leftTop);


		break;
	case CubeSurface::kFront:
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_TOP].leftBottom);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_TOP].rightBottom);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_BOTTOM].leftTop);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_BOTTOM].rightTop);



		break;
	case CubeSurface::kBeyond:
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_TOP].leftTop);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_TOP].rightTop);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_BOTTOM].leftBottom);
		TransformDebugMouse::ReleaseRegistration(&model.local_surfaces[C_BOTTOM].rightBottom);

		break;
	}

}

void CubeForModel::Oyako(CubeForModel* parent_)
{
	bone.jointPart.trans.parent_trans = &parent_->bone.jointPart.trans;
}
