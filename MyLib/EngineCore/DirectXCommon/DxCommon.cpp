#include "DxCommon.h"
#include <timeapi.h>

#pragma comment(lib,"dxcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"Dbghelp.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")



void DxCommon::Initialize(int32_t kWindowWidth_, int32_t kWindowHeight_, LPCWSTR const windowTitle_)
{
	//COMの初期化
	CoInitializeEx(0, COINITBASE_MULTITHREADED);

	//Window、デバッグログ、エラー検知の初期化
	SetWindowAndSome(kWindowWidth_, kWindowHeight_, windowTitle_);
	//デバイス探してセットする
	SetGoodDevice();
	//各種Descriptorのサイズを測定
	SetValuesOfDescriptorSize();
	//フェンスの設定
	SetFence();
	//コンパイラーの初期化
	SetDXcCompiler();
	//コマンドキューの生成
	MakeCommandQueue();
	//コマンドアローケータの生成
	MakeCommandAllocator();
	//コマンドリストを生成する
	MakeCommandList();
	//swapChainの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = SetSwapChain();
	//SwapChainResourceからResourceを引っ張ってくる
	PullSwapChainResource();


	//RTV用のヒープでディスクリプタの数は2。RTVはShader内で触るものではないので,ShaderVisibleはfalse
	rtvDescriptorHeap = MakeDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 12, false);
	//SRV用のヒープ。SRVはShader内で触るもなので,ShaderVisibleはtrue
	srvDescriptorHeap = MakeDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxTexture, true);
	//DSV用のディスクリプタヒープ(Shader内で見るものではない)
	dsvDescriptorHeap = MakeDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);

	//シェーダをコンパイルする
	CompileMyShaderes();

	//DirectionalLightResourceの生成
	directionalLightResource = CreateBufferResource( sizeof(DirectionalLight));
	//書き込むためのアドレスを取得
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	//depthStencilresourceの作成
	depthStencilTextureResource = CreateDepthStencilTextureResource(clientWidth, clientHeight);

	//DirectionalLightResourceの生成
	commonVariablesResource = CreateBufferResource(sizeof(CommonVariables));
	//書き込むためのアドレスを取得
	commonVariablesResource->Map(0, nullptr, reinterpret_cast<void**>(&commonVariablesData));

	//PSO,rootSignatureセットの作成
	CreateGraphicsPipelineSets();

	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = SetRenderTargetView();
	//DepthStencilViewDescの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = CreateDepthStencilViewDesc();
	//DSVHeapの先頭にDSVを作る
	device->CreateDepthStencilView(depthStencilTextureResource.Get(), &depthStencilViewDesc,
		dsvDescriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart());

	//ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsClassic();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(
		device.Get(),
		swapChainDesc.BufferCount,
		rtvDesc.Format,
		srvDescriptorHeap.Get(),
		GetCPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV, 0),		//SRVHeap上の０番目
		GetGPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV, 0));

	//CreateOS_Set();

	//TriangleMesh
	primitiveMeshData.triangleMesh = CreateMesh(primitiveMeshData.kNumVertexPerTriangle * primitiveMeshData.kMaxTriangle, 
		0, primitiveMeshData.kMaxTriangle);
	//QuadMesh
	primitiveMeshData.quadMesh = CreateMesh(primitiveMeshData.kNumVertexPerQuad * primitiveMeshData.kMaxQuad, 
		primitiveMeshData.kNumIndexPerQuad * primitiveMeshData.kMaxQuad, primitiveMeshData.kMaxQuad);
	//LineMesh
	primitiveMeshData.lineMesh = CreateMeshForLine(primitiveMeshData.
		kNumVertexPerLine * primitiveMeshData.kMaxLine, primitiveMeshData.kMaxLine);
	//SphereMesh
	primitiveMeshData.sphereMesh = CreateMeshForSphere(primitiveMeshData.kMaxSphere);



}// Initialize


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,WPARAM wparam, LPARAM lparam)
{
	//ImGuiにメッセージを渡す
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);

		return 0;
	}

	//標準メッセージの処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
//===================↓初期化↓==========================
void DxCommon::SetGoodDevice()
{
	//HRESULTはWindows系のエラーコードであり、
	//関数が成功したかどうかをSUCCEEDEDマクロで判定する
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	//初期化の根本的な部分でエラーが出た場合はプログラムが間違っているか、
	//どうにもできない場合が多いのでassertにしておく
	assert(SUCCEEDED(hr));
	//一番いいアダプタを頼む
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		//アダプタの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDisc{};
		hr = useAdapter->GetDesc3(&adapterDisc);
		//取得できなければエラー
		assert(SUCCEEDED(hr));
		//ソフトウェアアダプタでなければ採用
		if (!(adapterDisc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			//採用したアダプタの情報をログに出力
			Log(debugLog, ConvertString(std::format(L"Use Adapter:{}\n", adapterDisc.Description)));

			break;
		}
		//ソフトウェアアダプタの場合はなかったことに
		useAdapter = nullptr;
	}

	//適切なアダプタが見つからなかった場合は起動できない
	assert(useAdapter != nullptr);

	//機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0
	};
	std::string featureLevelStrings[] = { "12.2","12.1","12.0" };
	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i)
	{
		//採用したアダプタでデバイスを生成
		hr = D3D12CreateDevice(useAdapter.Get(), featureLevels[i], IID_PPV_ARGS(&device));
		//指定した機能レベルでデバイスが生成できたか確認
		if (SUCCEEDED(hr))
		{
			//生成できたのでログを出力してループを抜ける
			Log(debugLog, std::format("FeatureLevel:{}\n", featureLevelStrings[i]));
			break;
		}
	}

	//デバイスの生成が上手くいかなかったので起動できない
	assert(device != nullptr);
	Log(debugLog, "Complete create D3D12Device\n");

	//エラー解消のために
#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		//ヤバいエラーの時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		//infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);


		//抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] =
		{
			//Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤー
			//相互作用バグによるエラーメッセージ
			//http://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);

		//解放
		infoQueue->Release();
	}

#endif
}
void DxCommon::SetFence()
{
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));
	Log(debugLog, "Complete built fence\n");

	//FenceのSignalを待つためのイベント（WINDOWへのメッセージ）を作成する
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
	Log(debugLog, "Complete creat event\n");

}
void DxCommon::SetWindowAndSome(int32_t kWindowWidth_, int32_t kWindowHeight_, LPCWSTR const windowTitle_)
{
	
#ifdef _DEBUG
	if (debugLayerSwitch)
	{
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
			debugController->SetEnableGPUBasedValidation(TRUE);
		}
	}

#endif // _DEBUG

	WNDCLASS wc{};
	//ウィンドウプロシージャ(複数の処理を一つにまとめたもの)
	wc.lpfnWndProc = WindowProc;
	//ウィンドウクラス名
	wc.lpszClassName = windowTitle_;
	//インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//ウィンドウクラスを登録する
	RegisterClass(&wc);
	//クライアントの領域サイズ
	clientWidth = kWindowWidth_;
	clientHeight = kWindowHeight_;

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0,(LONG)clientWidth,(LONG)clientHeight };
	//クライアント領域をもとに実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//[ ウィンドウの生成 ]
	hwnd = CreateWindow(
		wc.lpszClassName,		//利用するクラス名
		windowTitle_,			//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル
		CW_USEDEFAULT,			//表示x座標
		CW_USEDEFAULT,			//表示y座標
		wrc.right - wrc.left,	//ウィンドウ横幅
		wrc.bottom - wrc.top,	//ウィンドウ縦幅
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		wc.hInstance,			//インスタンスハンドル
		nullptr);				//オプション

	//[ ウィンドウを表示する ]
	ShowWindow(hwnd, SW_SHOW);

	//[ Log ]
	std::filesystem::create_directory("DebugLog");
	debugLog = DebugLogInitialize();

	//[ デバッグヘルパー ]
	//誰も捕捉しなかった場合に(Unhandled)、補足する関数を登録
	//main関数始まってすぐに登録するといい
	SetUnhandledExceptionFilter(ExportDump);


	//DirectInputの初期化
	HRESULT hr = DirectInput8Create(wc.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&inputs.directInput, nullptr);
	assert(SUCCEEDED(hr));
	//キーボードデバイスの生成
	hr = inputs.directInput->CreateDevice(GUID_SysKeyboard, &inputs.keyboard, NULL);
	assert(SUCCEEDED(hr));
	//入力データ形式のセット
	hr = inputs.keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));
	//排他制御レベルのセット
	hr = inputs.keyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

	//マウス用にデバイスオブジェクトを作成
	hr = DirectInput8Create(wc.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&inputs.directInputForMouse, nullptr);
	assert(SUCCEEDED(hr));

	hr = inputs.directInputForMouse->CreateDevice(GUID_SysMouse, &inputs.mouse, NULL);
	assert(SUCCEEDED(hr));
	hr = inputs.mouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(hr));
		hr = inputs.mouse->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	assert(SUCCEEDED(hr));

}
void DxCommon::SetValuesOfDescriptorSize()
{
	descriptorSizeSRV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descriptorSizeRTV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	descriptorSizeDSV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}
void DxCommon::MakeCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	HRESULT hr = device->CreateCommandQueue(&commandQueueDesc,
		IID_PPV_ARGS(&commandQueue));
	//コマンドキューの生成がうまくいかなかった場合はエラー
	assert(SUCCEEDED(hr));
	Log(debugLog, "Complete create CommandQueue\n");
}
void DxCommon::MakeCommandAllocator()
{
	HRESULT hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	//生成がうまくいかなかった場合はエラー
	assert(SUCCEEDED(hr));
	Log(debugLog, "Complete create AlloCator\n");
}
void DxCommon::MakeCommandList()
{
	HRESULT hr = device.Get()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(),
		nullptr, IID_PPV_ARGS(&commandList));
	//生成失敗
	assert(SUCCEEDED(hr));
	Log(debugLog, "Complete create CommandList\n");
}
DXGI_SWAP_CHAIN_DESC1 DxCommon::SetSwapChain()
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	//画面の縦横。クライアント領域と同じにしておく
	swapChainDesc.Width = clientWidth;
	swapChainDesc.Height = clientHeight;
	//色の形成
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//マルチサンプルしない
	swapChainDesc.SampleDesc.Count = 1;
	//描画のターゲットとして利用する
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//ダブルバッファ
	swapChainDesc.BufferCount = 2;
	//モニタに移したら中身を破棄
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	HRESULT hr = dxgiFactory.Get()->CreateSwapChainForHwnd(commandQueue.Get(), hwnd, &swapChainDesc,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf()));
	//生成失敗
	assert(SUCCEEDED(hr));
	Log(debugLog, "Complete create SwapChain\n");

	return swapChainDesc;
}
void DxCommon::PullSwapChainResource()
{
	Log(debugLog, "Complete create rtvDescriptorHeap\n");

	//SwapChainからResourceを引っ張ってくる
	HRESULT hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));
	//生成失敗
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChainResources[1]));
	//生成失敗
	assert(SUCCEEDED(hr));
	Log(debugLog, "Complete pull Resource\n");
}
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DxCommon::MakeDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType_, UINT numDescriptors_, bool shaderVisible)
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ret_descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorheapDesc{};
	descriptorheapDesc.Type = heapType_;
	descriptorheapDesc.NumDescriptors = numDescriptors_;
	descriptorheapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = device->CreateDescriptorHeap(&descriptorheapDesc,
		IID_PPV_ARGS(ret_descriptorHeap.GetAddressOf()));

	assert(SUCCEEDED(hr));

	return ret_descriptorHeap;
}
D3D12_RENDER_TARGET_VIEW_DESC DxCommon::SetRenderTargetView()
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//出力結果をSRGBに変換する
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//2Dテクスチャとして書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	//1つめを作る。１つ目は最初に作る。作る場所をこちらで指定する必要がある
	rtvHandles[0] = rtvStartHandle;
	device->CreateRenderTargetView(swapChainResources[0].Get(), &rtvDesc, rtvHandles[0]);
	//2つめのディスクリプタハンドルを得る
	rtvHandles[1].ptr = rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//2つめを作る
	device->CreateRenderTargetView(swapChainResources[1].Get(), &rtvDesc, rtvHandles[1]);


	return rtvDesc;
}
void DxCommon::SetDXcCompiler()
{
	HRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr));
	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));
}
IDxcBlob* DxCommon::CompileShader(
	std::wstring const& filePath_,
	const wchar_t* profile_,
	IDxcUtils* dxcUtils_,
	IDxcCompiler3* dxcCompiler_,
	IDxcIncludeHandler* includeHandler_)
{
	//[ 1.hlslファイルを読み込む ]
	Log(ConvertString(std::format(L"Begin CompileShader, path:{},profile:{}\n", filePath_, profile_)));
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils_->LoadFile(filePath_.c_str(), nullptr, &shaderSource);
	assert(SUCCEEDED(hr));
	Log(debugLog, "Complete load shader\n");

	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	//UTF-8のもじであることを通知
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	//[ 2.コンパイルする ]
	LPCWSTR arguments[] =
	{
		//コンパイル対象のhlslファイル名
		filePath_.c_str(),
		//エントリーポイントの指定。基本的にmain以外にしない
		L"-E",L"main",
		//shaderProfileの設定
		L"-T",profile_,
		//デバッグ用の情報を埋め込む
		L"-Zi",L"-Qembed_debug",
		//最適化を外す
		L"-Od",
		//メモリレイアウトは行優先
		L"-Zpr"
	};

	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler_->Compile(
		//読み込んだファイル
		&shaderSourceBuffer,
		//コンパイルオプション
		arguments,
		//コンパイルオプションの数
		_countof(arguments),
		//includeが含まれたもろもろ
		includeHandler_,
		IID_PPV_ARGS(&shaderResult)
	);
	//dxcが起動できないなどの致命的な状況
	assert(SUCCEEDED(hr));
	Log(debugLog, "Complete compile\n");

	//[ 3.エラー、警告確認 ]
	//警告エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0)
	{
		Log(shaderError->GetStringPointer());
		assert(false);
	}

	//[ 4.コンパイル結果を受け取る ]
	//コンパイル結果から実行用のバイナリ部分を受け取る
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	Log(debugLog, ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath_, profile_)));

	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();

	//実行用のバイナリをリターン
	return shaderBlob;
}
void DxCommon::CompileMyShaderes()
{

	d_vertexShaderBlob = CompileShader(VP_Shadertable.table[ShaderSetType::kV0P0].vsFilePath,
		L"vs_6_0", dxcUtils.Get(), dxcCompiler.Get(), includeHandler.Get());
	assert(d_vertexShaderBlob != nullptr);
	Log(debugLog, "VertexShader Compiled\n");


	d_pixcelShaderBlob = CompileShader(VP_Shadertable.table[ShaderSetType::kV0P0].psFlilePath,
		L"ps_6_0", dxcUtils.Get(), dxcCompiler.Get(), includeHandler.Get());
	assert(d_pixcelShaderBlob != nullptr);
	Log(debugLog, "PixcelShader Compiled\n");

	test_pixcelShaderBlob = CompileShader(VP_Shadertable.table[ShaderSetType::kV0PTest].psFlilePath,
		L"ps_6_0", dxcUtils.Get(), dxcCompiler.Get(), includeHandler.Get());
	assert(test_pixcelShaderBlob != nullptr);
	Log(debugLog, "test_pixcelShader Compiled\n");

	kirei_pixcelShaderBlob = CompileShader(VP_Shadertable.table[ShaderSetType::kKirei].psFlilePath,
		L"ps_6_0", dxcUtils.Get(), dxcCompiler.Get(), includeHandler.Get());
	assert(kirei_pixcelShaderBlob != nullptr);
	Log(debugLog, "kirei_pixcelShader Compiled\n");



}

D3D12_DESCRIPTOR_RANGE DxCommon::SetShaderViewDescriptorRange()
{
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//0から始まる
	descriptorRange[0].BaseShaderRegister = 0;
	//数は1つ
	descriptorRange[0].NumDescriptors = 1;
	//SRVを使う
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//offsetを自動計算
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return descriptorRange[0];
}
std::unique_ptr<PipeLineSet> DxCommon::CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType_, BlendMode blendMode_,
	CullMode cullMode_,ShaderSetType shaderSetType_)
{
	std::unique_ptr<PipeLineSet> pipelineSet = std::make_unique<PipeLineSet>();
	//graphiscPipelineDesc
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graghicsPipeLineStatedesc{};

	switch (shaderSetType_)
	{
	case kV0P0:
		graghicsPipeLineStatedesc.VS =
		{
			d_vertexShaderBlob->GetBufferPointer(),
			d_vertexShaderBlob->GetBufferSize()
		};

		//pixcelShader
		graghicsPipeLineStatedesc.PS =
		{
			d_pixcelShaderBlob->GetBufferPointer(),
			d_pixcelShaderBlob->GetBufferSize()
		};

		break;

	case kV0PTest:
		graghicsPipeLineStatedesc.VS =
		{
			d_vertexShaderBlob->GetBufferPointer(),
			d_vertexShaderBlob->GetBufferSize()
		};

		//pixcelShader
		graghicsPipeLineStatedesc.PS =
		{
			test_pixcelShaderBlob->GetBufferPointer(),
			test_pixcelShaderBlob->GetBufferSize()
		};

		break;

	case kKirei:
		graghicsPipeLineStatedesc.VS =
		{
			d_vertexShaderBlob->GetBufferPointer(),
			d_vertexShaderBlob->GetBufferSize()
		};

		//pixcelShader
		graghicsPipeLineStatedesc.PS =
		{
			kirei_pixcelShaderBlob->GetBufferPointer(),
			kirei_pixcelShaderBlob->GetBufferSize()
		};

		break;

	}

	//DescriptorRangeの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange[1];
	descriptorRange[0] = SetShaderViewDescriptorRange();

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};

	//バイリニアフィルター
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	//0～1の範囲外をリピート
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//比較しない
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//ありったけのmipMapを使う
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	//レジスタ番号0を使う
	staticSamplers[0].ShaderRegister = 0;
	//PixcelShaderで使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	//RootParameter
	D3D12_ROOT_PARAMETER rootParameters[5] = {};
	//CBVを使う(b0の0)
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//PixcelShaderで使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号0とバインド
	rootParameters[0].Descriptor.ShaderRegister = 0;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//VertexShaderで使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//レジスタ番号0とバインド
	rootParameters[1].Descriptor.ShaderRegister = 0;

	//Descriptortableを使う
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//pixcelShaderを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//tableの中身の配列を指定
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	//tableで利用する
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	//CBVを使う
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//pixcelShaderを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号1を使う
	rootParameters[3].Descriptor.ShaderRegister = 1;

	//CBVを使う
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//pixcelShaderを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//レジスタ番号1を使う
	rootParameters[4].Descriptor.ShaderRegister = 2;


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.pStaticSamplers = staticSamplers;
	rootSignatureDesc.NumStaticSamplers = _countof(staticSamplers);
	//ルートパラメータ配列へのポインタ
	rootSignatureDesc.pParameters = rootParameters;
	//配列の長さ
	rootSignatureDesc.NumParameters = _countof(rootParameters);
	//pixcelShaderで読むConstantBufferのBind情報を追加する
	rootSignatureDesc.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	//シリアライズしてバイナリにする
	HRESULT hr = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	//バイナリをもとにrootSignatureを作成
	hr = device->CreateRootSignature(0,
		signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet->rootSignature));
	assert(SUCCEEDED(hr));
	Log(debugLog, "Complete create rootSignature\n");

	//RootSignature
	graghicsPipeLineStatedesc.pRootSignature = pipelineSet->rootSignature.Get();
	//InputLayOut
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};

	inputElementDescs[0].SemanticName = "SV_POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);
	graghicsPipeLineStatedesc.InputLayout = inputLayoutDesc;


	D3D12_BLEND_DESC blendDesc{};

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	switch (blendMode_) {
		case kBlendModeNormal:

		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;


		break;
		case kBlendModeAdd:
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			break;
	}

	graghicsPipeLineStatedesc.BlendState = blendDesc; 
	//ラスタライザーステート
	D3D12_RASTERIZER_DESC rasterizerDesc = GetRasterizerDesc(cullMode_);
	graghicsPipeLineStatedesc.RasterizerState = rasterizerDesc;
	//書き込むRTVの情報
	graghicsPipeLineStatedesc.NumRenderTargets = 1;
	graghicsPipeLineStatedesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトポロジ（形状）のタイプ三角形
	graghicsPipeLineStatedesc.PrimitiveTopologyType = topologyType_;//D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
	//どのようにして色を打ち込むかの設定
	graghicsPipeLineStatedesc.SampleDesc.Count = 1;
	graghicsPipeLineStatedesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//DepthStencilStateの設定
	//depthの機能を有効化する
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	//書き込みする
	if (blendMode_ == kBlendModeNormal)
	{
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	}

	else if (blendMode_ == kBlendModeAdd)
	{
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}

	//比較関数はLessEqual。つかり近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	graghicsPipeLineStatedesc.DepthStencilState = depthStencilDesc;
	graghicsPipeLineStatedesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//実際に生成
	hr = device->CreateGraphicsPipelineState(&graghicsPipeLineStatedesc,
		IID_PPV_ARGS(&pipelineSet->pipelineStateObject));
	assert(SUCCEEDED(hr));
	Log(debugLog, "Complete create graghicsPipelineState\n");

	return pipelineSet;
}
void DxCommon::CreateGraphicsPipelineSets() 
{
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kFront][ShaderSetType::kV0P0] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kFront, kV0P0);
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kFront][ShaderSetType::kV0PTest] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kFront, ShaderSetType::kV0PTest);
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kFront][ShaderSetType::kKirei] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kFront, ShaderSetType::kKirei);

	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kBack][ShaderSetType::kV0P0] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kBack, kV0P0);
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kBack][ShaderSetType::kV0PTest] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kBack, ShaderSetType::kV0PTest);
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kBack][ShaderSetType::kKirei] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kBack, ShaderSetType::kKirei);

	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kNone][ShaderSetType::kV0P0] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kNone, kV0P0);
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kNone][ShaderSetType::kV0PTest] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kNone, ShaderSetType::kV0PTest);
	pipelineSetTriangles[kBlendModeNormal][(int)CullMode::kNone][ShaderSetType::kKirei] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeNormal, CullMode::kNone, ShaderSetType::kKirei);


	pipelineSetLines[kBlendModeNormal][(int)CullMode::kFront][ShaderSetType::kV0P0] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeNormal, CullMode::kFront, kV0P0);
	pipelineSetLines[kBlendModeNormal][(int)CullMode::kFront][ShaderSetType::kV0PTest] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeNormal, CullMode::kFront, ShaderSetType::kV0PTest);
	pipelineSetLines[kBlendModeNormal][(int)CullMode::kBack][ShaderSetType::kV0P0] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeNormal, CullMode::kBack, kV0P0);
	pipelineSetLines[kBlendModeNormal][(int)CullMode::kBack][ShaderSetType::kV0PTest] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeNormal, CullMode::kBack, ShaderSetType::kV0PTest);
	pipelineSetLines[kBlendModeNormal][(int)CullMode::kNone][ShaderSetType::kV0P0] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeNormal, CullMode::kNone, kV0P0);
	pipelineSetLines[kBlendModeNormal][(int)CullMode::kNone][ShaderSetType::kV0PTest] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeNormal, CullMode::kNone, ShaderSetType::kV0PTest);

	//--------------------------------------------------------------------------------------------------------------------------------
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kFront][ShaderSetType::kV0P0] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kFront, kV0P0);
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kFront][ShaderSetType::kV0PTest] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kFront, ShaderSetType::kV0PTest);
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kFront][ShaderSetType::kKirei] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kFront, ShaderSetType::kKirei);

	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kBack][ShaderSetType::kV0P0] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kBack, kV0P0);
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kBack][ShaderSetType::kV0PTest] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kBack, ShaderSetType::kV0PTest);
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kBack][ShaderSetType::kKirei] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kBack, ShaderSetType::kKirei);

	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kNone][ShaderSetType::kV0P0] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kNone, kV0P0);
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kNone][ShaderSetType::kV0PTest] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kNone, ShaderSetType::kV0PTest);
	pipelineSetTriangles[kBlendModeAdd][(int)CullMode::kNone][ShaderSetType::kKirei] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, kBlendModeAdd, CullMode::kNone, ShaderSetType::kKirei);


	pipelineSetLines[kBlendModeAdd][(int)CullMode::kFront][ShaderSetType::kV0P0] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAdd, CullMode::kFront, kV0P0);
	pipelineSetLines[kBlendModeAdd][(int)CullMode::kFront][ShaderSetType::kV0PTest] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAdd, CullMode::kFront, ShaderSetType::kV0PTest);
	pipelineSetLines[kBlendModeAdd][(int)CullMode::kBack][ShaderSetType::kV0P0] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAdd, CullMode::kBack, kV0P0);
	pipelineSetLines[kBlendModeAdd][(int)CullMode::kBack][ShaderSetType::kV0PTest] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAdd, CullMode::kBack, ShaderSetType::kV0PTest);
	pipelineSetLines[kBlendModeAdd][(int)CullMode::kNone][ShaderSetType::kV0P0] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAdd, CullMode::kNone, kV0P0);
	pipelineSetLines[kBlendModeAdd][(int)CullMode::kNone][ShaderSetType::kV0PTest] =
		CreateGraphicsPipelineSet(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAdd, CullMode::kNone, ShaderSetType::kV0PTest);

}
D3D12_RASTERIZER_DESC DxCommon::GetRasterizerDesc(CullMode cullMode_)
{
	D3D12_RASTERIZER_DESC ret_rasterizerDesc{};

	//裏面（時計回り）の表示をしない
	switch (cullMode_)
	{
	case CullMode::kFront:
		ret_rasterizerDesc.CullMode = D3D12_CULL_MODE_FRONT;
		break;
	case CullMode::kBack:
		ret_rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;

		break;
	case CullMode::kNone:
		ret_rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}
	//三角形の中身を塗りつぶす
	ret_rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	return ret_rasterizerDesc;
}
Microsoft::WRL::ComPtr<ID3D12Resource> DxCommon::CreateBufferResource(UINT sizeInByte_)
{
	Microsoft::WRL::ComPtr<ID3D12Resource> ret_resource;
	//頂点リソースのヒープ設定
	//upLoadHEapを使う
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//リソースのサイズ。
	resourceDesc.Width = sizeInByte_;
	//バッファの場合はこれらを1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	//決まり2
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際に頂点リソースを作る
	HRESULT hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&ret_resource));
	assert(SUCCEEDED(hr));

	return ret_resource;
}

Microsoft::WRL::ComPtr<ID3D12Resource> DxCommon::UploadtextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture_, DirectX::ScratchImage const& mipImages_)
{
	std::vector<D3D12_SUBRESOURCE_DATA> subresource;
	DirectX::PrepareUpload(device.Get(), mipImages_.GetImages(),
		mipImages_.GetImageCount(), mipImages_.GetMetadata(), subresource);

	UINT intermediateSize = static_cast<UINT>(GetRequiredIntermediateSize(texture_.Get(), 0, UINT(subresource.size())));

	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = CreateBufferResource(intermediateSize);

	UpdateSubresources(commandList.Get(), texture_.Get(), intermediateResource.Get(), 0, 0,
		UINT(subresource.size()), subresource.data());
	//Textureへの転送後は利用できるよう、D3D2_RESOUTRCE_STATE_COPY_DESTから
	//D3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture_.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	commandList->ResourceBarrier(1, &barrier);

	return intermediateResource;
}
D3D12_SHADER_RESOURCE_VIEW_DESC DxCommon::CreateSRVDesc(DXGI_FORMAT metaDataFormat_, size_t mipLevels_)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metaDataFormat_;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(mipLevels_);
	//srvDesc.Texture2D.MostDetailedMip = 0;//


	return srvDesc;
}
Microsoft::WRL::ComPtr<ID3D12Resource> DxCommon::CreateDepthStencilTextureResource(int32_t width_, int32_t height_)
{
	//生成するResourceの設定
	//テクスチャのサイズ
	D3D12_RESOURCE_DESC resourceDesc{};

	resourceDesc.Width = width_;
	resourceDesc.Height = height_;
	//mipmapの数
	resourceDesc.MipLevels = 1;
	//奥行or配列Textureの配列数
	resourceDesc.DepthOrArraySize = 1;
	//depthstencilとして利用可能なフォーマット
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//サンプリングカウント1固定
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//DepthStencilとして使う通知
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//利用するHeapの設定
	//VRAM上に作る
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	//深度値クリア設定
	//最大値（1.0）でクリア
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	//フォーマット。resourceと合わせる
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//Resourceの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> ret_resource = nullptr;
	HRESULT hr = device->CreateCommittedResource
	(
		&heapProperties,					//Heapの設定
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定：特になし
		&resourceDesc,						//resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値を書き込む状態にしておく
		&depthClearValue,					//Clear最適値
		IID_PPV_ARGS(&ret_resource)			//作成するリソースポインタへのポインタ
	);

	assert(SUCCEEDED(hr));

	return ret_resource;
}
D3D12_DEPTH_STENCIL_VIEW_DESC DxCommon::CreateDepthStencilViewDesc()
{
	D3D12_DEPTH_STENCIL_VIEW_DESC ret_dsvDesc{};

	//format。基本的にはリソースに合わせる
	ret_dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//2DTexture
	ret_dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	return ret_dsvDesc;
}
D3D12_CPU_DESCRIPTOR_HANDLE DxCommon::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descrptorHeap_,
	uint32_t descriptorSize_, uint32_t index_)
{
	D3D12_CPU_DESCRIPTOR_HANDLE ret_handleCPU = descrptorHeap_->GetCPUDescriptorHandleForHeapStart();
	ret_handleCPU.ptr += descriptorSize_ * index_;

	return ret_handleCPU;
}
D3D12_GPU_DESCRIPTOR_HANDLE DxCommon::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descrptorHeap_,
	uint32_t descriptorSize_, uint32_t index_)
{
	D3D12_GPU_DESCRIPTOR_HANDLE ret_handleGPU = descrptorHeap_->GetGPUDescriptorHandleForHeapStart();
	ret_handleGPU.ptr += descriptorSize_ * index_;

	return ret_handleGPU;
}
std::unique_ptr<Mesh> DxCommon::CreateExclusiveMesh(size_t modelVerticesSize)
{
	std::unique_ptr<Mesh> ret_mesh;

	// 頂点データのサイズ
	UINT sizeOfVertexBuffer = static_cast<UINT>(sizeof(VertexData) * static_cast<UINT>(modelVerticesSize));
	// 頂点バッファ生成
	ret_mesh->vertexBuff = CreateBufferResource(sizeOfVertexBuffer);
	// 頂点バッファビューの作成
	ret_mesh->vertexBufferView.BufferLocation = ret_mesh->vertexBuff->GetGPUVirtualAddress();
	ret_mesh->vertexBufferView.SizeInBytes = sizeOfVertexBuffer;
	ret_mesh->vertexBufferView.StrideInBytes = sizeof(VertexData);
	// 頂点バッファのマッピング
	HRESULT result = ret_mesh->vertexBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->vertexMap));
	assert(SUCCEEDED(result));


	ret_mesh->mappedResource.resize(1);

	// 行列バッファ生成
	ret_mesh->mappedResource[0].transformationMatrixBuff = CreateBufferResource(sizeof(TransformationMatrix));
	// 行列バッファのマッピング
	ret_mesh->mappedResource[0].transformationMatrixBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->mappedResource[0].transformationMatrixMap));
	// マテリアルバッファ作成
	ret_mesh->mappedResource[0].materialBuff = CreateBufferResource(sizeof(Material));
	// マテリアルバッファのマッピング
	ret_mesh->mappedResource[0].materialBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->mappedResource[0].materialMap));

	return ret_mesh;
}
std::unique_ptr<Mesh> DxCommon::CreateMesh(UINT vertexCount, UINT indexCount,UINT kMaxInstance_)
{
	std::unique_ptr<Mesh> ret_mesh = std::make_unique<Mesh>();

	if (vertexCount > 0) {
		// 頂点データのサイズ
		UINT sizeOfVertexBuffer = static_cast<UINT>(sizeof(VertexData) * vertexCount);
		// 頂点バッファ生成
		ret_mesh->vertexBuff = CreateBufferResource(sizeOfVertexBuffer);

		// 頂点バッファビューの作成
		ret_mesh->vertexBufferView.BufferLocation = ret_mesh->vertexBuff->GetGPUVirtualAddress();
		ret_mesh->vertexBufferView.SizeInBytes = sizeOfVertexBuffer;
		ret_mesh->vertexBufferView.StrideInBytes = sizeof(VertexData);

		// 頂点バッファのマッピング
		HRESULT result = ret_mesh->vertexBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->vertexMap));
		assert(SUCCEEDED(result));
	}

	if (indexCount > 0) {
		// インデックスデータのサイズ
		UINT sizeOfIndexBuffer = static_cast<UINT>(sizeof(uint32_t) * indexCount);
		// インデックスバッファ生成
		ret_mesh->indexBuff = CreateBufferResource(sizeOfIndexBuffer);

		// インデックスバッファビューの作成
		ret_mesh->indexBufferView.BufferLocation = ret_mesh->indexBuff->GetGPUVirtualAddress();
		ret_mesh->indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		ret_mesh->indexBufferView.SizeInBytes = sizeOfIndexBuffer;

		// インデックスバッファのマッピング
		HRESULT result = ret_mesh->indexBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->indexMap));
		assert(SUCCEEDED(result));
	}

	if (kMaxInstance_ > 0)
	{
		ret_mesh->mappedResource.resize(kMaxInstance_);

		for (UINT i = 0; i < kMaxInstance_; ++i)
		{
			// 行列バッファ生成
			ret_mesh->mappedResource[i].transformationMatrixBuff = CreateBufferResource(sizeof(TransformationMatrix));
			// 行列バッファのマッピング
			ret_mesh->mappedResource[i].transformationMatrixBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->mappedResource[i].transformationMatrixMap));
			// マテリアルバッファ作成
			ret_mesh->mappedResource[i].materialBuff = CreateBufferResource(sizeof(Material));
			// マテリアルバッファのマッピング
			ret_mesh->mappedResource[i].materialBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->mappedResource[i].materialMap));
		}
	}

	return ret_mesh;
}
std::unique_ptr<Mesh> DxCommon::CreateMeshForLine(UINT vertexCount, UINT kMaxInstance_)
{
	std::unique_ptr<Mesh> ret_mesh = std::make_unique<Mesh>();

	if (vertexCount > 0) {
		// 頂点データのサイズ
		UINT sizeOfVertexBuffer = static_cast<UINT>(sizeof(Vector3) * vertexCount);
		// 頂点バッファ生成
		ret_mesh->vertexBuff = CreateBufferResource(sizeOfVertexBuffer);

		// 頂点バッファビューの作成
		ret_mesh->vertexBufferView.BufferLocation = ret_mesh->vertexBuff->GetGPUVirtualAddress();
		ret_mesh->vertexBufferView.SizeInBytes = sizeOfVertexBuffer;
		ret_mesh->vertexBufferView.StrideInBytes = sizeof(Vector3);

		// 頂点バッファのマッピング
		HRESULT result = ret_mesh->vertexBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->vertexMapForLine));
		assert(SUCCEEDED(result));
	}

	if (kMaxInstance_ > 0)
	{
		ret_mesh->mappedResource.resize(kMaxInstance_);

		for (UINT i = 0; i < kMaxInstance_; ++i)
		{
			// 行列バッファ生成
			ret_mesh->mappedResource[i].transformationMatrixBuff = CreateBufferResource(sizeof(TransformationMatrix));
			// 行列バッファのマッピング
			ret_mesh->mappedResource[i].transformationMatrixBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->mappedResource[i].transformationMatrixMap));
			// マテリアルバッファ作成
			ret_mesh->mappedResource[i].materialBuff = CreateBufferResource(sizeof(Material));
			// マテリアルバッファのマッピング
			ret_mesh->mappedResource[i].materialBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->mappedResource[i].materialMap));
		}
	}

	return ret_mesh;

}
std::unique_ptr<Mesh> DxCommon::CreateMeshForSphere(UINT kMaxInstance_)
{
	std::unique_ptr<Mesh> ret_mesh = std::make_unique<Mesh>();

	if (kMaxInstance_ > 0) {
		// 頂点データのサイズ
		UINT sizeOfVertexBuffer = static_cast<UINT>(sizeof(VertexData) * primitiveMeshData.kNumVertexPerSphere);
		// 頂点バッファ生成
		ret_mesh->vertexBuff = CreateBufferResource(sizeOfVertexBuffer);

		// 頂点バッファビューの作成
		ret_mesh->vertexBufferView.BufferLocation = ret_mesh->vertexBuff->GetGPUVirtualAddress();
		ret_mesh->vertexBufferView.SizeInBytes = sizeOfVertexBuffer;
		ret_mesh->vertexBufferView.StrideInBytes = sizeof(VertexData);

		// 頂点バッファのマッピング
		HRESULT result = ret_mesh->vertexBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->vertexMap));
		assert(SUCCEEDED(result));
	}

	if (kMaxInstance_ > 0) {
		// インデックスデータのサイズ
		UINT sizeOfIndexBuffer = static_cast<UINT>(sizeof(uint32_t) * primitiveMeshData.kNumIndexPerSphere);
		// インデックスバッファ生成
		ret_mesh->indexBuff = CreateBufferResource(sizeOfIndexBuffer);

		// インデックスバッファビューの作成
		ret_mesh->indexBufferView.BufferLocation = ret_mesh->indexBuff->GetGPUVirtualAddress();
		ret_mesh->indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		ret_mesh->indexBufferView.SizeInBytes = sizeOfIndexBuffer;

		// インデックスバッファのマッピング
		HRESULT result = ret_mesh->indexBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->indexMap));
		assert(SUCCEEDED(result));
	}

	//頂点データを転送する
	float const kPi = 3.141592653589f;
	float const delta_t = kPi  / primitiveMeshData.kSubdivision;
	float const delta_p = kPi * 2.0f / primitiveMeshData.kSubdivision;
	Vector2 adjust = { 0.5f,0.5f };


	//緯度の方向に分割(-0.5Pi <= cur_t <= 0.5Pi)
	for (uint8_t latIndex = 0; latIndex < primitiveMeshData.kSubdivision; ++latIndex)
	{
		//currentLat
		float cur_t = -Torima::kPi * 0.5f + delta_t * (float)latIndex;

		//経度の方向に分割(0.0 <= cur_p <= 2Pi)
		for (uint8_t lonIndex = 0; lonIndex < primitiveMeshData.kSubdivision; ++lonIndex)
		{
			//currentLon
			float cur_p = delta_p * (float)lonIndex;
			//開始頂点
			uint32_t startIndex = (latIndex * (int)primitiveMeshData.kSubdivision + lonIndex) * 4;

			//A
			ret_mesh->vertexMap[startIndex].position =
			{
				cosf(cur_t) * cosf(cur_p),
				sinf(cur_t),
				cosf(cur_t) * sinf(cur_p)
			};
			ret_mesh->vertexMap[startIndex].texcoord =
			{
				float((float)lonIndex / primitiveMeshData.kSubdivision),
				1.0f - float((float)latIndex / primitiveMeshData.kSubdivision)
			};


			//B
			ret_mesh->vertexMap[startIndex + 1].position =
			{
				cosf(cur_t + delta_t) * cosf(cur_p),
				sinf(cur_t + delta_t),
				cosf(cur_t + delta_t) * sinf(cur_p)
			};
			ret_mesh->vertexMap[startIndex + 1].texcoord =
			{
				float((float)lonIndex / primitiveMeshData.kSubdivision),
				1.0f - float((float)(latIndex + 1) / primitiveMeshData.kSubdivision)
			};

			//C
			ret_mesh->vertexMap[startIndex + 2].position =
			{
				cosf(cur_t) * cosf(cur_p + delta_p) ,
				sinf(cur_t) ,
				cosf(cur_t) * sinf(cur_p + delta_p) 
			};
			ret_mesh->vertexMap[startIndex + 2].texcoord =
			{
				float((float)(lonIndex + 1) / primitiveMeshData.kSubdivision),
				1.0f - float((float)latIndex / primitiveMeshData.kSubdivision)
			};

			//D
			ret_mesh->vertexMap[startIndex + 3].position =
			{
				cosf(cur_t + delta_t) * cosf(cur_p + delta_p) ,
				sinf(cur_t + delta_t) ,
				cosf(cur_t + delta_t) * sinf(cur_p + delta_p) 
			};
			ret_mesh->vertexMap[startIndex + 3].texcoord =
			{
				float((float)(lonIndex + 1) / primitiveMeshData.kSubdivision)  ,
					1.0f - float((float)(latIndex + 1) / primitiveMeshData.kSubdivision ) 
			};

			for (int i = 0; i < 4; ++i)
			{
				ret_mesh->vertexMap[startIndex + i].texcoord.u -= adjust.u;
				ret_mesh->vertexMap[startIndex + i].texcoord.v -= adjust.v;

				ret_mesh->vertexMap[startIndex + i].normal.x = ret_mesh->vertexMap[startIndex + i].position.x;
				ret_mesh->vertexMap[startIndex + i].normal.y = ret_mesh->vertexMap[startIndex + i].position.y;
				ret_mesh->vertexMap[startIndex + i].normal.z = ret_mesh->vertexMap[startIndex + i].position.z;
			}
		}

	}

	//インデックスデータ転送
	int index_vertex = 0;
	int index_index = 0;
	for (int i = 0; i < primitiveMeshData.kSubdivision * primitiveMeshData.kSubdivision; ++i,index_vertex += 4, index_index += 6)
	{
		ret_mesh->indexMap[index_index] = index_vertex;			ret_mesh->indexMap[index_index + 1] = index_vertex + 1;
		ret_mesh->indexMap[index_index + 2] = index_vertex + 2; ret_mesh->indexMap[index_index + 3] = index_vertex + 2;
		ret_mesh->indexMap[index_index + 4] = index_vertex + 1; ret_mesh->indexMap[index_index + 5] = index_vertex + 3;
	}
	


	if (kMaxInstance_ > 0)
	{
		ret_mesh->mappedResource.resize(kMaxInstance_);

		for (UINT i = 0; i < kMaxInstance_; ++i)
		{
			// 行列バッファ生成
			ret_mesh->mappedResource[i].transformationMatrixBuff = CreateBufferResource(sizeof(TransformationMatrix));
			// 行列バッファのマッピング
			ret_mesh->mappedResource[i].transformationMatrixBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->mappedResource[i].transformationMatrixMap));
			// マテリアルバッファ作成
			ret_mesh->mappedResource[i].materialBuff = CreateBufferResource(sizeof(Material));
			// マテリアルバッファのマッピング
			ret_mesh->mappedResource[i].materialBuff->Map(0, nullptr, reinterpret_cast<void**>(&ret_mesh->mappedResource[i].materialMap));
		}
	}

	return ret_mesh;

}

uint16_t DxCommon::CreateOS_Set()
{
	assert(SRV_Handle::numHandle < kMaxTexture);

	uint16_t ret_intHandle = SRV_Handle::numHandle;

	//Textureを読み込む
	offScreenSet.emplace_back(GetOS_TextureData(device.Get()));

	//SRV上の番地
	offScreenSet[OSResourceData::NumOSResource].textureHandle =
	{
		GetCPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV,SRV_Handle::numHandle + 1),
		GetGPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV,SRV_Handle::numHandle + 1)
	};

	//やり方汚いけど
	auto& unchi = textureResourceDataList.emplace_back();
	unchi.textureHandle.textureSrvHandleGPU = offScreenSet[OSResourceData::NumOSResource].textureHandle.textureSrvHandleGPU;
	TextureResourceData::NumTextures++;


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc = CreateSRVDesc(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);

	//DXGI_FORMAT_B8G8R8A8_UNORM_SRGB

	//SRVの作成
	device->CreateShaderResourceView(
		offScreenSet[OSResourceData::NumOSResource].textureResource.Get(),
		&(srvDesc),
		offScreenSet[OSResourceData::NumOSResource].textureHandle.textureSrvHandleCPU);

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//出力結果をSRGBに変換する
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
	//2Dテクスチャとして書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	offScreenSet[OSResourceData::NumOSResource].osRenderTargetHandle.ptr = 
		rtvHandles[1].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	device->CreateRenderTargetView(offScreenSet[OSResourceData::NumOSResource].textureResource.Get(),
		&rtvDesc, offScreenSet[OSResourceData::NumOSResource].osRenderTargetHandle);

	//heap上のハンドルをインクリメント
	OSResourceData::NumOSResource++;
	SRV_Handle::numHandle++;

	return ret_intHandle;
}


UINT DxCommon::LoadTextureFile(std::string filePath_)
{
	assert(SRV_Handle::numHandle < kMaxTexture);

	UINT ret_intHandle = SRV_Handle::numHandle;

	//Textureを読み込む
	textureResourceDataList.emplace_back(LoadTextureData(filePath_, device.Get()));
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	//データを転送する
	textureResourceDataList[TextureResourceData::NumTextures].intermediateResource =
		UploadtextureData(textureResourceDataList[TextureResourceData::NumTextures].textureResource,
			textureResourceDataList[TextureResourceData::NumTextures].mipImages);
	textureResourceDataList[TextureResourceData::NumTextures].intermediateResource->SetName(L"INTER");

	//テクスチャハンドル
	textureResourceDataList[TextureResourceData::NumTextures].textureHandle =
	{
		GetCPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV,SRV_Handle::numHandle + 1),
		GetGPUDescriptorHandle(srvDescriptorHeap.Get(), descriptorSizeSRV,SRV_Handle::numHandle + 1)
	};

	//metaDataをもとにShaderReaourceViewDescの追加
	srvDesc = CreateSRVDesc(textureResourceDataList[TextureResourceData::NumTextures].metaData.format,
		textureResourceDataList[TextureResourceData::NumTextures].metaData.mipLevels);

	//SRVの作成
	device->CreateShaderResourceView(
		textureResourceDataList[TextureResourceData::NumTextures].textureResource.Get(),
		&(srvDesc),
		textureResourceDataList[TextureResourceData::NumTextures].textureHandle.textureSrvHandleCPU);

	//heap上のハンドルをインクリメント
	TextureResourceData::NumTextures++;
	SRV_Handle::numHandle++;

	return ret_intHandle;
}
//===================↑初期化↑==========================



//===================↓更新処理↓==========================

void DxCommon::SetBarrierState(D3D12_RESOURCE_BARRIER& dst_barrier_, D3D12_RESOURCE_STATES before_, D3D12_RESOURCE_STATES after_)
{
	dst_barrier_.Transition.StateBefore = before_;
	dst_barrier_.Transition.StateAfter = after_;
}
void DxCommon::PrepareForNextCommandList()
{
	HRESULT hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList->Reset(commandAllocator.Get(), nullptr);;
	assert(SUCCEEDED(hr));
}
void DxCommon::WaitFenceEvent()
{
	//Fenceの値を更新
	fenceValue++;
	//GPUがここまでたどり着いたとき、
	//fenceの値を指定した値に代入するようにSignalを送る
	commandQueue->Signal(fence.Get(), fenceValue);
	//fenceの値が指定したSignal値にたどり着いているか確認する
	//GetCompletedValueの初期値はfence作成時に渡した初期値
	if (fence->GetCompletedValue() < fenceValue)
	{
		//指定したSignalにたどり着いていないので、たどり着くまで待つ
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		//イベント待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}
void DxCommon::SetDXMatrix(D3D12_VIEWPORT& dst_viewport_, D3D12_RECT& dst_scissorRect_)
{
	//クライアント領域と一緒のサイズにして画面全体に表示
	dst_viewport_.Width = static_cast<FLOAT>(clientWidth);
	dst_viewport_.Height = static_cast<FLOAT>(clientHeight);
	dst_viewport_.TopLeftX = 0;
	dst_viewport_.TopLeftY = 0;
	dst_viewport_.MinDepth = 0.0f;
	dst_viewport_.MaxDepth = 1.0f;
	//しざー矩形
	dst_scissorRect_.right = static_cast<LONG>(clientWidth);
	dst_scissorRect_.bottom = static_cast<LONG>(clientHeight);
	dst_scissorRect_.left = 0;
	dst_scissorRect_.top = 0;
}

void DxCommon::SetCameraPos(Vector3 cameraWorld_)
{
	commonVariablesData->camera_world = cameraWorld_;
}

void DxCommon::BeginFrame(BYTE* key, DIMOUSESTATE* mouseState_)
{
	time += deltaTime;
	commonVariablesData->time += deltaTime;

	//キーボード情報の取得
	inputs.keyboard->Acquire();
	inputs.keyboard->GetDeviceState(sizeof(*key) * 256, key);
	//マウスの情報の取得
	inputs.mouse->Acquire();
	inputs.mouse->GetDeviceState(sizeof(*mouseState_), mouseState_);

	//Imguiにここからフレームが始まる旨を告げる
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//描画先のRTV、DSVを設定する
	dsvHandle = dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_RESOURCE_BARRIER barrier{};

	//これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();
	//バリアの設定
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファーに対して行う
	barrier.Transition.pResource = swapChainResources[backBufferIndex].Get();
	//バリアステートを遷移
	SetBarrierState(barrier, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

	commandList->OMSetRenderTargets(1, &rtvHandles[backBufferIndex], false, &dsvHandle);
	//指定した深度で画面クリアする
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	//指定した色で画面全体をクリアする
	commandList->ClearRenderTargetView(rtvHandles[backBufferIndex], windowColor, 0, nullptr);
	////描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap.Get() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps);
	//DXの行列の設定
	D3D12_VIEWPORT viewPort{};
	D3D12_RECT scissorRect{};
	SetDXMatrix(viewPort, scissorRect);
	commandList->RSSetViewports(1, &viewPort);
	commandList->RSSetScissorRects(1, &scissorRect);
}
void DxCommon::EndFrame()
{
	//DrawIndexのリセット
	DrawIndexReset();

	ImGui::Render();
	//実際のcommandListのImguiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());

	//これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();
	D3D12_RESOURCE_BARRIER barrier{};
	//バリアの設定
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファーに対して行う
	barrier.Transition.pResource = swapChainResources[backBufferIndex].Get();

	//画面に書く処理が終わり、画面に映すので状態を遷移
	//RenderTarget->Prsent
	SetBarrierState(barrier, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

	//コマンドリストの内容を確定させる
	HRESULT hr = commandList->Close();
	assert(SUCCEEDED(hr));

	//GPUにコマンドリストの実行を行わさせる
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, commandLists);
	//GPUとOSに画面の交換を行うように通知する
	swapChain->Present(1, 0);

	//イベントを待つ
	WaitFenceEvent();
	//次のフレーム用のコマンドリストを準備
	PrepareForNextCommandList();

	fpsController.TimeAdjust();
	deltaTime = 1.0f / ImGui::GetIO().Framerate;

}
void DxCommon::InputDirectionalLightData(Vector3 worldPos_, Vector4<float> color_, float intensity_)
{
	directionalLightData->color = { color_.x * inv_255,color_.y * inv_255, color_.z * inv_255, color_.w * inv_255 };
	directionalLightData->direction = worldPos_.GetNormalized() * -1.0f;
	directionalLightData->intensity = intensity_;
}

void DxCommon::DrawTriangle(VertexData left_, VertexData top_, VertexData right_, 
	Vector4<float> color_, int texHandle_,TransformationMatrix tMat_,DrawMode drawMode_, Matrix4 uvMat_, int enableLighting_)
{
	assert(primitiveMeshData.indexTriangle < primitiveMeshData.kMaxTriangle);

	if (drawMode_ == kSolid)
	{
		//使用する三角形のマップインデックス
		uint32_t usingVertex_index = primitiveMeshData.indexTriangle * primitiveMeshData.kNumVertexPerTriangle;

		VertexData vData[3] =
		{
			left_,top_,right_
		};

		Vector4 <float> color = { color_.x * inv_255,color_.y * inv_255 ,color_.z * inv_255 ,color_.w * inv_255 };

		//< データの転送 >
		//[ 頂点 ]
		std::memcpy(&primitiveMeshData.triangleMesh->vertexMap[usingVertex_index], vData, sizeof(VertexData) * primitiveMeshData.kNumVertexPerTriangle);
		//[ 行列 ]
		primitiveMeshData.triangleMesh->mappedResource[primitiveMeshData.indexTriangle].transformationMatrixMap->World = tMat_.World;
		primitiveMeshData.triangleMesh->mappedResource[primitiveMeshData.indexTriangle].transformationMatrixMap->WVP = tMat_.WVP;
		//[ マテリアル ]
		//色
		primitiveMeshData.triangleMesh->mappedResource[primitiveMeshData.indexTriangle].materialMap->color = color;
		primitiveMeshData.triangleMesh->mappedResource[primitiveMeshData.indexTriangle].materialMap->enableLighting = enableLighting_;

		//UVTransitionは後回し
		primitiveMeshData.triangleMesh->mappedResource[primitiveMeshData.indexTriangle].materialMap->uvTransform = uvMat_;

		//< 描画 >
		//RootSignatureを設定、PSOに設定しているけど別途必要
		commandList->SetGraphicsRootSignature(pipelineSetTriangles[pipelineComponents.cur_blendMode]
			[(int)pipelineComponents.cur_cullMode][pipelineComponents.cur_shaderType]->rootSignature.Get());
		commandList->SetPipelineState(pipelineSetTriangles[pipelineComponents.cur_blendMode]
			[(int)pipelineComponents.cur_cullMode][pipelineComponents.cur_shaderType]->pipelineStateObject.Get());
		//VBV
		commandList->IASetVertexBuffers(0, 1, &primitiveMeshData.triangleMesh->vertexBufferView);
		//形状の設定。PSOに設定しているものと同じものを選択
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//マテリアルのCバッファの場所を指定
		commandList->SetGraphicsRootConstantBufferView(0, primitiveMeshData.triangleMesh->mappedResource[primitiveMeshData.indexTriangle].materialBuff->GetGPUVirtualAddress());
		//transformationMatrixResource用Cバッファの場所を指定
		commandList->SetGraphicsRootConstantBufferView(1, primitiveMeshData.triangleMesh->mappedResource[primitiveMeshData.indexTriangle].transformationMatrixBuff->GetGPUVirtualAddress());
		//SRVのDescriptortableの先頭を設定。2はrootparameter[2]である
		commandList->SetGraphicsRootDescriptorTable(2, textureResourceDataList[texHandle_].textureHandle.textureSrvHandleGPU);
		//DirectionalLight用のバッファの場所を指定
		commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
		//CommonVariableのバッファの場所を指定
		commandList->SetGraphicsRootConstantBufferView(4, commonVariablesResource->GetGPUVirtualAddress());

		//描画(DrawCall)。3インデックスで一つのインスタンス
		commandList->DrawInstanced(primitiveMeshData.kNumVertexPerTriangle, 1, static_cast<UINT>(usingVertex_index), 0);
		//次の三角形描画用にインクリメント
		primitiveMeshData.indexTriangle++;
	}

	else if (drawMode_ == kWireFrame)
	{
		DrawLine(left_.position, top_.position, texHandle_, color_, tMat_,uvMat_);
		DrawLine(top_.position, right_.position, texHandle_,color_, tMat_, uvMat_);
		DrawLine(right_.position, left_.position, texHandle_,color_, tMat_, uvMat_);

	}
}
void DxCommon::DrawQuad(VertexData leftTop_, VertexData rightTop_, VertexData leftBottom_, VertexData rightBottom_, 
	int texHandle_, Vector4<float> color_, TransformationMatrix tMat_, int enableLighting_, DrawMode drawMode_ , Matrix4 uvMat_)
{
	assert(primitiveMeshData.indexQuad < primitiveMeshData.kMaxQuad);

	if (drawMode_ == kSolid)
	{
		//使用する三角形のマップインデックス
		uint32_t const usingIndex_index = primitiveMeshData.indexQuad * primitiveMeshData.kNumIndexPerQuad;
		uint32_t const usingVertex_index = primitiveMeshData.indexQuad * primitiveMeshData.kNumVertexPerQuad;

		VertexData vData[4] =
		{
			leftBottom_,leftTop_,rightBottom_,rightTop_
		};

		uint32_t indices[6] = { 0,1,2,2,1,3 };
		Vector4 <float> color = { color_.x * inv_255,color_.y * inv_255 ,color_.z * inv_255 ,color_.w * inv_255 };

		//< データの転送 >
		//[ 頂点 ]
		std::memcpy(&primitiveMeshData.quadMesh->vertexMap[usingVertex_index], vData, sizeof(VertexData) * primitiveMeshData.kNumVertexPerQuad);
		//[ インデックス ]
		std::memcpy(&primitiveMeshData.quadMesh->indexMap[usingIndex_index], indices, sizeof(uint32_t) * primitiveMeshData.kNumIndexPerQuad);
		//[ 行列 ]
		primitiveMeshData.quadMesh->mappedResource[primitiveMeshData.indexQuad].transformationMatrixMap->World = tMat_.World;
		primitiveMeshData.quadMesh->mappedResource[primitiveMeshData.indexQuad].transformationMatrixMap->WVP = tMat_.WVP;
		//[ マテリアル ]
		//色
		primitiveMeshData.quadMesh->mappedResource[primitiveMeshData.indexQuad].materialMap->color = color;
		primitiveMeshData.quadMesh->mappedResource[primitiveMeshData.indexQuad].materialMap->enableLighting = enableLighting_;
		//UVTransitionは後回し
		primitiveMeshData.quadMesh->mappedResource[primitiveMeshData.indexQuad].materialMap->uvTransform = uvMat_;
		//primitiveMeshData.quadMesh->mappedResource[primitiveMeshData.indexQuad].materialMap->shininess = 


		//< 描画 >
		//RootSignatureを設定、PSOに設定しているけど別途必要
		commandList->SetGraphicsRootSignature(pipelineSetTriangles[pipelineComponents.cur_blendMode]
			[(int)pipelineComponents.cur_cullMode][pipelineComponents.cur_shaderType]->rootSignature.Get());
		commandList->SetPipelineState(pipelineSetTriangles[pipelineComponents.cur_blendMode]
			[(int)pipelineComponents.cur_cullMode][pipelineComponents.cur_shaderType]->pipelineStateObject.Get());
		//VBV
		commandList->IASetVertexBuffers(0, 1, &primitiveMeshData.quadMesh->vertexBufferView);
		//IBV
		commandList->IASetIndexBuffer(&primitiveMeshData.quadMesh->indexBufferView);
		//形状の設定。PSOに設定しているものと同じものを選択
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		//マテリアルのCバッファの場所を指定
		commandList->SetGraphicsRootConstantBufferView(0, primitiveMeshData.quadMesh->mappedResource[primitiveMeshData.indexQuad].materialBuff->GetGPUVirtualAddress());
		//transformationMatrixResource用Cバッファの場所を指定
		commandList->SetGraphicsRootConstantBufferView(1, primitiveMeshData.quadMesh->mappedResource[primitiveMeshData.indexQuad].transformationMatrixBuff->GetGPUVirtualAddress());
		
		//SRVのDescriptortableの先頭を設定。2はrootparameter[2]である//
		commandList->SetGraphicsRootDescriptorTable(2, textureResourceDataList[texHandle_].textureHandle.textureSrvHandleGPU);
		//DirectionalLight用のバッファの場所を指定
		commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
		//CommonVariableのバッファの場所を指定
		commandList->SetGraphicsRootConstantBufferView(4, commonVariablesResource->GetGPUVirtualAddress());

		//描画(DrawCall)。6インデックスで一つのインスタンス
		commandList->DrawIndexedInstanced(primitiveMeshData.kNumIndexPerQuad, 1, static_cast<UINT>(usingIndex_index), static_cast<UINT>(usingVertex_index), 0);

		//次の描画用にインクリメント
		primitiveMeshData.indexQuad++;
	}

	else if (drawMode_ == kWireFrame)
	{
		DrawLine(leftTop_.position, rightTop_.position, texHandle_, color_, tMat_, uvMat_);
		DrawLine(rightTop_.position, rightBottom_.position, texHandle_, color_, tMat_, uvMat_);
		DrawLine(rightBottom_.position, leftBottom_.position, texHandle_,color_, tMat_, uvMat_);
		DrawLine(leftBottom_.position, leftTop_.position, texHandle_,color_, tMat_, uvMat_);

	}
}
void DxCommon::DrawLine(Vector3 start_, Vector3 end_, int texHandle_,Vector4<float> color_, TransformationMatrix tMat_, Matrix4 uvMat_)
{
	assert(primitiveMeshData.indexLine < primitiveMeshData.kMaxLine);

	//使用するマップインデックス
	uint32_t usingVertex_index = primitiveMeshData.indexLine * primitiveMeshData.kNumVertexPerLine;

	Vector3 vData[2] =
	{
		start_,end_
	};

	Vector4 <float> color = { color_.x * inv_255,color_.y * inv_255 ,color_.z * inv_255 ,color_.w * inv_255 };

	//< データの転送 >
	//[ 頂点 ]
	std::memcpy(&primitiveMeshData.lineMesh->vertexMapForLine[usingVertex_index], vData, sizeof(Vector3) * primitiveMeshData.kNumVertexPerLine);
	//[ 行列 ]
	primitiveMeshData.lineMesh->mappedResource[primitiveMeshData.indexLine].transformationMatrixMap->World = tMat_.World;
	primitiveMeshData.lineMesh->mappedResource[primitiveMeshData.indexLine].transformationMatrixMap->WVP = tMat_.WVP;
	//[ マテリアル ]
	//色
	primitiveMeshData.lineMesh->mappedResource[primitiveMeshData.indexLine].materialMap->color = color;
	primitiveMeshData.lineMesh->mappedResource[primitiveMeshData.indexLine].materialMap->enableLighting = false;
	//UVTransitionは後回し
	primitiveMeshData.lineMesh->mappedResource[primitiveMeshData.indexLine].materialMap->uvTransform = uvMat_;

	//< 描画 >
	//RootSignatureを設定、PSOに設定しているけど別途必要
	commandList->SetGraphicsRootSignature(pipelineSetLines[pipelineComponents.cur_blendMode]
		[(int)pipelineComponents.cur_cullMode][pipelineComponents.cur_shaderType]->rootSignature.Get());
	commandList->SetPipelineState(pipelineSetLines[pipelineComponents.cur_blendMode]
		[(int)pipelineComponents.cur_cullMode][pipelineComponents.cur_shaderType]->pipelineStateObject.Get());
	//VBV
	commandList->IASetVertexBuffers(0, 1, &primitiveMeshData.lineMesh->vertexBufferView);
	//形状の設定。PSOに設定しているものと同じものを選択
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST); 
	//マテリアルのCバッファの場所を指定
	commandList->SetGraphicsRootConstantBufferView(0, primitiveMeshData.lineMesh->mappedResource[primitiveMeshData.indexLine].materialBuff->GetGPUVirtualAddress());
	//transformationMatrixResource用Cバッファの場所を指定
	commandList->SetGraphicsRootConstantBufferView(1, primitiveMeshData.lineMesh->mappedResource[primitiveMeshData.indexLine].transformationMatrixBuff->GetGPUVirtualAddress());
	////SRVのDescriptortableの先頭を設定。2はrootparameter[2]である
	commandList->SetGraphicsRootDescriptorTable(2, textureResourceDataList[texHandle_].textureHandle.textureSrvHandleGPU);
	//DirectionalLight用のバッファの場所を指定
	commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	//CommonVariableのバッファの場所を指定
	commandList->SetGraphicsRootConstantBufferView(4, commonVariablesResource->GetGPUVirtualAddress());


	//描画(DrawCall)。
	commandList->DrawInstanced(primitiveMeshData.kNumVertexPerLine, 1, static_cast<UINT>(usingVertex_index), 0);
	//次の描画用にインクリメント
	primitiveMeshData.indexLine++;
}
void DxCommon::DrawSphere(Vector4<float> color_, TransformationMatrix tMat_, int texHandle_, DrawMode drawMode_, Matrix4 uvMat_,int enableLightning_)
{
	assert(primitiveMeshData.indexSphere < primitiveMeshData.kMaxSphere);

	if (drawMode_ == kSolid)
	{
		Vector4 <float> color = { color_.x * inv_255,color_.y * inv_255 ,color_.z * inv_255 ,color_.w * inv_255 };

		//< データの転送 >
		//[ 行列 ]
		primitiveMeshData.sphereMesh->mappedResource[primitiveMeshData.indexSphere].transformationMatrixMap->World = tMat_.World;
		primitiveMeshData.sphereMesh->mappedResource[primitiveMeshData.indexSphere].transformationMatrixMap->WVP = tMat_.WVP;
		//[ マテリアル ]
		//色
		primitiveMeshData.sphereMesh->mappedResource[primitiveMeshData.indexSphere].materialMap->color = color;
		primitiveMeshData.sphereMesh->mappedResource[primitiveMeshData.indexSphere].materialMap->enableLighting = enableLightning_;

		//UVTransitionは後回し
		primitiveMeshData.sphereMesh->mappedResource[primitiveMeshData.indexSphere].materialMap->uvTransform = uvMat_;


		//< 描画 >
		//RootSignatureを設定、PSOに設定しているけど別途必要
		commandList->SetGraphicsRootSignature(pipelineSetTriangles[pipelineComponents.cur_blendMode]
			[(int)pipelineComponents.cur_cullMode][pipelineComponents.cur_shaderType]->rootSignature.Get());
		commandList->SetPipelineState(pipelineSetTriangles[pipelineComponents.cur_blendMode]
			[(int)pipelineComponents.cur_cullMode][pipelineComponents.cur_shaderType]->pipelineStateObject.Get());

		//IBV
		commandList->IASetIndexBuffer(&primitiveMeshData.sphereMesh->indexBufferView);
		//VBV
		commandList->IASetVertexBuffers(0, 1, &primitiveMeshData.sphereMesh->vertexBufferView);
		//形状の設定。PSOに設定しているものと同じものを選択
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//マテリアルのCバッファの場所を指定
		commandList->SetGraphicsRootConstantBufferView(0, primitiveMeshData.sphereMesh->mappedResource[primitiveMeshData.indexSphere].materialBuff->GetGPUVirtualAddress());
		//transformationMatrixResource用Cバッファの場所を指定
		commandList->SetGraphicsRootConstantBufferView(1, primitiveMeshData.sphereMesh->mappedResource[primitiveMeshData.indexSphere].transformationMatrixBuff->GetGPUVirtualAddress());
		//SRVのDescriptortableの先頭を設定。2はrootparameter[2]である
		commandList->SetGraphicsRootDescriptorTable(2, textureResourceDataList[texHandle_].textureHandle.textureSrvHandleGPU);
		//DirectionalLight用のバッファの場所を指定
		commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
		//CommonVariableのバッファの場所を指定
		commandList->SetGraphicsRootConstantBufferView(4, commonVariablesResource->GetGPUVirtualAddress());

		//描画(DrawCall)。
		commandList->DrawIndexedInstanced(primitiveMeshData.kNumIndexPerSphere, 1, 0, 0, 0);
		//次の描画用にインクリメント
		primitiveMeshData.indexSphere++;
	}

	else if (drawMode_ == kWireFrame)
	{
		//頂点データを転送する
		float const kPi = 3.141592653589f;
		float const delta_t = kPi / primitiveMeshData.kSubdivision;
		float const delta_p = kPi * 2.0f / primitiveMeshData.kSubdivision;

		//緯度の方向に分割(-0.5Pi <= cur_t <= 0.5Pi)
		for (uint8_t latIndex = 0; latIndex < primitiveMeshData.kSubdivision; ++latIndex)
		{
			//currentLat
			float cur_t = -Torima::kPi * 0.5f + delta_t * (float)latIndex;

			//経度の方向に分割(0.0 <= cur_p <= 2Pi)
			for (uint8_t lonIndex = 0; lonIndex < primitiveMeshData.kSubdivision; ++lonIndex)
			{
				//currentLon
				float cur_p = delta_p * (float)lonIndex;
				//開始頂点
				uint32_t startIndex = (latIndex * (int)primitiveMeshData.kSubdivision + lonIndex) * 4;

				//A
				Vector3 pointA =
				{
					cosf(cur_t) * cosf(cur_p),
					sinf(cur_t),
					cosf(cur_t) * sinf(cur_p)
				};


				//B
				Vector3 pointB =
				{
					cosf(cur_t + delta_t) * cosf(cur_p),
					sinf(cur_t + delta_t),
					cosf(cur_t + delta_t) * sinf(cur_p),
				};

				//C
				Vector3 pointC =
				{
					cosf(cur_t) * cosf(cur_p + delta_p) ,
					sinf(cur_t) ,
					cosf(cur_t) * sinf(cur_p + delta_p) ,
				};

				//D
				Vector3 pointD =
				{
					cosf(cur_t + delta_t) * cosf(cur_p + delta_p) ,
					sinf(cur_t + delta_t) ,
					cosf(cur_t + delta_t) * sinf(cur_p + delta_p) ,
				};

				DrawLine(pointA, pointB, texHandle_, color_, tMat_, uvMat_);
				DrawLine(pointB, pointD, texHandle_, color_, tMat_, uvMat_);
				DrawLine(pointD, pointC, texHandle_, color_, tMat_, uvMat_);
				DrawLine(pointC, pointA, texHandle_, color_, tMat_, uvMat_);

			}

		}
	}

}
void DxCommon::DrawCube(VertexData* leftTop_, VertexData* rightTop_, VertexData* leftBottom_, VertexData* rightBottom_,
	int* texHandle_, TransformationMatrix* tMat_, Vector4<float> color_, bool enableLighting_,  DrawMode drawMode_, Matrix4* uvMat_)
{
	if (drawMode_ == kSolid)
	{
		for (int i = 0; i < 6; ++i)
		{
			DrawQuad(leftTop_[i], rightTop_[i], leftBottom_[i], rightBottom_[i],
				texHandle_[i], color_, tMat_[i], enableLighting_, drawMode_, uvMat_[i]);
		}
	}

	else if (drawMode_ == kWireFrame)
	{
		for (int i = 0; i < 6; ++i)
		{
			DrawLine(leftTop_[i].position, rightTop_[i].position, texHandle_[i], color_, tMat_[i], uvMat_[i]);
			DrawLine(rightTop_[i].position, rightBottom_[i].position, texHandle_[i], color_, tMat_[i], uvMat_[i]);
			DrawLine(rightBottom_[i].position, leftBottom_[i].position, texHandle_[i], color_, tMat_[i], uvMat_[i]);
			DrawLine(leftBottom_[i].position, leftTop_[i].position, texHandle_[i], color_, tMat_[i], uvMat_[i]);
		}

	}
}
void DxCommon::DrawModel(Mesh* mesh_, UINT verticesSIze_, int texHandle_, Vector4<float> color_, TransformationMatrix tMat_, int enableLighting_, Matrix4 uvMat_)
{
	Vector4 <float> color = { color_.x * inv_255,color_.y * inv_255 ,color_.z * inv_255 ,color_.w * inv_255 };

	//[ 行列 ]
	mesh_->mappedResource[0].transformationMatrixMap->World = tMat_.World;
	mesh_->mappedResource[0].transformationMatrixMap->WVP = tMat_.WVP;
	//[ マテリアル ]
	//色
	mesh_->mappedResource[0].materialMap->color = color;
	mesh_->mappedResource[0].materialMap->enableLighting = enableLighting_;
	mesh_->mappedResource[0].materialMap->uvTransform = uvMat_;

	//RootSignatureを設定、PSOに設定しているけど別途必要
	commandList->SetGraphicsRootSignature(pipelineSetTriangles[pipelineComponents.cur_blendMode]
		[(int)pipelineComponents.cur_cullMode][pipelineComponents.cur_shaderType]->rootSignature.Get());
	commandList->SetPipelineState(pipelineSetTriangles[pipelineComponents.cur_blendMode]
		[(int)pipelineComponents.cur_cullMode][pipelineComponents.cur_shaderType]->pipelineStateObject.Get());

	//VBV
	commandList->IASetVertexBuffers(0, 1, &mesh_->vertexBufferView);
	//形状の設定。PSOに設定しているものと同じものを選択
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルのCバッファの場所を指定
	commandList->SetGraphicsRootConstantBufferView(0, mesh_->mappedResource[0].materialBuff->GetGPUVirtualAddress());
	//transformationMatrixResource用Cバッファの場所を指定
	commandList->SetGraphicsRootConstantBufferView(1, mesh_->mappedResource[0].transformationMatrixBuff->GetGPUVirtualAddress());
	//SRVのDescriptortableの先頭を設定。2はrootparameter[2]である
	commandList->SetGraphicsRootDescriptorTable(2, textureResourceDataList[texHandle_].textureHandle.textureSrvHandleGPU);
	//DirectionalLight用のバッファの場所を指定
	commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	//CommonVariableのバッファの場所を指定
	commandList->SetGraphicsRootConstantBufferView(4, commonVariablesResource->GetGPUVirtualAddress());

	//描画(DrawCall)。
	commandList->DrawInstanced(verticesSIze_, 1, 0, 0);

}
//4明太描画
void DxCommon::DrawTetrahedra(VertexData* left_, VertexData* top_, VertexData* right_,
	Vector4<float> color_, int texHandle_, TransformationMatrix* tMat_, DrawMode drawMode_, Matrix4* uvMat_)
{
	if (drawMode_ == kSolid)
	{
		for (int i = 0; i < 4; ++i)
		{
			DrawTriangle(left_[i], top_[i], right_[i], color_, texHandle_, tMat_[i], drawMode_, uvMat_[i],true);
		}
	}

	else if (drawMode_ == kWireFrame)
	{
		for (int i = 0; i < 4; ++i)
		{
			DrawLine(left_[i].position, top_[i].position, texHandle_, color_, tMat_[i], uvMat_[i]);
			DrawLine(top_[i].position, right_[i].position, texHandle_, color_, tMat_[i], uvMat_[i]);
			DrawLine(right_[i].position, left_[i].position, texHandle_, color_, tMat_[i], uvMat_[i]);
		}
	}
}

void DxCommon::SetMousePos(POINT* point_)
{
	GetCursorPos(point_);
}
void DxCommon::SetBlendMode(BlendMode mode_)
{
	pipelineComponents.cur_blendMode = mode_;
}
void DxCommon::SetCullMode(CullMode cullMode_)
{
	pipelineComponents.cur_cullMode = cullMode_;
}
void DxCommon::SetShaderSet(ShaderSetType shaderSetType_)
{
	pipelineComponents.cur_shaderType = shaderSetType_;
}

void DxCommon::DrawIndexReset()
{
	primitiveMeshData.indexTriangle = 0;
	primitiveMeshData.indexQuad = 0;
	primitiveMeshData.indexLine = 0;
	primitiveMeshData.indexSphere = 0;
}

void DxCommon::OffScreecBegin(OSResourceData& osData_)
{
	//描画先のRTV、DSVを設定する
	dsvHandle = dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_RESOURCE_BARRIER barrier{};
	//バリアの設定
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファーに対して行う
	barrier.Transition.pResource = osData_.textureResource.Get();
	//バリアステートを遷移
	SetBarrierState(barrier, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);
	//オフパコリソースをレンダーターゲットとして設定
	commandList->OMSetRenderTargets(1, &osData_.osRenderTargetHandle, false, &dsvHandle);

	//指定した深度で画面クリアする
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	//指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f,0.1f,0.1f,0.5f };
	commandList->ClearRenderTargetView(osData_.osRenderTargetHandle, clearColor, 0, nullptr);
	////描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap.Get() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps);
	//DXの行列の設定
	D3D12_VIEWPORT viewPort{};
	D3D12_RECT scissorRect{};
	SetDXMatrix(viewPort, scissorRect);
	commandList->RSSetViewports(1, &viewPort);
	commandList->RSSetScissorRects(1, &scissorRect);

}

void DxCommon::OffScreenEnd(OSResourceData& osData_)
{
	D3D12_RESOURCE_BARRIER barrier{};

	//ここからENDRENDER
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファーに対して行う
	barrier.Transition.pResource = osData_.textureResource.Get();
	//バリアステートを遷移
	SetBarrierState(barrier, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);
}


//===================↑更新処理↑==========================


void DxCommon::Finalize()
{

	CloseHandle(fenceEvent);

	//[ ImGui ]
	ImGuiFinalize();

	inputs.Finalize();

	//#ifdef _DEBUG
	//debugController->Release();
	//#endif


	CloseWindow(hwnd);

	//COMの終了
	CoUninitialize();

}

void DxCommon::ChangeWindowColor(Vector4<float> col_)
{
	windowColor[0] = col_.x;
	windowColor[1] = col_.y;
	windowColor[2] = col_.z;
	windowColor[3] = col_.w;

}

void DxCommon::ImGuiFinalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}




