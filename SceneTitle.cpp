#include "..\Header\SceneTitle.h"

#include "..\Header\framework.h"

#include "..\Header\Light.h"
#include "..\Header\debug.h"


Light title_light;
Light t_light;

bool isload = false;

void SceneTitle::init()
{
	SetFontSize(128);
	DrawString(200, 300, { 255,255,255 }, L"Now Loading...");
	InitFontSize();

	title.Initialize();
	title.Load("Sa_fbx/title.fbx");
	title.angle = { 0.0f,10.0f*TO_RADIAN,0.0f };
	title.pos.x = -7.0f;

	push.Initialize();
	push.Load("Sa_fbx/push_enter.fbx");
	push.angle = { 0.0f,0.0f*TO_RADIAN,0.0f };
	push.scale = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
	push.pos.x = 0.0f;
	push.pos.y = -15.0f;
	push.pos.z = -5.0f;

	player.Initialize("Sa_fbx/body.fbx");
	push.scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	player.obj.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//player.pos = DirectX::XMFLOAT3(0.0f, 30.0f, -38.0f);

	//	「地面」を初期化
	mesh.Release();
	mesh.Initialize();
	mesh.Load("Sa_fbx/sawani.fbx");

	mesh.pos = { -30.0f,-10.0f,-3.0f };
	mesh.angle = { 0.0f,150.0f*TO_RADIAN,0.0f };

	back.Initialize();
	back.Load("FBX/001_cube.fbx");
	back.pos = DirectX::XMFLOAT3(0.0f, 0.0f, 50.0f);
	back.scale = DirectX::XMFLOAT3(100.0f, 0.1f, 100.0f);
	back.angle = DirectX::XMFLOAT3(90.0f, 0.0f, 0.0f);

	title_bgm= audio.LoadSound(L"Resources/Sounds/going_mad1.wav", 30);//ループ
	warp_se = audio.LoadSound(L"Resources/Sounds/surprising_shock2 (online-audio-converter.com).wav", 0);//ループ

	//world_box.Initialize();
	//world_box.Load("FBX/001_cube.fbx");
	////world_box.pos = DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f);
	//world_box.scale = DirectX::XMFLOAT3(100.0f, 50.0f, 90.0f);
	////world_box.angle = DirectX::XMFLOAT3(90.0f, 0.0f, 0.0f);

	cameraInit(DirectX::XMFLOAT4(0, -1, 1, 0));

	iskey = false;
	is_move_camera = false;

	timer = 0.0f;
	font_angle = 0;

	holler_state = 0;
	holler_timer = 0;

	title_light.pos = DirectX::XMFLOAT3(0.0f, 3.0f, -10.0f);
	title_light.attenution = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	title_light.color = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);

	t_light.pos = DirectX::XMFLOAT3(0.0f, 3.0f, -100.0f);
	t_light.attenution = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	t_light.color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	camera.position = DirectX::XMFLOAT3(0.0f, 1.2f, -50.0f);
	camera.point_view = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void SceneTitle::update()
{
	if (kb.PushTrigger(VK_RETURN) || pad.PushTrigger(VK_PAD_A, GamePad::PlayerID::_1P)) {
		release();
		scene = SCENE_GAME;
		return;
	}

	player.pos.y = 0.0f;

	push.angle.y += 1.0f*TO_RADIAN;

	mesh.anim_timer = FRAMEWORK.GetTimeIntarval();

	if (kb.PushTrigger('0')) {
		iskey = !iskey;
	}

	if (kb.PushTrigger('C')) {
		is_move_camera = !is_move_camera;
	}

	if (kb.PushTrigger('1')) {
		mesh.pos = DirectX::XMFLOAT3(0, 0, 0);
		mesh.angle = DirectX::XMFLOAT3(0, 0, 0);
		mesh.scale = DirectX::XMFLOAT3(1, 1, 1);
	}


	if (iskey)
		move();

	if (is_move_camera)
		cameraMove();

	holler();
}

void SceneTitle::render()
{
	//	ビュー変換行列
	view = camera.GetViewMatrix();

	blender::Set(blender::NONE, FRAMEWORK.getContext());

	player.obj.color = DirectX::XMFLOAT4(1.0f, .0f, .0f,.0f);
	player.head.color = DirectX::XMFLOAT4(1.0f, .0f, 0.0f, 1.0f);
	player.tail.color = DirectX::XMFLOAT4(1.0f, .0f, 0.0f, 1.0f);
	player.Render(view, projection, p_light);

}

void SceneTitle::release()
{
	mesh.Release();

	title.Release();
	back.Release();

	push.Release();

	audio.ReleaseSound(title_bgm);
	audio.ReleaseSound(warp_se);
}

void SceneTitle::move()
{
	static float speed = 0.05f;
	static float rotate_speed = 0.5f;

	if (pad.CheckLStickRange(GamePad::PlayerID::_1P)) {
		mesh.pos.z += speed*cosf(pad.GetLStickRadianAngle(GamePad::PlayerID::_1P));
		mesh.pos.x += speed*sinf(pad.GetLStickRadianAngle(GamePad::PlayerID::_1P));
	}

	if (pad.Push(XINPUT_GAMEPAD_DPAD_UP, GamePad::PlayerID::_1P)) mesh.pos.y += speed;
	if (pad.Push(XINPUT_GAMEPAD_DPAD_DOWN, GamePad::PlayerID::_1P)) mesh.pos.y -= speed;



	if (pad.Push(XINPUT_GAMEPAD_A, GamePad::PlayerID::_1P)) {
		mesh.scale.x += 0.01f;
		mesh.scale.y += 0.01f;
		mesh.scale.z += 0.01f;
	}

	if (pad.Push(XINPUT_GAMEPAD_B, GamePad::PlayerID::_1P)) {
		mesh.scale.x -= 0.01f;
		mesh.scale.y -= 0.01f;
		mesh.scale.z -= 0.01f;
	}


	if (pad.PushTrigger(XINPUT_GAMEPAD_RIGHT_THUMB, GamePad::PlayerID::_1P)) pad.SetVibration(0.0f, 100.0f, GamePad::PlayerID::_1P);
	if (pad.ReleaseTrigger(XINPUT_GAMEPAD_LEFT_THUMB, GamePad::PlayerID::_1P)) pad.ResetVibration(GamePad::PlayerID::_1P);

	if (pad.PushTrigger(XINPUT_GAMEPAD_X, GamePad::PlayerID::_2P)) pad.SetVibration(100.0f, 0.0f, GamePad::PlayerID::_2P);
	if (pad.ReleaseTrigger(XINPUT_GAMEPAD_X, GamePad::PlayerID::_2P)) pad.ResetVibration(GamePad::PlayerID::_2P);

	if (pad.LTrigger(GamePad::TriggerType::PUSH_TRIGGER, GamePad::PlayerID::_1P))pad.SetVibration(0.0f, 100.0f, GamePad::PlayerID::_1P);
	if (pad.LTrigger(GamePad::TriggerType::RELEASE_TRIGGER, GamePad::PlayerID::_1P)) pad.ResetVibration(GamePad::PlayerID::_1P);

	if (pad.RTrigger(GamePad::TriggerType::PUSH_TRIGGER, GamePad::PlayerID::_1P))pad.SetVibration(100.0f, 0.0f, GamePad::PlayerID::_1P);
	if (pad.RTrigger(GamePad::TriggerType::RELEASE_TRIGGER, GamePad::PlayerID::_1P)) pad.ResetVibration(GamePad::PlayerID::_1P);



	if (pad.CheckRStickRange(GamePad::PlayerID::_1P)) {
		mesh.angle.y -= speed*0.1f*sinf(pad.GetRStickRadianAngle(GamePad::PlayerID::_1P));
		mesh.angle.x += speed*0.1f*cosf(pad.GetRStickRadianAngle(GamePad::PlayerID::_1P));
	}


	if (pad.Push(XINPUT_GAMEPAD_RIGHT_SHOULDER, GamePad::PlayerID::_1P)) mesh.angle.z -= rotate_speed*TO_RADIAN;
	if (pad.Push(XINPUT_GAMEPAD_LEFT_SHOULDER, GamePad::PlayerID::_1P)) mesh.angle.z += rotate_speed*TO_RADIAN;


	if (kb.Push(VK_CONTROL)) {
		mesh.pos.x = mesh.pos.y = mesh.pos.z = 0.0f;
	}

	if (kb.PushTrigger(VK_SPACE)) mesh.pos.y += 1.0f;
	if (kb.ReleaseTrigger('Z')) mesh.pos.y -= 1.0f;

	if (kb.Push(VK_SHIFT) && kb.None('U')) mesh.angle.z += rotate_speed*TO_RADIAN;

	if (kb.Push(VK_UP)) mesh.pos.z += speed;
	if (kb.Push(VK_DOWN)) mesh.pos.z -= speed;
	if (kb.Push(VK_RIGHT)) mesh.pos.x += speed;
	if (kb.Push(VK_LEFT)) mesh.pos.x -= speed;

	if (kb.Push('W')) mesh.angle.x += rotate_speed*TO_RADIAN;
	if (kb.Push('S')) mesh.angle.x -= rotate_speed*TO_RADIAN;
	if (kb.Push('D')) mesh.angle.y += rotate_speed*TO_RADIAN;
	if (kb.Push('A')) mesh.angle.y -= rotate_speed*TO_RADIAN;

	if (kb.Push('E')) {
		mesh.scale.x += 0.01f;
		mesh.scale.y += 0.01f;
		mesh.scale.z += 0.01f;
	}
	if (kb.Push('Q')) {
		mesh.scale.x -= 0.01f;
		mesh.scale.y -= 0.01f;
		mesh.scale.z -= 0.01f;
	}

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

	if (mouse.PushTrigger(Mouse::Botton::LEFT)) mesh.pos.x += 5.0f;
	if (mouse.PushTrigger(Mouse::Botton::RIGHT)) mesh.pos.x -= 5.0f;


	if (mouse.ReleaseTrigger(Mouse::Botton::LEFT)) mesh.pos.y += 5.0f;
	if (mouse.ReleaseTrigger(Mouse::Botton::RIGHT)) mesh.pos.y -= 5.0f;

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

void SceneTitle::holler()
{
	int rng = rand() % 1000 - 30;
	int rng2 = rand() % 1000;

	player.pos.y = 28;
	switch (holler_state%2)
	{
	case 0:
		if (!holler_timer)
		{
			//audio.StopSound(title_bgm);
			audio.PlaySounds(title_bgm);
		}
		holler_timer++;
		player.pos.z = -42;
		if (rng < 0)
			p_light = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		else
			p_light = DirectX::XMFLOAT4(-0.4f, -0.8f, 0.1f, 0.0f);

		if (holler_timer > 400)
		{
			holler_state++;
			holler_timer = 0;
		}
		break;
	case 1:
		holler_timer++;
		p_light = DirectX::XMFLOAT4(-0.4f, -0.8f, 0.1f, 0.0f);

		if (rng2<1)
		{
			player.pos.z = -48;
			audio.StopSound(warp_se);
			audio.PlaySounds(warp_se);
		}
		else if(rng2<10)
		{
			player.pos.z = -1 * rand() % 30;
		}

		if (holler_timer > 2000)
		{
			holler_state++;
			holler_timer = 0;
		}
		break;
	}
}