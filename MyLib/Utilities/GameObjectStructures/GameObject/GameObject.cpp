#include "GameObject.h"


void ObjectBehavior::SetObjectID(ObjectID::Tag tag_)
{
    //ゲームオブジェクトの生成カウント
    ObjectBehavior::objectNumber++;

    //ゲームオブジェクトのテーブル
    ObjectID objectIDTable[(int)ObjectID::Tag::kCount]
    {
        {tag_,0x0000000f,0x00000f00,"Player[" + std::to_string(ObjectBehavior::objectNumber) + "]"},
        {tag_,0x000000f0,0x00000000,"MovableBlock[" + std::to_string(ObjectBehavior::objectNumber) + "]"},
        {tag_,0x0000f000,0x0000000f,"Key[" + std::to_string(ObjectBehavior::objectNumber) + "]"},
        {tag_,0x00000f00,0x0000000f,"Goal[" + std::to_string(ObjectBehavior::objectNumber) + "]"},
        {tag_,0x0000f0000,0x00000000,"Doudemoii[" + std::to_string(ObjectBehavior::objectNumber) + "]"},

    };

    objectID = objectIDTable[(int)tag_];
}

void ObjectBehavior::OnTriggerEnter(ObjectID::Tag tag_)
{
    collisionBackActivationMap[tag_].isActive = true;
}


