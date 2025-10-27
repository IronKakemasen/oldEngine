#pragma once
#include "../CharacterBehavior.h"
#include "../../../MyLib/Utilities/ModellObjectStructures/ModelObject.h"


struct CharacterParts
{
	Transform4 kara;

	ModelObject around;
	ModelObject center;
	ModelObject eye;
	ModelObject large;
	ModelObject middle;
	ModelObject g_small;

	Vector3 centerPos;
	Vector3 eyePos;
	Vector3 largePos;
	Vector3 g_smallPos;

	CharacterParts();

};


struct RobotChara :public CharacterBehavior
{
	struct Idle :AnimFunctionBehavior
	{
		CharacterParts* charaParts = nullptr;

		virtual void operator()(float counter_) override;
		virtual void Initialize(std::map<std::string, CubeForModel>* parts_) override;
	};

	struct Moving :AnimFunctionBehavior
	{
		CharacterParts* charaParts = nullptr;

		virtual void operator()(float counter_) override;
		virtual void Initialize(std::map<std::string, CubeForModel>* parts_) override;

	};

	struct Jumping :AnimFunctionBehavior
	{
		CharacterParts* charaParts = nullptr;

		virtual void operator()(float counter_) override;
		virtual void Initialize(std::map<std::string, CubeForModel>* parts_) override;
	};

	struct Falling :AnimFunctionBehavior
	{
		CharacterParts* charaParts = nullptr;

		virtual void operator()(float counter_) override;
		virtual void Initialize(std::map<std::string, CubeForModel>* parts_) override;
	};


	struct Dead :AnimFunctionBehavior
	{
		CharacterParts* charaParts = nullptr;

		virtual void operator()(float counter_) override;
		virtual void Initialize(std::map<std::string, CubeForModel>* parts_) override;
	};

	struct Enter :AnimFunctionBehavior
	{
		CharacterParts* charaParts = nullptr;

		virtual void operator()(float counter_) override;
		virtual void Initialize(std::map<std::string, CubeForModel>* parts_) override;
	};

	struct Goal :AnimFunctionBehavior
	{
		CharacterParts* charaParts = nullptr;

		virtual void operator()(float counter_) override;
		virtual void Initialize(std::map<std::string, CubeForModel>* parts_) override;
	};


	struct Animator :AnimatorBehavior
	{
		enum STATE
		{
			IDLE = 0,//アイドリング loop
			MOVE = 1,//移動 loop
			JUMP = 2,//ジャンプ 一度
			FALLING = 3,//落ちる　loop
			DEAD = 4,//死亡時
			ENTER = 5,//入場
			GOAL = 6,//ゴール時

			STATES
		};


		virtual void Initialize(std::function<bool(float counter_)>* funcs_)override;
		virtual std::string OutputStateInString(int state_)override;
		Animator();
	};

	Animator animator;

	Idle idle;
	Moving moving;
	Dead dead;
	Jumping jumping;
	Enter enter;
	Falling falling;
	Goal goal;

	CharacterParts charaParts;

	virtual void Update(Matrix4 vpMat_, int GameObjState_ = 0, float stateCount = 0.0f) override;
	virtual void Draw(DrawMode mode_ = DrawMode::kSolid) override;
	virtual void Reset() override;
	virtual void Initialize(Transform4* objectTrans_) override;


	RobotChara();
};
