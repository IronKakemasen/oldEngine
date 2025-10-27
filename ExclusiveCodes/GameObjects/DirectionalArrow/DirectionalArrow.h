#pragma once
#include "../MovableBlock/MovableBlock.h"
#include "../MovableField/MovableField.h"
#include "../../../MyLib/Utilities/PrimitiveStructures/TriangleMesh/Tetrahedra/DisjointedTetrahedra/DisjointedTetrahedra.h"
#include "../../../MyLib/Utilities/ModellObjectStructures/ModelObject.h"

class DirectionalArrow
{
public:
    void Initialize(MovableField* movableField);
    void Update(Matrix4 vpMat_);
    void Draw();

    void SetDirection(MovableBlock::ArrowDirection dir);
    void SetPosition(const Vector3& pos);
    // ブロックデータや矢印の始点位置を渡す関数
    void SetMoveData(const std::vector<std::unique_ptr<MovableBlock>>& blocks, Vector3 arrowPos);

    DirectionalArrow();
    MovableField* movableField = nullptr;

    float value; // 移動量
    bool enabled; // 線が有効なときだけ判定
    MovableBlock::ArrowDirection direction;
    JointedCube jointCube;
    ModelObject sentan;//先端の三角形
	float arrowLength; // 矢印の長さ
    std::vector<MovableBlock*> targetBlocks;
    Vector3 startPosition;

    bool hasTriggeredArrowInput = false;

private:
    Vector3 position;
    float stretchAmount;  // 伸ばす量
    float easingTime;           // 経過時間
    float easingDuration;       // 伸ばす時間
    float easingStartLength;    // 開始時の長さ
    float easingTargetLength;   // 最大長さ
    bool isEasing = false;
};

