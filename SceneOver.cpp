#include "..\Header\SceneOver.h"
#include "..\Header\Input.h"
#include "..\Header\debug.h"
#include "..\Header\Light.h"
#include "..\Header\Sound.h"

Light over_light;
Player over_player;

void SceneOver::init()
{
	over.Initialize();
	over.Load("Sa_fbx/gameover.fbx");
	over.pos = { -30.0f,-15.0f,60.0f };
	over.color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	over.scale = DirectX::XMFLOAT3(0.8f, 0.8f,0.8f);

	over_player.Initialize("Sa_fbx/body.fbx");
	over_player.obj.color = DirectX::XMFLOAT4(1.0f, .0f, .0f, 1.0f);
	over_player.tail.color= DirectX::XMFLOAT4(1.0f, .0f, .0f, 1.0f);
	over_player.head.color = DirectX::XMFLOAT4(1.0f, .0f, .0f, 1.0f);
	over_player.pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	over_bgm=audio.LoadSound(L"Resources/Sounds/terror_clock (online-audio-converter.com).wav",0);

	cameraInit(DirectX::XMFLOAT4(0, -1, 1, 0));

	camera.position.x = 0.0f;
	camera.position.y = 0.0f;
	camera.position.z = -20.0f;
	camera.point_view = over_player.pos;
}

void SceneOver::update()
{
	audio.PlaySounds(over_bgm);
	push.angle.y += 1.0f*TO_RADIAN;

	if (kb.PushTrigger(VK_RETURN) || pad.PushTrigger(VK_PAD_A, GamePad::PlayerID::_1P)) {
		release();
		scene = SCENE_TITLE;
		audio.StopSound(over_bgm);
		return;
	}
}

void SceneOver::render()
{
	//	ÉrÉÖÅ[ïœä∑çsóÒ
	view = camera.GetViewMatrix();

	blender::Set(blender::NONE, FRAMEWORK.getContext());

	over.Render(view, projection, light_direction,2, over_light.pos, 1, over_light);
	over_player.Render(view, projection, DirectX::XMFLOAT4(0.0f,0.8f,0.2f, 0.0f));

	//push.Render(view, projection, light_direction,0, over_light.pos, 1, over_light);
}

void SceneOver::release()
{
	over.Release();
	push.Release();
	over_player.Release();
	audio.ReleaseSound(over_bgm);
}