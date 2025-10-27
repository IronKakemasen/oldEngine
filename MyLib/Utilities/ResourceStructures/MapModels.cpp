#include "MapModels.h"


void MapModels::CreateModels()
{
    //チェーン
    mapContainer[TagModel::kChain] = NoviceLike::CreateModel("./Resource/Models/chain", "chain.obj");

    ////ギア
    //mapContainer[TagModel::kGearAround] = NoviceLike::CreateModel("./Resource/Models/gear", "around.obj");
    //mapContainer[TagModel::kGearCenter] = NoviceLike::CreateModel("./Resource/Models/gear", "center.obj");
    //mapContainer[TagModel::kGearEye] = NoviceLike::CreateModel("./Resource/Models/gear", "eye.obj");
    //mapContainer[TagModel::kGearLarge] = NoviceLike::CreateModel("./Resource/Models/gear", "large.obj");
    //mapContainer[TagModel::kGearMiddle] = NoviceLike::CreateModel("./Resource/Models/gear", "middle.obj");
    //mapContainer[TagModel::kGearSmall] = NoviceLike::CreateModel("./Resource/Models/gear", "small.obj");

}

std::unique_ptr<Model> MapModels::GetModelMusic(TagModel model_)
{
    std::unique_ptr<Model> ret;

    switch (model_
)
    {
    case TagModel::kChain:
        ret = std::move(NoviceLike::CreateModelMusic("./Resource/Models/chain", "chain.obj"));

        break;
    case TagModel::kGearAround:
        ret = std::move(NoviceLike::CreateModelMusic("./Resource/Models/gear", "around.obj"));

        break;
    case TagModel::kGearCenter:
        ret = std::move(NoviceLike::CreateModelMusic("./Resource/Models/gear", "center.obj"));

        break;
    case TagModel::kGearEye:
        ret = std::move(NoviceLike::CreateModelMusic("./Resource/Models//gear", "eye.obj"));

        break;
    case TagModel::kGearLarge:
        ret = std::move(NoviceLike::CreateModelMusic("./Resource/Models/gear", "large.obj"));

        break;
    case TagModel::kGearMiddle:
        ret = std::move(NoviceLike::CreateModelMusic("./Resource/Models/gear", "middle.obj"));

        break;
    case TagModel::kGearSmall:
        ret = std::move(NoviceLike::CreateModelMusic("./Resource/Models/gear", "small.obj"));

        break;
    case TagModel::kDoor:
        ret = std::move(NoviceLike::CreateModelMusic("./Resource/Models/door", "door.obj"));

        break;
    case TagModel::kKeyHole:
        ret = std::move(NoviceLike::CreateModelMusic("./Resource/Models/door", "keyHole.obj"));

        break;
    case TagModel:: kKey:
        ret = std::move(NoviceLike::CreateModelMusic("./Resource/Models/door", "key.obj"));

        break;
    case TagModel::kTriangle:
          ret = std::move(NoviceLike::CreateModelMusic("./Resource/Models/triangle", "triangle.obj"));

        break;
    case TagModel::kTitle:
        ret = std::move(NoviceLike::CreateModelMusic("./Resource/Models/title", "title.obj"));

        break;
    }

    return std::move(ret);

}

MapModels::~MapModels()
{

}

MapModels::MapModels()
{
    CreateModels();
}
