#pragma once

#include "../../NoviceLike/NoviceLike.h"

#include <map>

enum class TagModel
{
    kChain,
    kGearAround,//ギア周りのオブジェクト
    kGearCenter,//ギア中央
    kGearEye,//ギア目
    kGearLarge,//ギア大きい
    kGearMiddle,//ギア中央
    kGearSmall,//ギア小さい
    kDoor,//ドア
    kKeyHole,//鍵穴
    kKey,// 鍵
    kTriangle,//三角形
    kTitle,//タイトル
};


struct MapModels
{

    inline static std::map<TagModel, Model> mapContainer;

    ~MapModels();
    MapModels();

    static std::unique_ptr<Model> GetModelMusic(TagModel model_);

private:
    void CreateModels();

};


