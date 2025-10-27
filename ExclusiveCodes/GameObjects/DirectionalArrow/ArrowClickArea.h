#pragma once
#include "../MovableBlock/MovableBlock.h"
#include "../../../MyLib/Utilities/ModellObjectStructures/ModelObject.h"


class ArrowClickArea 
{
public:
    Vector3 position;
    MovableBlock::ArrowDirection direction;

    ModelObject chara;

    JointedCube auraJointCube;

    float auraEasingTime = 0.0f;
    const float auraEasingDuration = 1.5f;
    bool isAuraEnabled = false;

    ArrowClickArea(Vector3 pos, MovableBlock::ArrowDirection dir);

    void Update(const Matrix4& viewProj);
    void Draw(); 
};

