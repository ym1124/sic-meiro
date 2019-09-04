#include "..\Header\SceneGame.h"

#include "..\Header\framework.h"

#include	"..\Header\player.h"
#include	"..\Header\camera.h"
#include	"..\Header\shot.h"

#include	"..\Header\collision.h"

#include "..\Header\debug.h"

#include "..\Header\Light.h"

#include "..\Header\trap.h"
#include "..\Header\stage.h"
#include "..\Header\Sprite.h"

#include <time.h>
#include <thread>


MyMesh	field;				//	地面用変数「地面」
Player	player;				//	プレイヤー用変数「プレイヤー」
Trap	trap;
Stage	stage;
extern Ground ground;
std::unique_ptr<Sprite> data;

bool _isload = false;

extern Light player_light;

void loadStage()
{
	_isload = false;
	stage.Init();
	ground.Init();
	_isload = true;
}

void SceneGame::init()
{
	Sprite_Load(&data, L"./Resources/effect.png");

	SetFontSize(128);
	DrawString(200, 300, { 255,255,255 }, L"Now Loading...");
	InitFontSize();

	cameraInit(DirectX::XMFLOAT4(0, -1, 1, 0));

	srand((unsigned)time(NULL));


	player.Initialize("Sa_fbx/body.fbx");
	player.obj.color = DirectX::XMFLOAT4(1.0f, .0f, .0f, 1.0f);//反映されてない
	player.pos = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	trap.Init();

	timer = 0;

	std::thread t1(loadStage);
	if (t1.joinable()) t1.detach();
}

void SceneGame::update()
{
	if (!_isload) return;

	if (player.goal_flg) {
		release();
		scene = SCENE_CLEAR;
		pad.ResetVibration(GamePad::PlayerID::_1P);
		return;
	}

	if (kb.PushTrigger(VK_RETURN)) {
		release();
		scene = SCENE_TITLE;
		pad.ResetVibration(GamePad::PlayerID::_1P);
		return;
	}

	if (0.1f >= player.GetElectric()) {
		release();
		scene = SCENE_OVER;
		pad.ResetVibration(GamePad::PlayerID::_1P);
		return;
	}

	timer++;
	if (0.0f <= player.GetElectric()) {
		player.SetElectric(player.GetElectric() - 0.01f);
	}

	if (kb.PushTrigger('L'))
	{
		for (int i = 0; i < 8; i++)
		{
			if (light_exist[i].x == 0)
			{
				light_exist[i].x = 1;
				break;
			}
		}
	}

	trap.Update();

	player.Move();					//	「プレイヤー」の移動処理
	camera.Update();

	player_light.pos = { player.pos.x,player.pos.y + 20.0f,player.pos.z };
}

void SceneGame::render()
{
	//	ビュー変換行列
	view = camera.GetViewMatrix();

	blender::Set(blender::ALPHA);

	if (!_isload) {
		player.angle = DirectX::XMConvertToRadians(90.0f);
		player.obj.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		player.head.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		player.tail.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		player.Render(view, projection, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),1);			//	「プレイヤー」の描画処理
		SetFontSize(64);
		
		DrawString(100, 600, { 255,255,255 }, L"NOW LOADING", player.GetElectric());//これいれるとばぐる？
	}

	else
	{
		trap.Render(view, projection, light_direction, light.pos, 2, light);
		stage.Render(view, projection, light_direction);
		ground.Render(view, projection, light_direction);

		player.obj.color = DirectX::XMFLOAT4(1.0f, .0f, .0f, 1.0f);
		player.head.color = DirectX::XMFLOAT4(1.0f, .0f, .0f, 1.0f);
		player.tail.color = DirectX::XMFLOAT4(1.0f, .0f, .0f, 1.0f);
		player.Render(view, projection, DirectX::XMFLOAT4(0.0f, -0.4f, 0.4f, 0.0f));			//	「プレイヤー」の描画処理

		SetFontSize(64);
		//DrawString(100, 300, { 0,0,255 }, L"ゲーム");
		DrawString(500, 100, { 255,255,255 }, L"HP : %.1f", player.GetElectric());
	}
}

void SceneGame::release()
{
	player.Release();			//	「プレイヤー」の解放処理
	trap.Release();
	stage.Release();
	ground.Release();
}