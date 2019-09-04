#include "..\Header\SceneSelect.h"
#include "..\Header\framework.h"
#include "..\Header\debug.h"

void SceneSelect::init()
{
	cameraInit(DirectX::XMFLOAT4(0, -1, 1, 0));
	//
	//clear_light.pos = DirectX::XMFLOAT3(0.0f, 3.0f, -10.0f);
	//clear_light.attenution = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	//clear_light.color = DirectX::XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);
	//
	camera.position.z = -100.0f;
}

void SceneSelect::update()
{
	if (kb.PushTrigger(VK_RETURN) || pad.PushTrigger(VK_PAD_A, GamePad::PlayerID::_1P)) {
		release();
		now_stage = 1;
		scene = SCENE_GAME;
		return;
	}
}

void SceneSelect::render()
{
	//	ÉrÉÖÅ[ïœä∑çsóÒ
	view = camera.GetViewMatrix();

	blender::Set(blender::NONE, FRAMEWORK.getContext());

	//mesh.Render(view, projection, light_direction, clear_light.pos, 1, clear_light);

	//push.Render(view, projection, light_direction, clear_light.pos, 1, clear_light);

	//SetFontSize(32);
	//DrawString(500, 500, { 255,255,255 }, L"push enter");
	//InitFontSize();
}

void SceneSelect::release()
{
	//mesh.Release();
	//push.Release();
}