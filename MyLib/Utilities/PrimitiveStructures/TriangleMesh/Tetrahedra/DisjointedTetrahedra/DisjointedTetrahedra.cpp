#include "DisjointedTetrahedra.h"

DisjointedTetrahedra::DisjointedTetrahedra()
{

}

void DisjointedTetrahedra::Update(Matrix4 vpMat_)
{
	trans.SetWorldMatBasedQuaternion();
	trans.SetUVMat();

	for (int i = 0; i < 4; ++i)
	{
		local_surfaces[i].Update(vpMat_);
	}
}

void DisjointedTetrahedra::Draw(int enableLighting_ )
{
	for (int k = 0; k < 4; ++k)
	{
		NoviceLike::DrawTriangle(local_surfaces[k].left, local_surfaces[k].top, local_surfaces[k].right,
			local_surfaces[k].color, local_surfaces[k].texHandle, local_surfaces[k].trans.mat,
			drawMode, local_surfaces[k].trans.uvMat, enableLighting_);
	}
}

void DisjointedTetrahedra::Debug()
{
	ImGui::Begin(("DJTetra" + std::to_string(count)).c_str());
	ImGui::DragFloat3("AxIs", reinterpret_cast<float*>(&trans.targetDir), 0.05f);

	ImGui::DragFloat("X_AxIs", reinterpret_cast<float*>(&trans.defaultQuatenion.x_axis.deltaTheta), 0.05f);
	ImGui::DragFloat("Y_AxIs", reinterpret_cast<float*>(&trans.defaultQuatenion.y_axis.deltaTheta), 1.0F);
	ImGui::DragFloat("Z_AxIs", reinterpret_cast<float*>(&trans.defaultQuatenion.z_axis.deltaTheta), 0.05f);

	ImGui::End();

}