#pragma once
#include "../../NoviceLike/NoviceLike.h"


struct DebugUI
{
	std::vector < std::pair < std::string, std::function<void() >> > debugFunctions;
	virtual void AddToDebugUI(std::string name_,std::function<void()> func_) = 0;
	virtual void DebugBasicalParameter() = 0;


	DebugUI() = default;
	~DebugUI() = default;
	DebugUI(const DebugUI&) = delete;
	DebugUI& operator=(const DebugUI&) = delete;

};

struct DebugSceneUI:DebugUI
{
	static DebugSceneUI* GetInstance();
	virtual void AddToDebugUI(std::string name_, std::function<void()> func_)override;
	virtual void DebugBasicalParameter()override;
};

void DrawGrid(Matrix4 vpMat_);
