#include "GlobalVariables.h"

Vector3 GlobalVariables::GetVector3Value(std::string groupName_, std::string key_)
{
	Group& group = datas.at(groupName_);
	Item& i = group.items.at(key_);

	return 	std::get<Vector3>(i.value);
}
Vector2 GlobalVariables::GetVector2Value(std::string groupName_, std::string key_)
{
	Group& group = datas.at(groupName_);
	Item& i = group.items.at(key_);

	return 	std::get<Vector2>(i.value);
}



void GlobalVariables::SaveFile(std::string groupName)
{
	auto itr_group = datas.find(groupName);
	assert(itr_group != datas.end());

	json root;
	root = json::object();
	root[groupName] = json::object();

	for (auto itr_item = itr_group->second.items.begin(); itr_item != itr_group->second.items.end(); ++itr_item)
	{
		std::string itemName = itr_item->first;
		Item& item = itr_item->second;
		if (std::holds_alternative<int>(item.value))
		{
			root[groupName][itemName] = std::get<int>(item.value);
		}

		else if (std::holds_alternative<float>(item.value))
		{
			root[groupName][itemName] = std::get<float>(item.value);
		}

		else if (std::holds_alternative<Vector3>(item.value))
		{
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = json::array({ value.x,value.y,value.z  });
		}

		else if (std::holds_alternative<Vector2>(item.value))
		{
			Vector2 value = std::get<Vector2>(item.value);
			root[groupName][itemName] = json::array({ value.u,value.v});
		}

		std::string direc = "Resource/GlobalVariables/";

		std::filesystem::path dir(direc);
		if (std::filesystem::exists(dir))
		{
			std::filesystem::create_directory(direc);
		}

		std::string filePath = direc + groupName + ".json";
		std::ofstream ofs;
		ofs.open(filePath);

		if (ofs.fail())
		{
			std::string mess = "sippai";
			MessageBoxA(nullptr, mess.c_str(), "GlobalVariables", 0);
			assert(0);
			return;
		}

		ofs << std::setw(4) << root << std::endl;
		ofs.close();
	}
}

void GlobalVariables::LoadFile(std::string groupName_)
{
	std::string direc = "Resource/GlobalVariables/";

	std::string filePath = direc + groupName_ + ".json";
	std::ifstream ifs;
	ifs.open(filePath);
	if (ifs.fail())
	{
		std::string mess = "sippai";
		MessageBoxA(nullptr, mess.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	json root;
	ifs >> root;
	ifs.close();

	json::iterator itGroup = root.find(groupName_);
	assert(itGroup != root.end());
	for (auto itr_item = itGroup->begin(); itr_item != itGroup->end(); ++itr_item)
	{
		const std::string& itemName = itr_item.key();

		if (itr_item->is_number_integer())
		{
			int value = itr_item->get<int>();
			SetValue(groupName_, itemName, value);

		}
		else if (itr_item->is_number_float())
		{
			double value = itr_item->get<double>();
			SetValue(groupName_, itemName, (float)value);
		}
		else if (itr_item->is_array() && itr_item->size() == 3)
		{
			Vector3 value = { itr_item->at(0),itr_item->at(1) ,itr_item->at(2)};
			SetValue(groupName_, itemName, value);
		}

		else if (itr_item->is_array() && itr_item->size() == 2)
		{
			Vector2 value = { itr_item->at(0),itr_item->at(1) };
			SetValue(groupName_, itemName, value);
		}

	}
}


void GlobalVariables::LoadFiles()
{
	std::string direc = "Resource/GlobalVariables";
	std::filesystem::directory_iterator itr_dir(direc);
	for (const auto& entry : itr_dir)
	{
		const std::filesystem::path& filePath = entry.path();
		std::string extension = filePath.extension().string();
		if (extension.compare(".json") != 0) continue;
		LoadFile(filePath.stem().string());
	}
}


void GlobalVariables::SetValue(std::string groupName_, std::string key_, int value_)
{
	Group& group = datas[groupName_];
	Item newItem;
	newItem.value = value_;
	group.items[key_] = newItem;
}
void GlobalVariables::SetValue(std::string groupName_, std::string key_, float value_)
{
	Group& group = datas[groupName_];
	Item newItem;
	newItem.value = value_;
	group.items[key_] = newItem;

}
void GlobalVariables::SetValue(std::string groupName_, std::string key_, Vector3 value_)
{
	Group& group = datas[groupName_];
	Item newItem;
	newItem.value = value_;
	group.items[key_] = newItem;

}

void GlobalVariables::SetValue(std::string groupName_, std::string key_, Vector2 value_)
{
	Group& group = datas[groupName_];
	Item newItem;
	newItem.value = value_;
	group.items[key_] = newItem;

}



void GlobalVariables::AddValue(std::string groupName_, std::string key_, int value_)
{
	Group& group = datas[groupName_];

	if (!group.items.contains(key_))
	{
		SetValue(groupName_, key_, value_);
	}

}
void GlobalVariables::AddValue(std::string groupName_, std::string key_, float value_)
{
	Group& group = datas[groupName_];

	if (!group.items.contains(key_))
	{
		SetValue(groupName_, key_, value_);
	}

}
void GlobalVariables::AddValue(std::string groupName_, std::string key_, Vector3 value_)
{
	Group& group = datas[groupName_];

	if (!group.items.contains(key_))
	{
		SetValue(groupName_, key_, value_);
	}
}

void GlobalVariables::AddValue(std::string groupName_, std::string key_, Vector2 value_)
{
	Group& group = datas[groupName_];

	if (!group.items.contains(key_))
	{
		SetValue(groupName_, key_, value_);
	}

}


GlobalVariables* GlobalVariables::GetInstance()
{
	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::CreateGroup(std::string groupName_)
{
	datas[groupName_];
}

void GlobalVariables::Update()
{
	//ImGui::Begin("GlobalV");

	//for (auto itr_Group = datas.begin(); itr_Group != datas.end(); ++itr_Group)
	//{
	//	std::string groupName = itr_Group->first;
	//	Group& group = itr_Group->second;

	//	if (!ImGui::BeginMenu(groupName.c_str())) continue;
	//	for (auto itr_item = group.items.begin(); itr_item != group.items.end(); ++itr_item)
	//	{
	//		std::string itemName = itr_item->first;
	//		Item& item = itr_item->second;

	//		if (std::holds_alternative<int>(item.value))
	//		{
	//			int* ptr = std::get_if<int>(&item.value);
	//			ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
	//		}

	//		else if (std::holds_alternative<float>(item.value))
	//		{
	//			float* ptr = std::get_if<float>(&item.value);
	//			ImGui::SliderFloat(itemName.c_str(), ptr, 0, 100);
	//		}

	//		else if (std::holds_alternative<Vector4<float>>(item.value))
	//		{
	//			Vector4<float>* ptr = std::get_if<Vector4<float>>(&item.value);
	//			ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.01f);
	//		}
	//	}

	//	ImGui::Text("\n");
	//	if (ImGui::Button("Save"))
	//	{
	//		SaveFile(groupName);
	//	}
	//	ImGui::EndMenu();
	//}


	////ImGui::EndMenuBar();
	//ImGui::End();

}
