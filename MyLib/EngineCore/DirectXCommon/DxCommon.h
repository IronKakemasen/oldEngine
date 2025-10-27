#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <dxgi1_6.h>
#include <dxcapi.h>
#include <DirectXMath.h>

#include "../LogAndException/AboutException.h"
#include "../LogAndException/AboutDebugLog.h"
#include "../Light/DirectionalLight.h"
#include "../Texture/MyTexture.h"
#include "../Model/Model.h"
#include "../Input/Input.h"
#include "../VPShaderTable/VPShaderTable.h"
#include "../PipelineSet/PipelineSet.h"
#include "../Mesh/PrimitiveMesh.h"
#include "../FPSControll/FPSController.h"

#include "../../../External/imgui/imgui.h"
#include "../../../External/imgui/imgui_impl_dx12.h"
#include "../../../External/imgui/imgui_impl_win32.h"
#include "../../../External/DirectXTex/d3dx12.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);


//ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
	WPARAM wparam, LPARAM lparam);


struct DxCommon
{
	//[ debugController ]
#ifdef _DEBUG
	Microsoft::WRL::ComPtr < ID3D12Debug1> debugController = nullptr;

#endif // _DEBUG

	bool debugLayerSwitch = false;

	FPSController fpsController;

	//[ Input ]
	Inputs inputs;

	//[ Window ]
	uint32_t clientWidth;
	uint32_t clientHeight;
	//ウィンドウハンドル
	HWND hwnd;

	//[ Device ]
	//DXGIファクトリーの作成
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory = nullptr;
	//使用するアダプタ用の変数。
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter = nullptr;
	//デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;

	//[ DebugLog ]
	std::ofstream debugLog;

	//[ Command ]
	//コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	//コマンドアローケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
	//コマンドリスト		
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;

	//[ Fence ]
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	uint64_t fenceValue = 0;
	HANDLE fenceEvent = nullptr;

	//[ SwapChain ]
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain = nullptr;

	//[ Handle ]
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;


	//[ DescriptorHeap ]
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap = nullptr;

	//[ DescriptorSize ]
	uint32_t descriptorSizeSRV;
	uint32_t descriptorSizeRTV;
	uint32_t descriptorSizeDSV;
	
	//[ dxCompiler ]
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils = nullptr;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler = nullptr;

	//[ ShaderBlob ]
	Microsoft::WRL::ComPtr<IDxcBlob> d_vertexShaderBlob = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> d_pixcelShaderBlob = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> test_pixcelShaderBlob = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> kirei_pixcelShaderBlob = nullptr;


	//具体的にshaderがどこかでデータを読めばいいのかの情報をまとめたもの
	//shaderとPSOのバインダー
	// パイプラインセット
	std::unique_ptr<PipeLineSet> pipelineSetTriangles[kCountOfBlendMode][(int)CullMode::kCountOfCullMode][kCountOfShaderSetType];
	std::unique_ptr<PipeLineSet> pipelineSetLines[kCountOfBlendMode][(int)CullMode::kCountOfCullMode][kCountOfShaderSetType];
	//シェーダーセット
	VPShaderTable VP_Shadertable;

	//[ Reaource ]
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResources[2] = { nullptr };
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilTextureResource = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> commonVariablesResource = nullptr;


	//[ PrimitiveMesh ]
	PrimitiveMeshData primitiveMeshData;

	//sprite
	uint8_t const kMaxTexture = 128;
	//テクスチャのデータ
	std::vector<TextureResourceData> textureResourceDataList;
	//offscreen
	std::vector<OSResourceData> offScreenSet;

	PipelineComponents pipelineComponents;

	DirectionalLight* directionalLightData = nullptr;
	CommonVariables* commonVariablesData = nullptr;


	//torima
	float time = 0.0f;
	float deltaTime = 0.0f;
	float windowColor[4] = { 0.1f,0.1f,0.1f,1.0f };


	//===================↓初期化↓==========================
	void SetWindowAndSome(int32_t kWindowWidth_, int32_t kWindowHeight_, LPCWSTR const windowTitle_);
	//一番いいのを頼む
	void SetGoodDevice();
	//Fenceの設定
	void SetFence();
	//各DescriptorのSizeを計測
	void SetValuesOfDescriptorSize();
	//コマンドキューの生成
	void MakeCommandQueue();
	//コマンドアローケータの生成
	void MakeCommandAllocator();
	//コマンドリストを生成する
	void MakeCommandList();
	//swapChainの設定
	DXGI_SWAP_CHAIN_DESC1 SetSwapChain();
	//SwapChainからリソースを引っ張る
	void PullSwapChainResource();
	//DescriptorHeapの作成関数
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> MakeDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType_, UINT numDescriptors_, bool shaderVisible);
	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC SetRenderTargetView();
	//Dxのコンパイラーを設定する
	void SetDXcCompiler();
	//対象のシェーダーをコンパイルする関数
	IDxcBlob* CompileShader(
		//コンパイルするシェーダのパス
		std::wstring const& filePath_,
		//コンパイルに使用するプロファイル
		const wchar_t* profile_,
		//初期化で生成したものを3つ
		IDxcUtils* dxcUtils_,
		IDxcCompiler3* dxcCompiler_,
		IDxcIncludeHandler* includeHandler_);
	//実際にシェーダーをコンパイルする
	void CompileMyShaderes();
	//DescriptorRangeの設定を行う
	D3D12_DESCRIPTOR_RANGE  SetShaderViewDescriptorRange();
	//ラスタライザーの設定
	D3D12_RASTERIZER_DESC GetRasterizerDesc(CullMode cullMode_);
	//BufferResource(example: VertexBuffer,constantbuffer)を作る関数
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(UINT sizeInByte_);
	//textureデータを転送する関数
	[[nodiscard]]
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadtextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture_, DirectX::ScratchImage const& mipImages_);
	//ShaderResourceViewDescの生成(とりま適当引数)
	D3D12_SHADER_RESOURCE_VIEW_DESC CreateSRVDesc(DXGI_FORMAT metaDataFormat_, size_t mipLevels_);
	//DepthStencilを生成する
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(int32_t width_, int32_t height_);
	//DepthStencilViewDesc(DSVDesc)の設定
	D3D12_DEPTH_STENCIL_VIEW_DESC CreateDepthStencilViewDesc();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descrptorHeap_, uint32_t descriptorSize_,uint32_t index_);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descrptorHeap_ , uint32_t descriptorSize_, uint32_t index_);
	//メッシュ作成
	std::unique_ptr<Mesh> CreateMesh(UINT vertexCount, UINT indexCount, UINT kMaxInstance_);
	std::unique_ptr<Mesh> CreateMeshForLine(UINT vertexCount, UINT kMaxInstance_);
	std::unique_ptr<Mesh> CreateMeshForSphere(UINT kMaxInstance_);
	//モデル専用メッシュ作成関数
	std::unique_ptr<Mesh> CreateExclusiveMesh(size_t modelVerticesSize);
	//パイプラインセット作成用
	std::unique_ptr<PipeLineSet> CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType_, BlendMode blendMode_,
		CullMode cullMode_, ShaderSetType shaderSetType_);
	//複数のパイプラインセットを作成する
	void CreateGraphicsPipelineSets();

	//===================↑初期化↑==========================



	//===================↓更新処理↓==========================
	
	//DXの行列の設定
	void SetDXMatrix(D3D12_VIEWPORT& dst_viewport_, D3D12_RECT& dst_scissorRect_);
	//バリアステートを遷移させる
	void SetBarrierState(D3D12_RESOURCE_BARRIER& dst_barrier_, D3D12_RESOURCE_STATES before_, D3D12_RESOURCE_STATES after_);
	//次のフレーム用のコマンドリストを準備
	void PrepareForNextCommandList();
	//イベントを待つ
	void WaitFenceEvent();
	//三角形描画
	void DrawTriangle(VertexData left_, VertexData top_, VertexData right_, 
		Vector4<float> color_, int texHandle_,TransformationMatrix tMat_, DrawMode drawMode_, Matrix4 uvMat_, int enableLighting_);
	//矩形描画
	void DrawQuad(VertexData leftTop_, VertexData rightTop_, VertexData leftBottom_, VertexData rightBottom_,
		int texHandle_, Vector4<float> color_, TransformationMatrix tMat_, int enableLighting_, DrawMode drawMode_, Matrix4 uvMat_ );
	//線描画
	void DrawLine(Vector3 start_, Vector3 end_, int texHandle_,Vector4<float> color_, TransformationMatrix tMat_, Matrix4 uvMat_ );
	//球体描画
	void DrawSphere(Vector4<float> color_, TransformationMatrix tMat_, int texHandle_, DrawMode drawMode_, Matrix4 uvMat_, int enableLightning_);
	//6明太描画
	void DrawCube(VertexData* leftTop_, VertexData* rightTop_, VertexData* leftBottom_, VertexData* rightBottom_,
		int* texHandle_, TransformationMatrix* tMat_, Vector4<float> color_, bool enableLighting_,DrawMode drawMode_, Matrix4* uvMat_);
	//4明太描画
	void DrawTetrahedra(VertexData* left_, VertexData* top_, VertexData* right_,
		Vector4<float> color_, int texHandle_, TransformationMatrix* tMat_, DrawMode drawMode_, Matrix4* uvMat_);
	//モデル描画
	void DrawModel(Mesh* mesh_, UINT verticesSIze_, int texHandle_, Vector4<float> color_, TransformationMatrix tMat_, int enableLighting_, Matrix4 uvMat_);


	//カーソルのポジションを取得
	void SetMousePos(POINT* point_);
	//描画インデックスのリセット
	void DrawIndexReset();
	//Draw前共通処理
	void BeginFrame(BYTE* key, DIMOUSESTATE* mouseState_);
	//Draw後共通処理
	void EndFrame();
	//テクスチャを読み込んで転送し、ShaderResourceViewも生成する
	UINT LoadTextureFile(std::string filePath_);
	uint16_t CreateOS_Set();
	void SetBlendMode(BlendMode mode_);
	void SetCullMode(CullMode cullMode_);
	void SetShaderSet(ShaderSetType shaderSetType_);
	//DirectionalLightDataの書き込む(ライトはとりまひとつ)
	void InputDirectionalLightData(Vector3 worldPos_, Vector4<float> color_, float intensity_);
	void SetCameraPos(Vector3 cameraWorld_);

	void OffScreecBegin(OSResourceData& osData_);
	void OffScreenEnd(OSResourceData& osData_);
	void ChangeWindowColor(Vector4<float> col_);
	//===================↑更新処理↑==========================

	void Initialize(int32_t kWindowWidth_, int32_t kWindowHeight_, LPCWSTR const windowTitle_);
	void Finalize();
	void ImGuiFinalize();
};




