#include "SceneBehavior.h"

void SceneController::Add(TagScene tag_, SceneBehavior* scene_)
{
	allScene[tag_] = scene_;
}

SceneBehavior* SceneController::ChangeScene(SceneBehavior* cur_)
{
	allScene[TagScene::kNone] = cur_;
	SceneBehavior* ret = allScene[nextScene];

	if (nextScene != TagScene::kNone)
	{
		ret->Initialize();
	}

	nextScene = TagScene::kNone;

	return ret;
}

void SceneController::ReceiveSignal(TagScene next_)
{
	nextScene = next_;
}

SceneController* SceneController::GetInstance()
{
	static SceneController sceneController;
	return &sceneController;

}