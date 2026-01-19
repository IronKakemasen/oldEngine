#include "MyTexture.h"
#include "../../Utilities/BaseClassStructures/VectorAndMatrix/VectorAndMatrix.h"

//テクスチャを読み込むための関数
DirectX::ScratchImage LoadTexture(std::string const& filePath_)
{
	//テクスチャファイルを読み込んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath_);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(),
		DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		DirectX::TEX_FILTER_SRGB, 0,
		mipImages);

	assert(SUCCEEDED(hr));

	//ミップマップ付きのデータを返す
	return mipImages;
}

//読み込んだTextureの情報をもとにTextureResourceを作る関数
Microsoft::WRL::ComPtr<ID3D12Resource>  CreateTextureResource(ID3D12Device* device_, DirectX::TexMetadata& metaData_)
{
	D3D12_RESOURCE_DESC resourceDesc{};

	//[ materialDataを元にResourceの設定 ]
	//textureの幅
	resourceDesc.Width = UINT(metaData_.width);
	//textureの高さ
	resourceDesc.Height = UINT(metaData_.height);
	//mipmapの数
	resourceDesc.MipLevels = UINT16(metaData_.mipLevels);
	//奥行orTextureの配列数
	resourceDesc.DepthOrArraySize = UINT16(metaData_.arraySize);
	//Textureのformat
	resourceDesc.Format = metaData_.format;
	//DXGI_FORMAT_B8G8R8A8_UNORM_SRGB
	//サンプリングカウント。１固定
	resourceDesc.SampleDesc.Count = 1;
	//テクスチャの次元数。2
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metaData_.dimension);

	//[ 利用するHeapの設定 ]
	//細かい設定を行う
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	//[ resourceを生成する ]
	Microsoft::WRL::ComPtr<ID3D12Resource>  resource = nullptr;
	HRESULT hr = device_->CreateCommittedResource(
		&heapProperties,					//Heapの設定
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定、特になし
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,		//データ転送される設定
		nullptr,							//Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource));			//作成するresourceへのポインタのポインタ

	assert(SUCCEEDED(hr));

	return resource;
}

//読み込んだTextureの情報をもとにTextureResourceを作る関数
Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(ID3D12Device* device_)
{
	D3D12_RESOURCE_DESC resourceDesc{};

	//[ materialDataを元にResourceの設定 ]
	//textureの幅
	resourceDesc.Width = UINT(WINDOW_W);
	//textureの高さ
	resourceDesc.Height = UINT(WINDOW_H);
	//mipmapの数
	resourceDesc.MipLevels = 1;
	//奥行orTextureの配列数
	resourceDesc.DepthOrArraySize = 1;
	//Textureのformat
	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//サンプリングカウント。１固定
	resourceDesc.SampleDesc.Count = 1;
	//テクスチャの次元数。2
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(3);
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//[ 利用するHeapの設定 ]
	//細かい設定を行う
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	
	//D3D12_CLEAR_VALUE c_value;
	//c_value.Color;
	//c_value.Color[0] = 1.0f;
	//c_value.Color[1] = 1.0f;
	//c_value.Color[2] = 1.0f;
	//c_value.Color[3] = 1.0f;
	//c_value.DepthStencil.Depth = 1.0f;
	//c_value.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//[ resourceを生成する ]
	Microsoft::WRL::ComPtr<ID3D12Resource>  resource = nullptr;
	HRESULT hr = device_->CreateCommittedResource(
		&heapProperties,					//Heapの設定
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定、特になし
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,							//Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource));			//作成するresourceへのポインタのポインタ
	//D3D12_RESOURCE_STATE_RENDER_TARGET
	//D3D12_RESOURCE_STATE_GENERIC_READ
	//D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,

	assert(SUCCEEDED(hr));

	return resource;
}


TextureResourceData LoadTextureData(std::string filePath_, ID3D12Device* device_)
{
	TextureResourceData ret_textureResourceData;
	ret_textureResourceData.mipImages = LoadTexture(filePath_);
	ret_textureResourceData.metaData = ret_textureResourceData.mipImages.GetMetadata();
	ret_textureResourceData.textureResource = CreateTextureResource(device_, ret_textureResourceData.metaData);
	ret_textureResourceData.textureResource->SetName(L"TEXTURE");

	return ret_textureResourceData;

}

OSResourceData GetOS_TextureData(ID3D12Device* device_)
{
	OSResourceData ret_textureResourceData;
	ret_textureResourceData.textureResource = CreateTextureResource(device_);

	return ret_textureResourceData;

}

