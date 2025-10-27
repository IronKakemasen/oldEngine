#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../VPShaderTable/VPShaderTable.h"

enum DrawMode
{
	kWireFrame,
	kSolid
};

// ブレンドモード
enum BlendMode
{
	kBlendModeNormal, //!< 通常αブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
	kBlendModeAdd,    //!< 加算。Src * SrcA + Dest * 1

	// 利用してはいけない
	kCountOfBlendMode
};

enum class CullMode
{
	kFront,
	kBack,
	kNone,
	kCountOfCullMode,
};

struct PipelineComponents
{	//[ BlendMode ]
	BlendMode cur_blendMode;
	//[ CullMode ]
	CullMode cur_cullMode;
	//[ ShaderType ]
	ShaderSetType cur_shaderType;

	PipelineComponents();
};

struct PipeLineSet
{
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineStateObject = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
};
