#pragma once

#include <string>
#include <map>

enum ShaderSetType
{
	kV0P0,
	kV0PTest,
	kKirei,
	kCountOfShaderSetType
};


struct ShaderSet
{
	std::wstring vsFilePath;
	std::wstring psFlilePath;
};

struct VPShaderTable
{
	std::map<ShaderSetType, ShaderSet> table;
	//シェーダセットのテーブルを設定
	VPShaderTable();

private:
	void SetShaderTable();



};
