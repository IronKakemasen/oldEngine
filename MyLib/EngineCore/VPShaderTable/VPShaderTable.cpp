#include "VPShaderTable.h"


VPShaderTable::VPShaderTable()
{
	SetShaderTable();
}
//シェーダセットのテーブルを設定
void VPShaderTable::SetShaderTable()
{
	table[ShaderSetType::kV0P0] = { L"./Resource/Shader/Object3D.VS.hlsl" ,L"./Resource/Shader/Object3D.PS.hlsl" };
	table[ShaderSetType::kV0PTest] = { L"./Resource/Shader/Object3D.VS.hlsl" ,L"./Resource/Shader/Test.PS.hlsl" };
	table[ShaderSetType::kKirei] = { L"./Resource/Shader/Object3D.VS.hlsl" ,L"./Resource/Shader/Kirei.PS.hlsl" };

}
