#pragma once
#define COLOR_BRONZE { 255,160,92,255 }
#define COLOR_PURPLE { 92,66,184,255 }
#define COLOR_RED { 255,0,0,255 }

#include "../CharacterBehavior.h"
#include "../../../MyLib/Utilities/ModellObjectStructures/ModelObject.h"
#include"../../../MyLib/Utilities/PrimitiveStructures/QuadMesh/QuadBehavior/Quad/Quad.h"

struct DoorParts {

    ModelObject middle;
    ModelObject eye;
    ModelObject keyhole;
    ModelObject doorL;
    ModelObject doorR;
    Quad quad;


    Transform4 Lkara;
    Transform4 Rkara;
    Transform4 root;

    Vector3 keyHolePos;
    Vector3 eyePos;
    Vector3 middlePos;
    DoorParts();
};

struct GoalCharacter :CharacterBehavior
{
    //DisjointedCube cube;

    struct Close :AnimFunctionBehavior
    {
        DoorParts* doorParts = nullptr;

        virtual void operator()(float counter_) override;
        virtual void Initialize(std::map<std::string, CubeForModel>* parts_) override;
    };

    struct Unlock :AnimFunctionBehavior
    {
        DoorParts* doorParts = nullptr;

        virtual void operator()(float counter_) override;
        virtual void Initialize(std::map<std::string, CubeForModel>* parts_) override;
    };

    struct Open :AnimFunctionBehavior
    {
        DoorParts* doorParts = nullptr;

        virtual void operator()(float counter_) override;
        virtual void Initialize(std::map<std::string, CubeForModel>* parts_) override;
    };


    struct Animator :AnimatorBehavior
    {
        enum STATE
        {
            CLOSE = 0,//閉じる
            UNLOCK = 1,//ロック解除
            OPEN = 2,//開く
            STATES
        };

        virtual void Initialize(std::function<bool(float counter_)>* funcs_)override {};
        virtual std::string OutputStateInString(int state_)override { return "iudbviusdh"; };
        Animator();
    };

    Animator animator;
    Close close;
    Unlock unlock;
    Open open;

    DoorParts doorParts;

    virtual void Update(Matrix4 vpMat_, int GameObjState_ = 0, float stateCount = 0.0f)override;
    virtual void Initialize(Transform4* objectTrans_) override;
    virtual void Draw(DrawMode mode_ = DrawMode::kSolid)override ;

    virtual void Reset()override
    {
    };

    GoalCharacter();

};

