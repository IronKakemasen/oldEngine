#pragma once
#include "../../../MyLib/Utilities/PrimitiveStructures/QuadMesh/QuadBehavior/Cube/JointedCube/JointedCube.h"
#include "../MovableBlock/MovableBlock.h"

class PredictiveCardUI 
{
public:
    Vector3 position;
    int moveValue;
    bool isVisible;

    Vector3 basePos;              
    Vector3 moveDirection;        
    float easingStartLength;
    float easingTargetLength;
    float easingDuration; 
    float easingTime;

    MovableBlock::ArrowDirection direction;

    JointedCube jointCube;

    PredictiveCardUI();

    void Initialize();
   
    void Set(int value, const Vector3& pos, MovableBlock::ArrowDirection dir);

    void Update(Matrix4 vpMat_);

    void Draw();

};
