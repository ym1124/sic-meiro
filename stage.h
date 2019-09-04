#pragma once
#include	"..\Header\Mesh.h"
#include	"..\Header\trap.h"

#include	<DirectXMath.h>

#define STAGE_WEIGHT 10
#define STAGE_HEIGHT 10

#define CHIP_SIZE 2

//フラグがオフでstage配列が5(GATE)または6(GATE_HIT)以外の時
#define GATE_ON (gate.start_up_flg&&(stage.HitCheck(pos)== GATE||stage.HitCheck(pos)== GATE_HIT))



enum StageHit
{
	STAGE_OUT = -1,//画面外
	NOHIT,//あたってない
	WALL,//非通電
	ELECTRIC,//通電

	GATE_FLG,//ゲートONにするギミックのポジション
	//4
	GATE,//ゲートを置く位置
	GATE_HIT,//ゲートの当たり判定

	//6
	GATE2,//ゲートを置く位置
	GATE_HIT2,//ゲートの当たり判定
	//8
	GATE3,
	GATE_HIT3,

	LIGHT,
	ELEC_DOWN_FLOAR,//電力下がる床
	GOAL,//ゴール







	MOVE,//ブロックを動かしてるとき
};

//ステージ
class Stage
{
public:

	static int brock_type;
	static int elec_time;

	bool damage_floar_flg;
	int light_ID[STAGE_HEIGHT][STAGE_WEIGHT];

	char stage[STAGE_HEIGHT][STAGE_WEIGHT] =
	{
		{ 0,2,2,2,2,2,2,2,2,2 },
		{ 0,0,0,2,0,3,0,1,0,1 },
		{ 0,0,0,2,0,0,1,1,0,1 },
		{ 1,0,0,2,0,0,0,1,0,1 },
		{ 1,0,4,5,5,0,0,0,0,1 },
		{ 1,0,10,0,0,0,0,0,0,1 },
		{ 1,0,0,1,0,0,1,0,0,1 },
		{ 1,11,11,0,1,1,0,6,0,1 },
		{ 1,0,0,0,0,0,0,0,0,12 },
		{ 1,1,1,1,1,1,1,1,1,1 },
	};

	bool move_flg[STAGE_HEIGHT][STAGE_WEIGHT];//ブロック移動

	bool elec_flg[STAGE_HEIGHT][STAGE_WEIGHT];//通電
	int elec_timer[STAGE_HEIGHT][STAGE_WEIGHT];//通電までの時間

	DirectX::XMFLOAT3 pos;



	MyMesh obj[STAGE_HEIGHT][STAGE_WEIGHT];
	void Init();
	void Update();
	void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir);	//	描画関数
	void Release();

	int HitCheck(const DirectX::XMFLOAT3 check_pos);

	void BrockMove(const DirectX::XMFLOAT3 brock_pos, bool flg);

	bool CheckBrockElec(const DirectX::XMFLOAT3 brock_pos, DirectX::XMFLOAT3 scale);


};
//地面
class Ground
{
public:

	DirectX::XMFLOAT3 pos;

	MyMesh obj[STAGE_HEIGHT][STAGE_WEIGHT];
	void Init();
	void Update();
	void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir);	//	描画関数
	void Release();


};

