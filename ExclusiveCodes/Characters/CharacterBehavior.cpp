#include "CharacterBehavior.h"

//[Animatior]-----------------------------------------------------------------------------------------------------


AnimatorBehavior::AnimatorBehavior()
{

}


//[CharacterBehavior]-----------------------------------------------------------------------------------------------------


void CharacterBehavior::AddAdjustment(std::string name_)
{
	AddCubeToAdjustment(name_, &parts[name_]);

}

void CharacterBehavior::LoadVariables()
{
	GlobalVariables* g = GlobalVariables::GetInstance();

	for (auto itr = adjustmentItems.begin(); itr != adjustmentItems.end(); ++itr)
	{
		if (std::holds_alternative<Vector3*>((*itr).second))
		{
			*std::get<Vector3*>(adjustmentItems[(*itr).first]) = g->GetVector3Value(groupName, (*itr).first);
			initialPosture[(*itr).first] = g->GetVector3Value(groupName, (*itr).first);
		}
		else if (std::holds_alternative<Vector2*>((*itr).second))
		{
			*std::get<Vector2*>(adjustmentItems[(*itr).first]) = g->GetVector2Value(groupName, (*itr).first);
		}

	}
}

void CharacterBehavior::SaveVariables()
{
	GlobalVariables* g = GlobalVariables::GetInstance();

	for (auto itr = adjustmentItems.begin(); itr != adjustmentItems.end(); ++itr)
	{
		if (std::holds_alternative<Vector3*>((*itr).second))
		{
			Vector3* num = std::get<Vector3*>((*itr).second);
			g->SetValue(groupName, (*itr).first, *num);

		}
		else if (std::holds_alternative<Vector2*>((*itr).second))
		{
			Vector2* num = std::get<Vector2*>((*itr).second);
			g->SetValue(groupName, (*itr).first, *num);

		}

	}

	g->SaveFile(groupName);
}

void CharacterBehavior::AddCubeToAdjustment(std::string name_, CubeForModel* dst_)
{
	GlobalVariables* g = GlobalVariables::GetInstance();

	adjustmentItems[name_ + "_LOCAL"] = &dst_->bone.jointPart.trans.pos;

	adjustmentItems[name_ + "_TargetDir"] = &dst_->bone.jointPart.trans.targetDir;

	adjustmentItems[name_ + "_TOP_LT"] = &dst_->model.local_surfaces[C_TOP].leftTop.position;
	adjustmentItems[name_ + "_TOP_RT"] = &dst_->model.local_surfaces[C_TOP].rightTop.position;
	adjustmentItems[name_ + "_TOP_RB"] = &dst_->model.local_surfaces[C_TOP].rightBottom.position;
	adjustmentItems[name_ + "_TOP_LB"] = &dst_->model.local_surfaces[C_TOP].leftBottom.position;

	adjustmentItems[name_ + "_BOTTOM_LT"] = &dst_->model.local_surfaces[C_BOTTOM].leftTop.position;
	adjustmentItems[name_ + "_BOTTOM_RT"] = &dst_->model.local_surfaces[C_BOTTOM].rightTop.position;
	adjustmentItems[name_ + "_BOTTOM_RB"] = &dst_->model.local_surfaces[C_BOTTOM].rightBottom.position;
	adjustmentItems[name_ + "_BOTTOM_LB"] = &dst_->model.local_surfaces[C_BOTTOM].leftBottom.position;

	adjustmentItems[name_ + "_TEX_TOP_LT"] = &dst_->model.local_surfaces[C_TOP].leftTop.texcoord;
	adjustmentItems[name_ + "_TEX_TOP_RT"] = &dst_->model.local_surfaces[C_TOP].rightTop.texcoord;
	adjustmentItems[name_ + "_TEX_TOP_RB"] = &dst_->model.local_surfaces[C_TOP].rightBottom.texcoord;
	adjustmentItems[name_ + "_TEX_TOP_LB"] = &dst_->model.local_surfaces[C_TOP].leftBottom.texcoord;

	adjustmentItems[name_ + "_TEX_BOTTOM_LT"] = &dst_->model.local_surfaces[C_BOTTOM].leftTop.texcoord;
	adjustmentItems[name_ + "_TEX_BOTTOM_RT"] = &dst_->model.local_surfaces[C_BOTTOM].rightTop.texcoord;
	adjustmentItems[name_ + "_TEX_BOTTOM_RB"] = &dst_->model.local_surfaces[C_BOTTOM].rightBottom.texcoord;
	adjustmentItems[name_ + "_TEX_BOTTOM_LB"] = &dst_->model.local_surfaces[C_BOTTOM].leftBottom.texcoord;


	float const num = 0.4f;
	g->AddValue(groupName, name_ + "_LOCAL", { 0.0f,0.0f,0});
	g->AddValue(groupName, name_ + "_TargetDir", { 0.0f,0.0f,1.0f});

	g->AddValue(groupName, name_ + "_TOP_LT", { -num,num,num });
	g->AddValue(groupName, name_ + "_TOP_RT", { num,num,num });
	g->AddValue(groupName, name_ + "_TOP_RB", { num,num,-num });
	g->AddValue(groupName, name_ + "_TOP_LB", { -num,num,-num });
	g->AddValue(groupName, name_ + "_BOTTOM_LT", { -num,-num,-num });
	g->AddValue(groupName, name_ + "_BOTTOM_RT", { num,-num,-num });
	g->AddValue(groupName, name_ + "_BOTTOM_RB", { num,-num,num });
	g->AddValue(groupName, name_ + "_BOTTOM_LB", { -num,-num,num });

	g->AddValue(groupName, name_ + "_TEX_TOP_LT", { -0.5f,-0.5f });
	g->AddValue(groupName, name_ + "_TEX_TOP_RT", { 0.5f,-0.5f});
	g->AddValue(groupName, name_ + "_TEX_TOP_RB", { 0.5f,0.5f });
	g->AddValue(groupName, name_ + "_TEX_TOP_LB", { -0.5f,0.5f});
	g->AddValue(groupName, name_ + "_TEX_BOTTOM_LT", { -0.5f,0.5f });
	g->AddValue(groupName, name_ + "_TEX_BOTTOM_RT", { 0.5f,0.5f });
	g->AddValue(groupName, name_ + "_TEX_BOTTOM_RB", { 0.5f,-0.5f});
	g->AddValue(groupName, name_ + "_TEX_BOTTOM_LB", { -0.5f,-0.5f });

}


//[AnimFunctionBehavior]-----------------------------------------------------------------------------------------------------
bool AnimFunctionBehavior::ReturnDone()
{
	counter.Add();

	bool ret = false;
	if (!loop)
	{
		if (counter.IsEnd())
		{
			ret = true;
		}
	}

	else
	{
		counter.IsEnd();
		ret = true;
	}

	return ret;
}

void AnimFunctionBehavior::Reset()
{
	counter.count = 0.0f;
}
