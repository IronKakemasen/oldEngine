#include "ArrowClickAreaManager.h"
#include "../../Map/InGameMapBehavior.h"


PredictionalCube::PredictionalCube()
{
    int t[6];
    Fill(t, TagTexture::kWhite2x2);

    cube.Initialize(Chip::kBlockWidth, Chip::kBlockWidth, Chip::kBlockWidth, {}, t, { 90,90,0,120});

}


ArrowClickAreaManager::ArrowClickAreaManager()
{
    arrowClickAreas.clear();

    const float blockW = ChipBehavior::kBlockWidth;
    const float blockH = ChipBehavior::kBlockHeight;
    const float z = -blockH / 2.0f;

    // 上下
    for (int i = 1; i <= 10; ++i)
    {
        float x = static_cast<float>(i) * blockW;

        arrowClickAreas.push_back(std::make_unique<ArrowClickArea>(
            Vector3{ x, (InGameMapBehavior::kNumBlockVertical - 11.5f) * blockH, z },
            MovableBlock::ArrowDirection::Up
        ));

        arrowClickAreas.push_back(std::make_unique<ArrowClickArea>(
            Vector3{ x, (InGameMapBehavior::kNumBlockVertical - 0.5f) * blockH, z },
            MovableBlock::ArrowDirection::Down
        ));

    }

    // 左右
    for (int i = 1; i <= 10; ++i)
    {
        float y = (i + 20.0f) * blockH;

        arrowClickAreas.push_back(std::make_unique<ArrowClickArea>(
            Vector3{ blockW / 2.0f, y, z },
            MovableBlock::ArrowDirection::Right
        ));

        arrowClickAreas.push_back(std::make_unique<ArrowClickArea>(
            Vector3{ (InGameMapBehavior::kNumBlockHorizontal - 0.5f) * blockW, y, z },
            MovableBlock::ArrowDirection::Left
        ));
    }
    collisionRadius = 12.0f;
    Initialize();
}

void ArrowClickAreaManager::Initialize()
{
 

}

void ArrowClickAreaManager::Update(const Matrix4& viewProj)
{


    for (auto& area : arrowClickAreas)
    {
        area->Update(viewProj);
    }


    //予測ブロックのアップデート
    for (int i = 0; i < kNumPredictionCubes; ++i)
    {
        if (predictionalCubes[i].isActive)
        {
            predictionalCubes[i].cube.Update(viewProj);
        }
    }

}

void ArrowClickAreaManager::Draw() const
{
    for (const auto& area : arrowClickAreas)
    {
        area->Draw();
    }

    NoviceLike::SetBlendMode(BlendMode::kBlendModeAdd);
    //予測ブロックのドロー
    for (int i = 0; i < kNumPredictionCubes; ++i)
    {
        if (predictionalCubes[i].isActive)
        {
            predictionalCubes[i].cube.Draw();
        }
    }
    NoviceLike::SetBlendMode(BlendMode::kBlendModeNormal);

}
