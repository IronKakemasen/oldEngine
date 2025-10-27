#pragma once
#include "../BenriFunctionStructures/BenriFunctions.h"
#include "../../Utilities/GameObjectStructures/GameObjectManager/ObjectManager.h"
#include "../LightStructures/Sun.h"
#include "../ResourceStructures/MapTextures.h"
#include "../ResourceStructures/MapModels.h"
#include "../DebugUIStructures/DebugUI.h"
#include "../../../ExclusiveCodes/Audio/AudioHandle.h"
#include "../../../ExclusiveCodes/Audio/AudioPlayer.h"

struct SceneBehavior
{

	enum class Status
	{
		kEntering,
		kTransitioning,
		kPlayable,
		kUnPlayable,
	};

	Status sceneStatus = Status::kPlayable;

	MapModels* mapModels = nullptr;
	MapTextures* mapTextures = nullptr;

	std::unique_ptr<ObjectManager> objManager;
	std::unique_ptr<Sun> lightSun;

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Initialize() = 0;
	virtual void Debug() = 0;

};


struct SceneController
{
	enum class TagScene
	{
		kTitle,
		kInGame,
		kSelect,


		kNone,
		kCount,
	};

	static inline TagScene nextScene = TagScene::kNone;

	std::map<TagScene, SceneBehavior*> allScene;
	static SceneController* GetInstance();


	void Add(TagScene tag_,SceneBehavior* scene_);
	SceneBehavior* ChangeScene(SceneBehavior* cur_);
	static void ReceiveSignal(TagScene next_);

	SceneController() = default;
	~SceneController() = default;
	SceneController(const SceneController&) = delete;
	SceneController& operator=(const SceneController&) = delete;


};

