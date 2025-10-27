#pragma once

#include <functional>
#include <map>
#include <variant>

#include "../../MyLib/Utilities/CounterStructures/CounterAndTime.h"
#include "../../MyLib/Utilities/CubeForModelStructures/CubeForModel.h"
#include "../../MyLib/Utilities/BenriFunctionStructures/BenriFunctions.h"
#include "../../MyLib/Utilities/EasingFuctionStructures/EasingFunctions.h"
#include "../../MyLib/Utilities/GlobalVariablesStructures/GlobalVariables.h"
#include "../../MyLib/Utilities/PrimitiveStructures/QuadMesh/QuadBehavior/Cube/DisjointedCube/DisjointedCube.h"

struct AnimFunctionBehavior
{
	std::map<std::string, CubeForModel>* parts = nullptr;
	bool loop = false;
	CounterAndInvTime counter;

	virtual void operator()(float counter_) = 0;
	virtual void Initialize(std::map<std::string, CubeForModel>* parts_) = 0;
	bool ReturnDone();
	void Reset();

};



struct AnimatorBehavior
{	
	//現在のアニメーションステート
	int cur_state = 0;
	//アニメーション関数の配列
	std::vector<AnimFunctionBehavior*> animFuncs;

	virtual void Initialize(std::function<bool(float counter_)>* funcs_) = 0;
	virtual std::string OutputStateInString(int state_) = 0;

	AnimatorBehavior();
	//オブジェごとに大きく異なる

};


struct CharacterBehavior
{
	//モデルのグループネーム
	std::string groupName = "";
	//キューブモデルの集合体
	std::map<std::string, CubeForModel> parts;
	//記録用
	std::map < std::string, std::variant<Vector3*,Vector2*>> adjustmentItems;
	//初期姿勢
	std::map < std::string, Vector3> initialPosture;

	virtual void Update(Matrix4 vpMat_, int GameObjState_ = 0, float stateCount = 0.0f) = 0;
	virtual void Initialize(Transform4* objectTrans_) = 0;
	virtual void Draw(DrawMode mode_ = DrawMode::kSolid) = 0;
	virtual void Reset() = 0;

	void AddAdjustment(std::string name_);
	void SaveVariables();
	void LoadVariables();
	void AddCubeToAdjustment(std::string name_, CubeForModel* dst_);
};

