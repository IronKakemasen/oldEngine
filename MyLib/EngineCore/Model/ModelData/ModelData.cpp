#include "ModelData.h"
#include <fstream>
#include <sstream>
#include < assert.h >


std::vector<MaterialData> ModelData::LoadMaterialTemplateFile(std::string const& directoryPath_, std::string const& fileName_)
{
	std::vector<MaterialData> materialData;
	std::string line;
	//ファイルを開く
	std::ifstream file(directoryPath_ + "/" + fileName_);
	assert(file.is_open());

	while (std::getline(file,line))
	{
		std::string identifier;
		std::istringstream s(line);

		//先端の識別子を読む
		s >> identifier;

		//identifierに応じた処理
		if (identifier == "map_Kd")
		{
			std::string textureFileName;
			s >> textureFileName;
			//連結してファイルパスにする
			materialData.emplace_back(directoryPath_ + "/" + textureFileName);	
		}
	}

	return materialData;
}

ModelData ModelData::LoadObjFile(std::string const& directoryPath_, std::string const& fileName_)
{
	//構築するModelData
	ModelData modelData;
	int vertexIndex = -1;

	//位置
	std::vector<Vector3>positions;
	//法線
	std::vector<Vector3>normals;
	//テクスチャ座標
	std::vector<Vector2>texcoords;
	//ファイルから読んだ１行を格納するもの
	std::string line;

	//面ポリゴンが何頂点なのかいったん読み取るため用
	bool onlyOnce = true;
	uint8_t tmp_faceVertex;
	int cc = 0;

	//[ ファイルを開く ]
	std::ifstream file(directoryPath_ + "/" + fileName_);
	assert(file.is_open());

	//[ ファイルを読み込む ]
	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		std::istringstream tmp_s(line);

		//先端の識別子を読む
		s >> identifier;
		tmp_s >> identifier;

		cc++;

		//identifierに応じた処理
		if (identifier == "o")
		{
			onlyOnce = true;
		}
		else if (identifier == "v")
		{
			Vector3 position;
			s >> position.x >> position.y >> position.z;
			positions.push_back(position);
		}

		else if (identifier == "vt")
		{
			Vector2 texcoord;
			s >> texcoord.u >> texcoord.v;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn")
		{
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}

		else if (identifier == "f")
		{
			//いったん面ポリゴンが何頂点なのか計算する
			if (onlyOnce)
			{
				if (texcoords.size() < positions.size())
				{
					size_t repeatNum = positions.size() - texcoords.size();
					for (size_t i = 0; i < repeatNum; ++i)
					{
						texcoords.emplace_back();
					}
				}

				bool do_break = false;
				for (tmp_faceVertex = 0; tmp_faceVertex < 8; ++tmp_faceVertex)
				{
					std::string tmp_vertexDefinition;
					tmp_s >> tmp_vertexDefinition;
					std::istringstream tmp_v(tmp_vertexDefinition);

					for (uint32_t element = 0; element < 3; ++element)
					{
						std::string index;
						//区切りでインデックスを読んでいく
						std::getline(tmp_v, index, '/');
						if ((index[0] - '0') < 0 || (index[0] - '0') > 9)
						{
							do_break = true;
							break;
						}
					}

					if (do_break) break;
				}

				//スザンヌ専用
				if (tmp_faceVertex < 3)tmp_faceVertex = 2;

				modelData.verticles.emplace_back();
				vertexIndex++;
				onlyOnce = false;
			}

			//面が三角形だった場合
			if (tmp_faceVertex == 3)
			{
				std::vector<VertexData> surcace;
				surcace.resize(tmp_faceVertex);

				for (uint32_t faceVertex = 0; faceVertex < tmp_faceVertex; ++faceVertex)
				{
					std::string vertexDefinition;
					s >> vertexDefinition;
					//頂点の要素へのIndexは「位置/UV/法線」で格納されるので
					//分解してIndexを取得する 
					std::istringstream v(vertexDefinition);
					uint32_t elementIndices[3];
					for (uint32_t element = 0; element < 3; ++element)
					{
						std::string index;
						//区切りでインデックスを読んでいく
						std::getline(v, index, '/');
						elementIndices[element] = std::stoi(index);
					}

					//要素へのIndexから、実際の要素の値を取得して、頂点を構築する
					Vector3 position = positions[elementIndices[0] - 1];
					position.x *= -1;
					//法線
					Vector2 texcoord = texcoords[elementIndices[1] - 1];
					texcoord.v = 1.0f - texcoord.v;
					//テクスチャ座標
					Vector3 normal = normals[elementIndices[2] - 1];
					normal.x *= -1;

					surcace[faceVertex] = { position,texcoord,normal };

				}

				//頂点を逆順で登録することで、回り順を逆にする
				for (int i = tmp_faceVertex - 1; i >= 0; --i)
				{
					modelData.verticles[vertexIndex].emplace_back(surcace[i]);
				}
			}//面が三角形

			//スザンヌ専用
			else if (tmp_faceVertex == 2)
			{
				std::vector<VertexData> surcace;
				surcace.resize(3);

				for (uint32_t faceVertex = 0; faceVertex < 3; ++faceVertex)
				{
					std::string vertexDefinition;
					s >> vertexDefinition;
					//頂点の要素へのIndexは「位置/UV/法線」で格納されるので
					//分解してIndexを取得する 
					std::istringstream v(vertexDefinition);
					uint32_t elementIndices[3];
					for (uint32_t element = 0; element < 3; ++element)
					{
						std::string index;
						//区切りでインデックスを読んでいく
						std::getline(v, index, '/');
						if (element == 1)
						{
							index = "1";
						}

						elementIndices[element] = std::stoi(index);
					}

					//要素へのIndexから、実際の要素の値を取得して、頂点を構築する
					Vector3 position = positions[elementIndices[0] - 1];
					position.x *= -1;
					//テクスチャ座標
					Vector2 texcoord = texcoords[elementIndices[1] - 1];
					texcoord.v = 1.0f - texcoord.v;
					//法線
					Vector3 normal = normals[elementIndices[2] - 1];
					normal.x *= -1;

					surcace[faceVertex] = { position,texcoord,normal };

				}

				//頂点を逆順で登録することで、回り順を逆にする
				for (int i = 3 - 1; i >= 0; --i)
				{
					modelData.verticles[vertexIndex].emplace_back(surcace[i]);
				}
			}//面が2

			//面が4角形(無理やりなのでいつか改善)
			else if (tmp_faceVertex == 4)
			{
				std::vector<VertexData> surcace_quad;
				std::vector<VertexData> surcace_triangle;

				surcace_triangle.resize(3);
				surcace_quad.resize(4);

				for (uint32_t faceVertex = 0; faceVertex < tmp_faceVertex; ++faceVertex)
				{
					std::string vertexDefinition;
					s >> vertexDefinition;
					//頂点の要素へのIndexは「位置/UV/法線」で格納されるので
					//分解してIndexを取得する 
					std::istringstream v(vertexDefinition);
					uint32_t elementIndices[3];
					for (uint32_t element = 0; element < 3; ++element)
					{
						std::string index;
						//区切りでインデックスを読んでいく
						std::getline(v, index, '/');
						elementIndices[element] = std::stoi(index);
					}

					//要素へのIndexから、実際の要素の値を取得して、頂点を構築する
					Vector3 position = positions[elementIndices[0] - 1];
					position.x *= -1;
					//法線
					Vector2 texcoord = texcoords[elementIndices[1] - 1];
					texcoord.v = 1.0f - texcoord.v;
					//テクスチャ座標
					Vector3 normal = normals[elementIndices[2] - 1];
					normal.x *= -1;

					surcace_quad[faceVertex] = { position,texcoord,normal };
				}

				//頂点を逆順で登録することで、回り順を逆にする(012 023)
				modelData.verticles[vertexIndex].emplace_back(surcace_quad[3]);
				modelData.verticles[vertexIndex].emplace_back(surcace_quad[2]);
				modelData.verticles[vertexIndex].emplace_back(surcace_quad[0]);
				modelData.verticles[vertexIndex].emplace_back(surcace_quad[2]);
				modelData.verticles[vertexIndex].emplace_back(surcace_quad[1]);
				modelData.verticles[vertexIndex].emplace_back(surcace_quad[0]);
			}
		}

		else if (identifier == "mtllib")
		{
			//MaterialTemplateLibraryファイルの名前を取得する
			std::string materialFileName;
			s >> materialFileName;
			//基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.materialData = LoadMaterialTemplateFile(directoryPath_, materialFileName);

		}

	}

	return modelData;

}
