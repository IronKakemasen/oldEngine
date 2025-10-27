#pragma once
#include <algorithm> 
#include <string>
#include <map>
#include "../GameObject/GameObject.h"



class ObjectManager
{

private:
	//ゲームオブジェクトのコンテナ
	std::vector<GameObject*> objData;


	std::vector<GameObject*> tmp_addressStorage;
	std::vector<GameObject*> tmp_deleteAddressStorage;


public:

	//ゲームオブジェクトのコンテナ「objData」を登録する
	template<typename T>
	void RegisterAsGameObject(T* t)
	{
		objData.emplace_back(t);
		SortObject();
	}

	//以下気にしなくていい↓↓↓↓↓↓

	//すべてのゲームオブジェクトの当たり判定をとる
	void ChackAllCollision(GameObject* thisObj_);


	template<typename T>
	void StoreGameObjectAddress(T* t)
	{
		tmp_addressStorage.emplace_back(t);
	}
	
	template<typename T>
	void StoreDeleteGameObjectAddress(T* t)
	{
		tmp_deleteAddressStorage.emplace_back(t);
	}


	void PassStoredGameObjectAddress();
	void DeleteObject();



	// オブジェクトをソートする
	void SortObject();
	auto& GetObjData() { return objData; }
	void Finalize();

};





