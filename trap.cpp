#include "..\Header\trap.h"
#include "..\Header\stage.h"
#include "..\Header\Easing.h"
#include "..\Header\player.h"

#include "..\Header\debug.h"

#include "..\Header\Input.h"

#define ELEC_MAX 100

extern Trap trap;

Lift lift;
Gate gate;
extern Player player;
extern Stage stage;
PointLight point_light[LIGHT_MAX];

void Trap::Init()
{
	gate_flg = false;
	lift_flg = false;
	//lift.Init();
	gate.Init();
}

void Trap::Update()
{
	//lift.Update();
	gate.Update();
}

void Trap::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, const DirectX::XMFLOAT3& pos, const int light_num, const Light &light)
{
	//lift.Render(view, projection, light_dir, pos, light_num, light);
	gate.Render(view, projection, light_dir, pos, light_num, light);

}

void Trap::Release()
{
	//lift.Release();
	gate.Release();

}

//------------------------------------------------------------------------------------------------------------
void Lift::Init()
{
	pos = { 0,0,0 };
	obj.Initialize();
	obj.Load("FBX/001_cube.fbx");
	obj.scale = { 2,1,2 };
	timer = 0;
	max_pos = 5;
}
void Lift::Update()
{

	if (!trap.lift_flg)return;
	pos.y = pEasing->Ease(_IN, SMOOTH, timer, 0, max_pos, 6);
	//if (speed >= 0)
	//{
	//
	//	if (max_pos_y / 2 > pos.y)
	//	{
	//		pos.y += speed*speed;
	//		speed += accel;
	//	}
	//	else
	//	{
	//		pos.y += speed*speed;
	//		speed -= accel;
	//	}
	//}
	timer += 0.1f;
}

void Lift::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, const DirectX::XMFLOAT3& pos, const int light_num, const Light &light)
{
	obj.pos = this->pos;
	obj.angle.y = this->angle;
	obj.Render(view, projection, light_dir,0, light.pos, 2, light);
}

void Lift::Release()
{

	obj.Release();
}

//ÉQÅ[Ég------------------------------------------------------------
void Gate::Init()
{
	switch_flg = false;
	power_charge_flg = false;
	start_up_flg = false;
	elec_power = 0;

	for (int i = 0; i < 3; i++)
	{

		obj[i].Initialize();
		obj[i].Load("FBX/001_cube.fbx");
		obj[i].scale = { 3, 2, 1 };
	}
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WEIGHT; j++)
		{

			if (stage.stage[i][j] == 5)
			{
				pos_check_x = (float)j;
				pos_check_z = (float)i;
			}
		}
	}
	pos[0] = { pos_check_x * (CHIP_SIZE + 1), -3, -pos_check_z * CHIP_SIZE };
	pos[1] = { pos_check_x * (CHIP_SIZE + 1), -3, -pos_check_z * CHIP_SIZE };
	pos[2] = { pos_check_x * (CHIP_SIZE + 1), -3, -pos_check_z * CHIP_SIZE };

	timer = 0;
	max_pos = 3;
	min_pos = -3;
	start_pos[0] = pos[0].y;
	start_pos[1] = pos[1].y;
	start_pos[2] = pos[2].y;

}

void Gate::Update()
{

	if (switch_flg)
	{
		pos[0].y = pEasing->Ease(_OUT, SMOOTH, timer, start_pos[0], min_pos, 6);
	}
	else
	{
		pos[0].y = pEasing->Ease(_OUT, SMOOTH, timer, start_pos[0], max_pos, 6);

	}

	//if (speed >= 0)
	//{
	//
	//	if (max_pos_y / 2 > pos.y)
	//	{
	//		pos.y += speed*speed;
	//		speed += accel;
	//	}
	//	else
	//	{
	//		pos.y += speed*speed;
	//		speed -= accel;
	//	}
	//}

	timer += 0.1f;
}

void Gate::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, const DirectX::XMFLOAT3& pos, const int light_num, const Light &light)
{
	for (int i = 0; i < 3; i++)
	{
		obj[i].pos = this->pos[i];
		obj[i].angle.y = this->angle[i];
		obj[i].Render(view, projection, light_dir, 0, light.pos, 2, light);
	}

	DrawString(100, 100, { 255,255,255 }, L"chargeflg:%d", power_charge_flg);
	DrawString(100, 200, { 255,255,255 }, L"elecpower:%d", elec_power);
	DrawString(100, 300, { 255,255,255 }, L"etimer:%d", player.e_timer);

}

void Gate::Release()
{
	for (int i = 0; i < 3; i++)
	{
		obj[i].Release();
	}
}