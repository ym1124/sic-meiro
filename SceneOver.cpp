#include "..\Header\SceneOver.h"
#include "..\Header\Input.h"
#include "..\Header\debug.h"
#include "..\Header\Light.h"

Light over_light;

void SceneOver::init()
{
	over.Initialize();
	over.Load("Sa_fbx/gameover.fbx");
	over.pos = { -35.0f,0.0f,0.0f };
	over.angle = { 0.0f,-10.0f*TO_RADIAN,0.0f };

	cameraInit(DirectX::XMFLOAT4(0, -1, 1, 0));

	push.Initialize();
	push.Load("Sa_fbx/push_enter.fbx");
	push.angle = { 0.0f,0.0f*TO_RADIAN,0.0f };
	push.scale = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
	push.pos.x = 0.0f;
	push.pos.y = -15.0f;
	push.pos.z = -5.0f;

	over_light.pos = DirectX::XMFLOAT3(0.0f, 3.0f, -10.0f);
	over_light.attenution = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	over_light.color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	camera.position.z = -20.0f;
}

void SceneOver::update()
{
	push.angle.y += 1.0f*TO_RADIAN;

	if (kb.PushTrigger(VK_RETURN) || pad.PushTrigger(VK_PAD_A, GamePad::PlayerID::_1P)) {
		release();
		scene = SCENE_TITLE;
		return;
	}
}

void SceneOver::render()
{
	//	ÉrÉÖÅ[ïœä∑çsóÒ
	view = camera.GetViewMatrix();

	blender::Set(blender::NONE, FRAMEWORK.getContext());

	//over.Render(view, projection, light_direction,0, over_light.pos, 1, over_light);

	//push.Render(view, projection, light_direction,0, over_light.pos, 1, over_light);
}

void SceneOver::release()
{
	over.Release();
	push.Release();
}