#include "..\Header\Scene.h"
#include "..\Header\SceneTitle.h"
#include "..\Header\SceneGame.h"
#include "..\Header\SceneOver.h"
#include "..\Header\SceneClear.h"
#include "..\Header\SceneSelect.h"

Scene *pScene = nullptr;

SceneTitle sceneTitle;
SceneGame sceneGame;
SceneOver sceneOver;
SceneClear sceneClear;
SceneSelect sceneSelect;

int scene = 0;
int now_stage = 0;

void SceneManager::init()
{
	scene = SCENE_TITLE;
	next_scene = -1;
	now_stage = 0;
}

void SceneManager::update()
{
	// 更新処理
	setScene(scene);

	pScene->update();				// ゲームの更新
}

void SceneManager::render()
{
	pScene->render();				// ゲームの描画処理
}

void SceneManager::final()
{
	pScene->release();
}

void SceneManager::setScene(int const scene)
{
	switch (scene) {
	case SCENE_TITLE:
		pScene = &sceneTitle;
		break;

		//case SCENE_TUTORIAL:
		//	pScene = &sceneTuto;
		//	break;

	case SELECT_STAGE:
		pScene = &sceneSelect;
		break;

	case SCENE_GAME:
		pScene = &sceneGame;
		break;

	case SCENE_CLEAR:
		pScene = &sceneClear;
		break;

	case SCENE_OVER:
		pScene = &sceneOver;
		break;

		//case SCENE_NAME:
		//	pScene = &sceneName;
		//	break;
	}

	if (scene != next_scene) {
		pScene->init();
		next_scene = scene;
	}
}

void Scene::cameraInit(DirectX::XMFLOAT4 ilght_direct)
{
	camera.Initialize();

	//	投影変換行列
	projection = camera.GetProjectionMatrix();

	//	光源(平行光)
	light_direction = ilght_direct;		//	上+奥 から 下+前へのライト
}

void Scene::cameraMove()
{

	static float speed = 0.5f;
	static float rotate_speed = 5.0f;
	static float angle = 100.0f;
	static float rotate_angleX = 0.0f;
	static float rotate_angleZ = 0.0f;

	if (pad.CheckLStickRange(GamePad::PlayerID::_1P)) {
		camera.point_view.z += speed*cosf(pad.GetLStickRadianAngle(GamePad::PlayerID::_1P));
		camera.point_view.x += speed*sinf(pad.GetLStickRadianAngle(GamePad::PlayerID::_1P));
	}

	if (pad.PushTrigger(XINPUT_GAMEPAD_RIGHT_THUMB, GamePad::PlayerID::_1P)) pad.SetVibration(0.0f, 100.0f, GamePad::PlayerID::_1P);
	if (pad.ReleaseTrigger(XINPUT_GAMEPAD_LEFT_THUMB, GamePad::PlayerID::_1P)) pad.ResetVibration(GamePad::PlayerID::_1P);

	if (pad.PushTrigger(XINPUT_GAMEPAD_X, GamePad::PlayerID::_2P)) pad.SetVibration(100.0f, 0.0f, GamePad::PlayerID::_2P);
	if (pad.ReleaseTrigger(XINPUT_GAMEPAD_X, GamePad::PlayerID::_2P)) pad.ResetVibration(GamePad::PlayerID::_2P);

	if (pad.LTrigger(GamePad::TriggerType::PUSH_TRIGGER, GamePad::PlayerID::_1P))pad.SetVibration(0.0f, 100.0f, GamePad::PlayerID::_1P);
	if (pad.LTrigger(GamePad::TriggerType::RELEASE_TRIGGER, GamePad::PlayerID::_1P)) pad.ResetVibration(GamePad::PlayerID::_1P);

	if (pad.RTrigger(GamePad::TriggerType::PUSH_TRIGGER, GamePad::PlayerID::_1P))pad.SetVibration(100.0f, 0.0f, GamePad::PlayerID::_1P);
	if (pad.RTrigger(GamePad::TriggerType::RELEASE_TRIGGER, GamePad::PlayerID::_1P)) pad.ResetVibration(GamePad::PlayerID::_1P);

	if (kb.Push(VK_CONTROL)) {
		camera.point_view.x = camera.point_view.y = camera.point_view.z = 0.0f;
	}

	if (kb.Push(VK_SPACE)) camera.point_view.y += 1.0f;
	if (kb.Push('Z')) camera.point_view.y -= 1.0f;

	if (kb.Push(VK_UP)) camera.position.z += speed;
	if (kb.Push(VK_DOWN)) camera.position.z -= speed;
	if (kb.Push(VK_RIGHT)) camera.position.x += speed;
	if (kb.Push(VK_LEFT)) camera.position.x -= speed;

	if (kb.Push('W')) camera.point_view.z += rotate_speed*TO_RADIAN;
	if (kb.Push('S')) camera.point_view.z -= rotate_speed*TO_RADIAN;
	if (kb.Push('D')) camera.point_view.x += rotate_speed*TO_RADIAN;
	if (kb.Push('A')) camera.point_view.x -= rotate_speed*TO_RADIAN;

	//if (kb.Push('W')) rotate_angleZ += rotate_speed*TO_RADIAN;
	//if (kb.Push('S')) rotate_angleZ -= rotate_speed*TO_RADIAN;
	//if (kb.Push('D')) rotate_angleX += rotate_speed*TO_RADIAN;
	//if (kb.Push('A')) rotate_angleX -= rotate_speed*TO_RADIAN;

	//camera.point_view.z = angle*cosf(rotate_angleZ);
	//camera.point_view.x = angle*sinf(rotate_angleX);

	if (kb.Push('Y')) {
		light_direction = DirectX::XMFLOAT4(0, 1, -1, 0);
	}
	if (kb.Push('U')) {
		light_direction = DirectX::XMFLOAT4(-1, 1, 1, 0);
	}
	if (kb.Push('I')) {
		light_direction = DirectX::XMFLOAT4(1, -1, 1, 0);
	}
	if (kb.Push('O')) {
		light_direction = DirectX::XMFLOAT4(1, 1, -1, 0);
	}
	if (kb.Push('P')) {
		light_direction = DirectX::XMFLOAT4(1, 1, 1, 0);
	}
	//if (mouse.Push(Mouse::Botton::LEFT)) {
	//	scale.x += 0.01f;
	//	scale.y += 0.01f;
	//	scale.z += 0.01f;
	//}
	//if (mouse.Push(Mouse::Botton::RIGHT)) {
	//	scale.x -= 0.01f;
	//	scale.y -= 0.01f;
	//	scale.z -= 0.01f;
	//}

}