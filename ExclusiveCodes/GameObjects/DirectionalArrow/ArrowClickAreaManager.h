#pragma once
#include "ArrowClickArea.h"


struct PredictionalCube
{
    bool isActive = false;
    JointedCube cube;
    PredictionalCube();

};

class ArrowClickAreaManager
{
public:
    ArrowClickAreaManager();
    void Initialize();
    void Update(const Matrix4& viewProj);
    void Draw() const;

    std::vector<std::unique_ptr<ArrowClickArea>> arrowClickAreas;
    // 最後にマウスが当たっていた位置
    Vector3 lastHoveredArrowPos{};
    bool isArrowHovered = false;
    float collisionRadius;

    static int const kNumPredictionCubes = 100;
    static inline PredictionalCube predictionalCubes[kNumPredictionCubes];

};

