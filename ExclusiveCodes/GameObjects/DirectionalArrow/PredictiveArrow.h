#pragma once
#include "../MovableBlock/MovableBlock.h"
#include "../../../MyLib/Utilities/PrimitiveStructures/QuadMesh/QuadBehavior/Cube/JointedCube/JointedCube.h"
#include "../../../MyLib/Utilities/ModellObjectStructures/ModelObject.h"

class PredictiveArrow
{
public:
    Vector3 position; // 始点
    MovableBlock::ArrowDirection direction;
    float arrowLength;
    bool visible;

    JointedCube jointCube;
    ModelObject sentan;//先端の三角形

    float easingTime;           // 経過時間
    float easingDuration = 1.0f;       // 伸ばす時間
    float easingStartLength = 0.0f;    // 開始時の長さ
    float easingTargetLength = 10.0f;

    PredictiveArrow();

    void Initialize();
    void SetDirectionAndPosition(MovableBlock::ArrowDirection dir, Vector3 startPos);
    void Update(Matrix4 vpMat);
    void Draw();
};