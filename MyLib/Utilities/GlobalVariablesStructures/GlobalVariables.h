#pragma once
#include <variant>
#include <map>
#include <string>
#include <fstream>
#include "../../NoviceLike/NoviceLike.h"
#include "../../../External/nlohmann/json.hpp"


using json = nlohmann::json;

struct GlobalVariables
{
	struct Item
	{
		std::variant < int, float, Vector3,Vector2> value;
	};

	struct Group
	{
		std::map<std::string, Item> items;
	};

	std::map<std::string, Group> datas;

	static GlobalVariables* GetInstance();

	void CreateGroup(std::string groupName_);
	void SetValue(std::string groupName_, std::string key_, int value_);
	void SetValue(std::string groupName_, std::string key_, float value_);
	void SetValue(std::string groupName_, std::string key_, Vector3 value_);
	void SetValue(std::string groupName_, std::string key_, Vector2);
	void AddValue(std::string groupName_, std::string key_, int value_);
	void AddValue(std::string groupName_, std::string key_, float value_);
	void AddValue(std::string groupName_, std::string key_, Vector3 value_);
	void AddValue(std::string groupName_, std::string key_, Vector2);

	Vector3 GetVector3Value(std::string groupName_, std::string key_);
	Vector2 GetVector2Value(std::string groupName_, std::string key_);

	void LoadFile(std::string groupName_);
	void LoadFiles();
	void Update();
	void SaveFile(std::string groupName);

	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	GlobalVariables& operator=(const GlobalVariables&) = delete;

};

