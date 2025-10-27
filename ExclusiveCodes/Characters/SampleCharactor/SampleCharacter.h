#pragma once
#include "../CharacterBehavior.h"

struct SampleCharacter:CharacterBehavior
{
	DisjointedCube cube;

	virtual void Update(Matrix4 vpMat_, int GameObjState_ = 0, float stateCount = 0.0f)override;
	virtual void Initialize(Transform4* objectTrans_) override;
	virtual void Draw(DrawMode mode_ = DrawMode::kSolid)override;

	SampleCharacter();

	virtual void Reset()override
	{
	};

};

