#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../../Utilities/BaseClassStructures/BaseClass.h"

struct MappedResource
{
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixBuff;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialBuff;
	//ワールド行列バッファマップ
	TransformationMatrix* transformationMatrixMap = nullptr;
	//マテリアルバッファマップ
	Material* materialMap = nullptr;
};
