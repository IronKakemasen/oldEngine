#pragma once
#include "MappedResource.h"
#include <vector>


struct Mesh
{
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuff;
	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	// 頂点バッファマップ
	VertexData* vertexMap = nullptr;
	// インデックスバッファマップ
	uint32_t* indexMap = nullptr;

	// 頂点バッファマップ(Line用)
	Vector3* vertexMapForLine = nullptr;

	std::vector< MappedResource >mappedResource;

};



