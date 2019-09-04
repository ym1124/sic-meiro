#include "..\Header\stage.h"
#include "..\Header\trap.h"
#include "..\Header\player.h"



int Stage::brock_type = NOHIT;
int Stage::elec_time = 0;

extern Gate gate;
extern PointLight point_light[LIGHT_MAX];
Ground ground;
extern Stage stage;
extern Player player;

void Stage::Init() {

	damage_floar_flg = false;
	brock_type = NOHIT;
	elec_time = 0;

	int num = 0;

	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WEIGHT; j++)
		{
			move_flg[i][j] = false;
			elec_flg[i][j] = false;
			elec_timer[i][j] = 0;

			obj[i][j].Initialize();
			obj[i][j].Load("FBX/001_cube.fbx");

			//ポジション
			obj[i][j].pos.x = j * 2.0f;
			obj[i][j].pos.y = 1.0f;
			obj[i][j].pos.z = i * -2.0f;
		}
	}

}

void Stage::Update() {
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WEIGHT; j++)
		{
			if (elec_timer[i][j] >= 1)
			{
				elec_timer[i][j]--;

				if (stage[i + 1][j] == GATE_FLG && elec_timer[i][j] == 0)
				{
					//gate.elec_power++;
					gate.power_charge_flg = true;
				}
				else if (stage[i - 1][j] == GATE_FLG && elec_timer[i][j] == 0)
				{
					//gate.elec_power++;
					gate.power_charge_flg = true;
				}
				else if (stage[i][j + 1] == GATE_FLG && elec_timer[i][j] == 0)
				{
					//gate.elec_power++;
					gate.power_charge_flg = true;
				}
				else if (stage[i][j - 1] == GATE_FLG && elec_timer[i][j] == 0)
				{
					//gate.elec_power++;
					gate.power_charge_flg = true;
				}

			}

		}
	}

}

void Stage::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir)	//	描画関数
{

	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WEIGHT; j++)
		{
			switch (stage[i][j])
			{
			case WALL:
				obj[i][j].color = { 1,0,0,1 };
				obj[i][j].Render(view, projection, light_dir, 0, light.pos, 2, light);
				break;
			case ELECTRIC:
				if (!elec_flg[i][j])
				{
					obj[i][j].color = { 0,1,0,1 };
					obj[i][j].Render(view, projection, light_dir, 0, light.pos, 2, light);

				}
				else if (elec_flg[i][j] && elec_timer[i][j] > 0)
				{
					obj[i][j].color = { 0,1,0,1 };
					obj[i][j].Render(view, projection, light_dir, 0, light.pos, 2, light);
				}

				else if (elec_flg[i][j] && elec_timer[i][j] <= 0)
				{
					obj[i][j].color = { 1,1,0,1 };
					obj[i][j].Render(view, projection, light_dir, 1, light.pos, 2, light);
				}

				break;
			case ELEC_DOWN_FLOAR:
				if (move_flg[i][j]) 
				{
					obj[i][j].color = { 0,0,1,1 };
					obj[i][j].Render(view, projection, light_dir, 0, light.pos, 2, light);
				}
				break;
			case MOVE:
				obj[i][j].color = { 0,0,1,1 };
				obj[i][j].Render(view, projection, light_dir, 0, light.pos, 2, light);
				break;
			case GATE_FLG:
				obj[i][j].color = { 0,0,0,1 };
				obj[i][j].Render(view, projection, light_dir, 0, light.pos, 2, light);
			case LIGHT:
				obj[i][j].color = { 0,0,0,1 };
				obj[i][j].Render(view, projection, light_dir, 0, light.pos, 2, light);
			}
		}
	}
}

void Stage::Release() {
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WEIGHT; j++)
		{
			obj[i][j].Release();
		}
	}
}

int Stage::HitCheck(const DirectX::XMFLOAT3 check_pos) {
	static int check_num = 0;
	static int column = 0;
	static int low = 0;

	column = (int)-check_pos.z / CHIP_SIZE;
	low = (int)check_pos.x / CHIP_SIZE;

	check_num = stage[column][low];

	if (column < 0 || low < 0 || column >9 || low >9)
	{
		check_num = STAGE_OUT;
		return STAGE_OUT;

	}
	switch (check_num)
	{
	case NOHIT:
		return NOHIT;
	case WALL:
		return WALL;
	case ELECTRIC:
		return ELECTRIC;
	case MOVE:
		return MOVE;
	case GATE_FLG:
		return GATE_FLG;
	case GATE:
		return GATE;
	case GATE_HIT:
		return GATE_HIT;
	case LIGHT:
		return LIGHT;
	case ELEC_DOWN_FLOAR:
		return ELEC_DOWN_FLOAR;
	case GOAL:
		return GOAL;
	default:
		return STAGE_OUT;
	}
}






void Stage::BrockMove(const DirectX::XMFLOAT3 brock_pos, bool flg) {//プレイヤーがブロックを動かす

	if (!flg) return;

	static bool move_num;
	move_num = move_flg[(int)-brock_pos.z / CHIP_SIZE][(int)brock_pos.x / CHIP_SIZE];
	if (move_num != true)
	{
		for (int i = 0; i < STAGE_HEIGHT; i++)
		{
			for (int j = 0; j < STAGE_WEIGHT; j++)
			{
				if (move_flg[i][j] == true)
				{
					move_flg[i][j] = false;
					if (stage[i][j] != ELEC_DOWN_FLOAR)
					{
						if (damage_floar_flg) 
						{
							stage[i][j] = ELEC_DOWN_FLOAR;
							damage_floar_flg = false;
						}
						else 
						{
							stage[i][j] = NOHIT;
						}
					}

					break;
				}
			}
		}
		move_flg[(int)-brock_pos.z / CHIP_SIZE][(int)brock_pos.x / CHIP_SIZE] = true;

		if (stage[(int)-brock_pos.z / CHIP_SIZE][(int)brock_pos.x / CHIP_SIZE] != ELEC_DOWN_FLOAR)
		{
			stage[(int)-brock_pos.z / CHIP_SIZE][(int)brock_pos.x / CHIP_SIZE] = MOVE;
		}
	}
}

bool Stage::CheckBrockElec(const DirectX::XMFLOAT3 brock_pos, DirectX::XMFLOAT3 scale) {//プレイヤーがブロックを動かす

	DirectX::XMFLOAT3 _shadow_pos[4] = {};

	_shadow_pos[0] = { brock_pos.x - scale.x*2.0f,brock_pos.y,brock_pos.z };
	_shadow_pos[1] = { brock_pos.x + scale.x*2.0f,brock_pos.y,brock_pos.z };
	_shadow_pos[2] = { brock_pos.x,brock_pos.y,brock_pos.z + scale.z*2.0f };
	_shadow_pos[3] = { brock_pos.x,brock_pos.y,brock_pos.z - scale.z*2.0f };

	int elec_num = 0;//何個接したか

	for (int i = 0; i < 4; i++)
	{
		if (HitCheck(_shadow_pos[i]) == ELECTRIC &&
			!elec_flg[(int)-_shadow_pos[i].z / CHIP_SIZE][(int)_shadow_pos[i].x / CHIP_SIZE])
		{


			elec_flg[(int)-_shadow_pos[i].z / CHIP_SIZE][(int)_shadow_pos[i].x / CHIP_SIZE] = true;
			elec_num++;
			elec_timer[(int)-_shadow_pos[i].z / CHIP_SIZE][(int)_shadow_pos[i].x / CHIP_SIZE] = elec_time;
			elec_time++;
			CheckBrockElec(_shadow_pos[i], scale);


		}
	}
	if (elec_num == 0)
	{
		//gate.power_charge_flg = false;
		return false;
	}
	else
	{
		return true;
	}
}


//地面--------------------------------------------------------
void Ground::Init() {



	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WEIGHT; j++)
		{
			obj[i][j].Initialize();
			obj[i][j].Load("FBX/001_cube.fbx");

			if (stage.stage[i][j] == ELEC_DOWN_FLOAR)
			{
				obj[i][j].color = { 1,0,0,1 };
			}
			else
			{
				obj[i][j].color = { 1,1,1,1 };
			}

			obj[i][j].pos.x = j * 2.0f;
			obj[i][j].pos.y = -1.0f;
			obj[i][j].pos.z = i * -2.0f;

		}
	}
}

void Ground::Update() {



}

void Ground::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir)	//	描画関数
{
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WEIGHT; j++)
		{

			obj[i][j].Render(view, projection, light_dir, 0, light.pos, 2, light);
		}
	}
}

void Ground::Release() {
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WEIGHT; j++)
		{
			obj[i][j].Release();
		}
	}
}

