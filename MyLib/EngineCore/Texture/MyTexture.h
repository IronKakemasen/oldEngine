#pragma once

#include "../LogAndException/AboutDebugLog.h"
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include "../../../External/DirectXTex/DirectXTex.h"
#include <map>

struct SRV_Handle
{
	inline static int numHandle = 0;
};

struct TextureHandle
{
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

};

struct TextureResourceData
{
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
	DirectX::ScratchImage mipImages;
	DirectX::TexMetadata metaData;
	TextureHandle textureHandle;
	inline static uint32_t NumTextures = 0;

};

struct OSResourceData
{
	D3D12_CPU_DESCRIPTOR_HANDLE osRenderTargetHandle;
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource;
	TextureHandle textureHandle;
	inline static uint32_t NumOSResource = 0;
};


//テクスチャを読み込むための関数
DirectX::ScratchImage LoadTexture(std::string const& filePath_);
//読み込んだTextureの情報をもとにTextureResourceを作る関数
Microsoft::WRL::ComPtr<ID3D12Resource>  CreateTextureResource(ID3D12Device* device_, DirectX::TexMetadata& metaData_);
Microsoft::WRL::ComPtr<ID3D12Resource>  CreateTextureResource(ID3D12Device* device_);

TextureResourceData LoadTextureData(std::string filePath_, ID3D12Device* device_);
OSResourceData GetOS_TextureData(ID3D12Device* device_);





