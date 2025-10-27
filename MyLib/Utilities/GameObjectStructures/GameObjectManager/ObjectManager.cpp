#include "ObjectManager.h"
#include <algorithm> 
#include "../../CollisionDetectionsStructures/CollisionDetection.h"


void ObjectManager::SortObject()
{
	//更新処理順で並び変える
	std::sort(objData.begin(), objData.end(),
		[](GameObject const* lhs, GameObject const* rhs) { return lhs->objectID.tag < rhs->objectID.tag; });
}

void ObjectManager::Finalize()
{

}

//ゲームオブジェクトすべての当たり判定をとる
void ObjectManager::ChackAllCollision(GameObject* thisObj_)
{
	for (auto const& otherObj : objData)
	{
		//衝突相手がアクティブでなければ
		if (!otherObj->isActive)
		{
			continue;
		}
		//マスク処理
		else if ((thisObj_->objectID.collisionAttribute & otherObj->objectID.collisionMask) == 0 )
		{
			continue;
		}

		//ワールド座標を取得
		Vector3 thisWorldPos = thisObj_->mainTrans.GetWorldPos();
		Vector3 otherWorldPos = otherObj->mainTrans.GetWorldPos();

		//キューブコリジョン
		if (CollisionDetections::Col3D::GetCubeCollisionResult(thisWorldPos, thisObj_->cubeCollider,
			otherWorldPos, otherObj->cubeCollider))
		{
			//双方のオブジェクトの衝突反応関数をアクティブ化する
			thisObj_->OnTriggerEnter(otherObj->objectID.tag);
			otherObj->OnTriggerEnter(thisObj_->objectID.tag);

			//双方のオブジェクトの衝突相手を登録する
			thisObj_->collidedObj_ = otherObj;
			otherObj->collidedObj_ = thisObj_;

			continue;
		}
	}

}

void ObjectManager::PassStoredGameObjectAddress()
{
	if (tmp_addressStorage.size() > 0)
	{
		for (auto itr = tmp_addressStorage.begin(); itr != tmp_addressStorage.end(); ++itr)
		{
			RegisterAsGameObject((*itr));
		}

		tmp_addressStorage.clear();
	}

}

void ObjectManager::DeleteObject()
{
	if (tmp_deleteAddressStorage.size() > 0)
	{
		for (auto itr_delete = tmp_deleteAddressStorage.begin(); itr_delete != tmp_deleteAddressStorage.end();++itr_delete)
		{
			//照合
			for (auto itr_objData = objData.begin(); itr_objData != objData.end();++itr_objData)
			{
				if ((*itr_delete) == (*itr_objData))
				{
					//delete (*itr_objData);
					objData.erase(itr_objData);

					break;
				}
			}
		}

		tmp_deleteAddressStorage.clear();

	}

}



