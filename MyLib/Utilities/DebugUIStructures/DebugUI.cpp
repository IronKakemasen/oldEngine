#include "DebugUI.h"
#include "../../NoviceLike/NoviceLike.h"

DebugSceneUI* DebugSceneUI::GetInstance()
{
	static DebugSceneUI debugUI;
	return &debugUI;
}

void DebugSceneUI::AddToDebugUI(std::string name_,std::function<void()> func_)
{
	std::pair<std::string, std::function<void()>> t(name_, func_);

	debugFunctions.emplace_back(t);
}

void DebugSceneUI::DebugBasicalParameter()
{
	//ImGui::Begin("How To");
	//ImGui::Text("[ W , A , S , D ] : MoveAround");
	//ImGui::Text("[ O , K ] : MoveForwardBack");
	//ImGui::End();

	ImGui::Begin("DebugBasicalParameter");
	ImGui::Text("deltaTime%.5f", pDxCommon->deltaTime);
	ImGui::Text("FPS%.2f", ImGui::GetIO().Framerate);

	for (auto itr = debugFunctions.begin(); itr != debugFunctions.end(); ++itr)
	{
		if (ImGui::TreeNode((*itr).first.c_str()))
		{
			(*itr).second();
			ImGui::TreePop();
		}
	}

	ImGui::End();
}




void DrawGrid(Matrix4 vpMat_)
{
	static float const len = 1.0f;
	static float const kNumLines = 120;
	static float const half = kNumLines * 0.5f;

	Vector3 xLineStart = { -half,0,-half};
	Vector3 xLineEnd = { half,0,-half};
	Vector3 zLineStart = { -half,0,half};
	Vector3 zLineEnd = { -half,0,-half};

	TransformationMatrix constantBuffer;
	constantBuffer.WVP = constantBuffer.WVP.Multiply(vpMat_);
	Matrix4 constUVMat;

	Vector4 < float > xColor = { 0,75,0,200 };
	Vector4 < float > zColor = { 75,0,0,200 };
	Vector4<float> zero = { 0,0,0.0f ,255 };
	Vector4<float> cur_xColor = zero;
	Vector4<float> cur_zColor = zero;

	for (int i = 0; i < kNumLines; ++i)
	{

		if (i == (int)half)
		{
			cur_xColor = xColor;
			cur_zColor = zColor;
		}

		else if ((i % 10) == 0)
		{
			int digit = i / 10;
			float const standardVal = 10.0f;
			Vector4 < float > color = { standardVal * digit ,standardVal * digit ,standardVal * digit ,255.0f };
			cur_xColor = color;
			cur_zColor = color;
		}

		else
		{
			cur_xColor = zero;
			cur_xColor.x = 25.0f * (xLineStart.z / kNumLines);
			cur_zColor = zero;
			cur_zColor.y = 25.0f* (zLineStart.x / kNumLines);

		}

		NoviceLike::DrawLine(xLineStart, xLineEnd, 0, cur_xColor, constantBuffer, constUVMat);
		NoviceLike::DrawLine(zLineStart, zLineEnd, 0, cur_zColor, constantBuffer, constUVMat);

		xLineStart.z ++;
		xLineEnd.z ++;

		zLineStart.x ++;
		zLineEnd.x ++;

	}
}
