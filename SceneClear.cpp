#include "..\Header\SceneClear.h"
#include "..\Header\framework.h"
#include "..\Header\debug.h"
#include "..\Header\Light.h"

Light clear_light;

void SceneClear::init()
{
	cameraInit(DirectX::XMFLOAT4(0, -1, 1, 0));
	camera.position = DirectX::XMFLOAT3(0.0f,0.0f,-30.0f);
	render_timer = 0;
	players[0].Initialize("Sa_fbx/body.fbx");
	players[0].obj.color = DirectX::XMFLOAT4(1.0f, .0f, .0f, 1.0f);
	players[0].head.color = DirectX::XMFLOAT4(1.0f, .0f, 0.0f, 1.0f);
	players[0].tail.color = DirectX::XMFLOAT4(1.0f, .0f, 0.0f, 1.0f);
	players[0].pos = DirectX::XMFLOAT3(0.0f, 0.0f,-40.0f);
	players[0].angle = DirectX::XMConvertToRadians(.0f);
	for (int i = 1; i < 30; i++)
	{
		players[i].Initialize("Sa_fbx/body.fbx");
		players[i].obj.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		players[i].head.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		players[i].tail.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		players[i].pos = DirectX::XMFLOAT3(rand() % 61 - 30, rand() % 61 - 30, 0.0f);
		players[i].angle = DirectX::XMConvertToRadians(.0f);
	}
}

void SceneClear::update()
{
	//push.angle.y += 1.0f*TO_RADIAN;

	for (int i = 0; i < 30; i++)
	{
		players[i].pos.z += 0.1f;
	}

	if (kb.PushTrigger(VK_RETURN) || pad.PushTrigger(VK_PAD_A, GamePad::PlayerID::_1P)) {
		release();
		scene = SCENE_TITLE;
		return;
	}
}

void SceneClear::render()
{
	//	ƒrƒ…[•ÏŠ·s—ñ
	view = camera.GetViewMatrix();

	blender::Set(blender::NONE, FRAMEWORK.getContext());

	render_timer++;

	int i = render_timer / 60;
	for ( int j = 0; j < 30; j++)
	{
		if(j<=i)
			players[j].Render(view, projection, DirectX::XMFLOAT4(-1.0f, -0.9f, 0.9f, 0.0f));
	}
}

void SceneClear::release()
{
	render_timer = 0;
	for (int i = 0; i < 30; i++)
	{
		players[i].Release();
	}
	//mesh.Release();
	//push.Release();
}