#include	"..\Header\player.h"

#include	"..\Header\shot.h"
#include	"..\Header\particle.h"

#include "..\Header\Light.h"
#include "..\Header\Input.h"
#include "..\Header\stage.h"
#include "..\Header\trap.h"
#include "..\Header\scene.h"



extern Stage stage;
extern Trap trap;
extern Gate gate;
extern PointLight point_light[LIGHT_MAX];

/*******************************************************************************
	「プレイヤー」クラスの初期化
*******************************************************************************/
void Player::Initialize(const char* filename)
{

	brock_move_flg = false;
	brock_elec_flg = false;
	goal_flg = false;

	electric = HP_MAX;
	elect_power = 0.04f;

	obj.Initialize();
	obj.Load(filename);

	head.Initialize();
	head.Load("Sa_fbx/head.fbx");
	tail.Initialize();
	tail.Load("Sa_fbx/tail.fbx");

	obj.scale = head.scale = tail.scale = { 0.1f,0.1f,0.1f };

	for (int i = 0; i < 4; i++) {
		shadow[i].Initialize();
		shadow[i].Load("FBX/001_cube.fbx");
		shadow[i].color = { 0.5f,obj.color.y,obj.color.z,0.2f };
	}

	shadow_timer = 0.0f;

	shadow[0].pos = { this->pos.x - 1.0f*2.0f,this->pos.y,this->pos.z };
	shadow[1].pos = { this->pos.x + 1.0f*2.0f,this->pos.y,this->pos.z };
	shadow[2].pos = { this->pos.x,this->pos.y,this->pos.z + 1.0f*2.0f };
	shadow[3].pos = { this->pos.x,this->pos.y,this->pos.z - 1.0f*2.0f };

	p = 0;
	angle = DirectX::XMConvertToRadians(180.0f);

	if (move_se < 0) {
		move_se = audio.LoadSound(L"Resources/Sounds/回避１.wav", 0);
		electric_se = audio.LoadSound(L"Resources/Sounds/縮小.wav", 0);
		catch_se = audio.LoadSound(L"Resources/Sounds/巨大化１.wav", 0);
		rele_se = audio.LoadSound(L"Resources/Sounds/巨大化２.wav", 0);
		no_move_se = audio.LoadSound(L"Resources/Sounds/被ダメ.wav", 0);
	}
}



/*******************************************************************************
	「プレイヤー」クラスの解放
*******************************************************************************/
void Player::Release()
{
	obj.Release();
	head.Release();
	tail.Release();

	audio.ReleaseSound(move_se);
	audio.ReleaseSound(electric_se);
	audio.ReleaseSound(catch_se);
	audio.ReleaseSound(rele_se);
	audio.ReleaseSound(no_move_se);

	for (int i = 0; i < 4; i++)
	{
		shadow[i].Release();
	}
}



/*******************************************************************************
	「プレイヤー」クラスの描画
*******************************************************************************/
void Player::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir,int renderType)
{
	shadow_timer += DirectX::XMConvertToRadians(3.0f);

	obj.pos = head.pos = tail.pos = this->pos;
	obj.pos.y = head.pos.y = tail.pos.y = sinf(shadow_timer)*0.5f + 1.5f;
	obj.angle.y = angle;
	head.angle.y += angle + DirectX::XMConvertToRadians(3.0f);
	tail.angle.y = angle;
	tail.angle.z += DirectX::XMConvertToRadians(3.0f);

	float time = 0.0f;
	/*obj.color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	head.color = DirectX::XMFLOAT4(1.0f, .0f, 0.0f, 1.0f);
	tail.color = DirectX::XMFLOAT4(1.0f, .0f, 0.0f, 1.0f);*/

	if (!renderType)
	{
		obj.Render(view, projection, light_dir, 0.0f);
		head.Render(view, projection, light_dir, 0.0f);
		tail.Render(view, projection, light_dir, 0.0f);
	}

	if (renderType!=0)
	{
		obj.Render(view, projection, light_dir, 2);
		head.Render(view, projection, light_dir, 2);
		tail.Render(view, projection, light_dir, 2);
	}

	/*for (int i = 0; i < 4; i++) {
		if (stage.HitCheck(shadow[i].pos) != NOHIT&&stage.HitCheck(shadow[i].pos) != STAGE_OUT) continue;
		if (i == p&&brock_move_flg) continue;

		shadow[i].color.w = sinf(shadow_timer)*0.4f + 0.2f;
		shadow[i].Render(view, projection, light_dir, 0.0f);
	}*/
}


/*******************************************************************************
	「プレイヤー」クラスの移動
*******************************************************************************/
void Player::Move()
{
	stage.Update();
	//体力の上限は100
	if (electric >= HP_MAX)
	{
		electric = HP_MAX;
	}

	const float dangle = DirectX::XMConvertToRadians(1.0f);		//	1度
	const float speed = 0.05f;									//	プレイヤーの速度

	static int timer = 0;

	DirectX::XMFLOAT3 _pos = pos;
	DirectX::XMFLOAT3 _shadow_pos[4] = {};

	_shadow_pos[0] = shadow[0].pos;
	_shadow_pos[1] = shadow[1].pos;
	_shadow_pos[2] = shadow[2].pos;
	_shadow_pos[3] = shadow[3].pos;


	if (kb.PushTrigger(VK_LEFT) || kb.PushTrigger(VK_RIGHT) || kb.PushTrigger(VK_UP) || kb.PushTrigger(VK_DOWN) ||
		pad.PushTrigger(XINPUT_GAMEPAD_DPAD_LEFT, GamePad::PlayerID::_1P) ||
		pad.PushTrigger(XINPUT_GAMEPAD_DPAD_RIGHT, GamePad::PlayerID::_1P) ||
		pad.PushTrigger(XINPUT_GAMEPAD_DPAD_UP, GamePad::PlayerID::_1P) ||
		pad.PushTrigger(XINPUT_GAMEPAD_DPAD_DOWN, GamePad::PlayerID::_1P))
	{
		electric -= elect_power;
		shadow_timer = 0.0f;
		timer = 10;
		audio.StopSound(move_se);
		audio.PlaySounds(move_se);
	}

	if (kb.PushTrigger(VK_LEFT) || pad.PushTrigger(XINPUT_GAMEPAD_DPAD_LEFT, GamePad::PlayerID::_1P)) {
		pos.x -= 1.0f*2.0f;
		angle = DirectX::XMConvertToRadians(270.0f);
	}

	if (kb.PushTrigger(VK_RIGHT) || pad.PushTrigger(XINPUT_GAMEPAD_DPAD_RIGHT, GamePad::PlayerID::_1P)) {
		pos.x += 1.0f*2.0f;
		angle = DirectX::XMConvertToRadians(90.0f);
	}

	if (kb.PushTrigger(VK_UP) || pad.PushTrigger(XINPUT_GAMEPAD_DPAD_UP, GamePad::PlayerID::_1P))
	{
		pos.z += 1.0f*2.0f;
		angle = DirectX::XMConvertToRadians(0.0f);
	}

	if (kb.PushTrigger(VK_DOWN) || pad.PushTrigger(XINPUT_GAMEPAD_DPAD_DOWN, GamePad::PlayerID::_1P))
	{
		pos.z -= 1.0f*2.0f;
		angle = DirectX::XMConvertToRadians(180.0f);
	}

	shadow[0].pos = { this->pos.x - 1.0f*2.0f,this->pos.y,this->pos.z };
	shadow[1].pos = { this->pos.x + 1.0f*2.0f,this->pos.y,this->pos.z };
	shadow[2].pos = { this->pos.x,this->pos.y,this->pos.z + 1.0f*2.0f };
	shadow[3].pos = { this->pos.x,this->pos.y,this->pos.z - 1.0f*2.0f };

	//ゴールの処理------------------------------------------------------
	if (stage.HitCheck(pos) == GOAL)
	{
		goal_flg = true;
	}

	//電力下がる床------------------------------------------------------
	if (stage.HitCheck(pos) == ELEC_DOWN_FLOAR)
	{
		electric -= FLOAR_DAMAGE;
	}



	if (stage.HitCheck(pos) != NOHIT && stage.HitCheck(pos) != MOVE &&
		stage.HitCheck(pos) != GOAL && stage.HitCheck(pos) != ELEC_DOWN_FLOAR)
	{
		if (GATE_ON)
		{
			//ゲートがONの時は通れる
		}
		else
		{
			pos = _pos;
			shadow[0].pos = _shadow_pos[0];
			shadow[1].pos = _shadow_pos[1];
			shadow[2].pos = _shadow_pos[2];
			shadow[3].pos = _shadow_pos[3];
			audio.StopSound(no_move_se);
			audio.PlaySounds(no_move_se);
		}
	}
	//ゲートが上がった時にプレイヤーと被った処理
	if (gate.elec_power <= 1 && (stage.HitCheck(pos) == GATE || stage.HitCheck(pos) == GATE_HIT))
	{
		gate.elec_power = 1;
	}
	if (brock_elec_flg)
	{
		pos = _pos;
		shadow[0].pos = _shadow_pos[0];
		shadow[1].pos = _shadow_pos[1];
		shadow[2].pos = _shadow_pos[2];
		shadow[3].pos = _shadow_pos[3];

	}

	if (brock_move_flg)
	{
		if (stage.HitCheck(shadow[p].pos) != NOHIT&&stage.HitCheck(shadow[p].pos) != ELEC_DOWN_FLOAR)//当たっていた場合
		{
			pos = _pos;
			shadow[0].pos = _shadow_pos[0];
			shadow[1].pos = _shadow_pos[1];
			shadow[2].pos = _shadow_pos[2];
			shadow[3].pos = _shadow_pos[3];
		}
	}

	DirectX::XMFLOAT3 check_pos = { this->pos.x + sinf(angle)*2.0f + 0.001f,this->pos.y,this->pos.z + cosf(angle)*2.0f };
	if (stage.HitCheck(check_pos) != NOHIT&&stage.HitCheck(check_pos) != STAGE_OUT&&
		stage.HitCheck(check_pos) != GATE&&stage.HitCheck(check_pos) != GATE_HIT&&
		stage.HitCheck(check_pos) != WALL&&stage.HitCheck(check_pos) != GOAL)//当たっていた場合
	{

		//ブロック動かす処理--------------------------------------------------------
		if (kb.PushTrigger('A') && !brock_move_flg)
		{
			audio.StopSound(catch_se);
			audio.PlaySounds(catch_se);

			//ブロックのタイプを保存
			stage.brock_type = stage.stage[(int)-check_pos.z / CHIP_SIZE][(int)check_pos.x / CHIP_SIZE];


			stage.move_flg[(int)-check_pos.z / CHIP_SIZE][(int)check_pos.x / CHIP_SIZE] = true;
			//ブロックのタイプをMOVEに上書き
			stage.stage[(int)-check_pos.z / CHIP_SIZE][(int)check_pos.x / CHIP_SIZE] = MOVE;

			brock_move_flg = true;
			switch (static_cast<int>(DirectX::XMConvertToDegrees(angle)))
			{
			case 0:
				p = 2;
				break;

			case 90:
				p = 1;
				break;

			case 180:
				p = 3;
				break;

			case 270:
				p = 0;
				break;
			}
		}


		//通電処理----------------------------------------------------------------
		if (kb.Push('E')) //E長押し
		{
			if (stage.stage[(int)-check_pos.z / CHIP_SIZE][(int)check_pos.x / CHIP_SIZE] == LIGHT)
			{
				//体力回復
				electric += LIGHT_HEAL;

			}
			brock_elec_flg = true;
			audio.StopSound(electric_se);
			audio.PlaySounds(electric_se);

			if (stage.stage[(int)-check_pos.z / CHIP_SIZE][(int)check_pos.x / CHIP_SIZE] == GATE_FLG)
			{
				stage.CheckBrockElec(check_pos, { 1.0f,1.0f,1.0f });
				electric -= ELECTRIC_DAMAGE;
			}
			//ブロックが通電だった場合
			if (stage.stage[(int)-check_pos.z / CHIP_SIZE][(int)check_pos.x / CHIP_SIZE] == ELECTRIC)
			{
				//elecflgをtrueにする
				stage.elec_flg[(int)-check_pos.z / CHIP_SIZE][(int)check_pos.x / CHIP_SIZE] = true;
				stage.CheckBrockElec(check_pos, { 1.0f,1.0f,1.0f });
				electric -= ELECTRIC_DAMAGE;
			}
		}
		else
		{
			stage.elec_time = 0;
			gate.power_charge_flg = false;
			brock_elec_flg = false;
			for (int i = 0; i < STAGE_HEIGHT; i++)
			{
				for (int j = 0; j < STAGE_WEIGHT; j++)
				{
					stage.elec_flg[i][j] = false;
				}
			}
		}
	}

	if (kb.PushTrigger('B') && brock_move_flg)
	{
		stage.move_flg[(int)-shadow[p].pos.z / CHIP_SIZE][(int)shadow[p].pos.x / CHIP_SIZE] = false;
		//brockを戻す
		if (stage.stage[(int)-shadow[p].pos.z / CHIP_SIZE][(int)shadow[p].pos.x / CHIP_SIZE] == ELEC_DOWN_FLOAR) 
		{
			stage.damage_floar_flg = true;
		}
		
		stage.stage[(int)-shadow[p].pos.z / CHIP_SIZE][(int)shadow[p].pos.x / CHIP_SIZE] = Stage::brock_type;

		brock_move_flg = false;
		audio.StopSound(rele_se);
		audio.PlaySounds(rele_se);
	}

	stage.BrockMove(shadow[p].pos, brock_move_flg);

}