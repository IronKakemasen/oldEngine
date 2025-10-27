#pragma once
#include "../EngineCore/DirectXCommon/DxCommon.h"
#include "../EngineCore/LeakChecker/LeakChecker.h"

#include <cassert>
#include <cstring>
#define DIRECTINPUT_VERSION    0x0800// DirectInputのバージョン指定
#include <dinput.h>
#include <Xinput.h> 

inline ResourceLeakChecker* pLeakChecker;
inline DxCommon* pDxCommon;

struct NoviceLike
{	
	inline static BYTE keys[256] = {};
	inline static BYTE preKeys[256] = {};
	inline static DIMOUSESTATE mouseState;
	inline static DIMOUSESTATE preMouseState;
	inline static POINT mousePos;
	inline static POINT preMousePos;

	enum StickType {
		LeftStick,
		RightStick
	};

	// Xboxコントローラーのボタン列挙
	enum PadButton {
		ButtonUp = XINPUT_GAMEPAD_DPAD_UP,
		ButtonDown = XINPUT_GAMEPAD_DPAD_DOWN,
		ButtonLeft = XINPUT_GAMEPAD_DPAD_LEFT,
		ButtonRight = XINPUT_GAMEPAD_DPAD_RIGHT,
		ButtonMenu = XINPUT_GAMEPAD_START,
		ButtonPhoto = XINPUT_GAMEPAD_BACK,
		ButtonLT = XINPUT_GAMEPAD_LEFT_THUMB,
		ButtonRT = XINPUT_GAMEPAD_RIGHT_THUMB,
		ButtonLB = XINPUT_GAMEPAD_LEFT_SHOULDER,
		ButtonRB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		ButtonA = XINPUT_GAMEPAD_A,
		ButtonB = XINPUT_GAMEPAD_B,
		ButtonX = XINPUT_GAMEPAD_X,
		ButtonY = XINPUT_GAMEPAD_Y,
	};
	
	//三角形描画
	static void DrawTriangle(VertexData left_, VertexData top_, VertexData right_,
		Vector4<float> color_, int texHandle_,TransformationMatrix tMat_, DrawMode drawMode_, Matrix4 uvMat_, int enableLighting_);
	//矩形描画
	static void DrawQuad(VertexData leftTop_, VertexData rightTop_, VertexData leftBottom_, VertexData rightBottom_,
		int texHandle_, Vector4<float> color_, TransformationMatrix tMat_, int enableLighting_, DrawMode drawMode_, Matrix4 uvMat_ );
	//線描画
	static void DrawLine(Vector3 start_, Vector3 end_, int texHandle_, Vector4<float> color_, TransformationMatrix tMat_, Matrix4 uvMat_ );
	//単位球体描画
	static void DrawSphere(Vector4<float> color_, TransformationMatrix tMat_, int texHandle_, DrawMode drawMode_, Matrix4 uvMat_, int enableLighting_ = 1);
	//モデル描画
	static void DrawModel(Mesh* mesh_, UINT verticesSIze_, int texHandle_, Vector4<float> color_, TransformationMatrix tMat_, int enableLighting_, Matrix4 uvMat_);

	//画像読み込み
	static uint16_t LoadTextureFile(std::string filePath_);
	//モデル専用メッシュ作成関数
	static Model CreateModel(std::string const& directoryPath_obj_, std::string const& fileName_obj_);
	static std::unique_ptr<Model> CreateModelMusic(std::string const& directoryPath_obj_, std::string const& fileName_obj_);

	//カーソルの座標をセットする
	static void SetMousePos(POINT* point_);
	//ブレンドモードのセット
	static void SetBlendMode(BlendMode mode_);
	//カリングモードのセット
	static void SetCullMode(CullMode cullMode_);
	//ピクセルシェーダのセット
	static void SetShaderSet(ShaderSetType shaderSetType_);
	//キー入力トリガーを検知( 引数「DIK_」)
	static bool IsKeyTrigger(int key_);
	//キー入力プレスを検知( 引数「DIK_」)
	static bool IsKeyPress(int key_);






	//気にしなくていい↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	//四面体描画
	static void DrawTetrahedra(VertexData* left_, VertexData* top_, VertexData* right_,
		Vector4<float> color_, int texHandle_, TransformationMatrix* tMat_, DrawMode drawMode_, Matrix4* uvMat_);
	//軸描画
	static void DrawAxis(Vector3 pos_, Vector3* targetDir_, float coe_, Matrix4 vpMat_);
	//６面体描画
	static void DrawCube(VertexData* leftTop_, VertexData* rightTop_, VertexData* leftBottom_, VertexData* rightBottom_,
		int* texHandle_, TransformationMatrix* tMat_, Vector4<float> color_, int enableLighting_, DrawMode drawMode_, Matrix4* uvMat_);
	//Draw前共通処理
	static void BeginFrame(BYTE* key, DIMOUSESTATE* mouseState_);
	//Draw後共通処理
	static void EndFrame();
	//光情報を書き込む（1つだけ）
	static void InputDirectionalLightData(Vector3 worldPos_, Vector4<float> color_, float intensity_);
	static void Initialize(int32_t kWindowWidth_, int32_t kWindowHeight_, LPCWSTR const windowTitle_);
	static void Finalize();
	static void SetCameraPos(Vector3 cameraWorld_);
	static uint16_t CreateOS_Set();
	static void ChangeWindowColor(Vector4<float> col_);
	//気にしなくていい↑↑↑↑↑↑↑↑↑↑↑↑↑↑

};

