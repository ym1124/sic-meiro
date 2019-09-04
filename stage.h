#pragma once
#include	"..\Header\Mesh.h"
#include	"..\Header\trap.h"

#include	<DirectXMath.h>

#define STAGE_WEIGHT 10
#define STAGE_HEIGHT 10

#define CHIP_SIZE 2

//�t���O���I�t��stage�z��5(GATE)�܂���6(GATE_HIT)�ȊO�̎�
#define GATE_ON (gate.start_up_flg&&(stage.HitCheck(pos)== GATE||stage.HitCheck(pos)== GATE_HIT))



enum StageHit
{
	STAGE_OUT = -1,//��ʊO
	NOHIT,//�������ĂȂ�
	WALL,//��ʓd
	ELECTRIC,//�ʓd

	GATE_FLG,//�Q�[�gON�ɂ���M�~�b�N�̃|�W�V����
	//4
	GATE,//�Q�[�g��u���ʒu
	GATE_HIT,//�Q�[�g�̓����蔻��

	//6
	GATE2,//�Q�[�g��u���ʒu
	GATE_HIT2,//�Q�[�g�̓����蔻��
	//8
	GATE3,
	GATE_HIT3,

	LIGHT,
	ELEC_DOWN_FLOAR,//�d�͉����鏰
	GOAL,//�S�[��







	MOVE,//�u���b�N�𓮂����Ă�Ƃ�
};

//�X�e�[�W
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

	bool move_flg[STAGE_HEIGHT][STAGE_WEIGHT];//�u���b�N�ړ�

	bool elec_flg[STAGE_HEIGHT][STAGE_WEIGHT];//�ʓd
	int elec_timer[STAGE_HEIGHT][STAGE_WEIGHT];//�ʓd�܂ł̎���

	DirectX::XMFLOAT3 pos;



	MyMesh obj[STAGE_HEIGHT][STAGE_WEIGHT];
	void Init();
	void Update();
	void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir);	//	�`��֐�
	void Release();

	int HitCheck(const DirectX::XMFLOAT3 check_pos);

	void BrockMove(const DirectX::XMFLOAT3 brock_pos, bool flg);

	bool CheckBrockElec(const DirectX::XMFLOAT3 brock_pos, DirectX::XMFLOAT3 scale);


};
//�n��
class Ground
{
public:

	DirectX::XMFLOAT3 pos;

	MyMesh obj[STAGE_HEIGHT][STAGE_WEIGHT];
	void Init();
	void Update();
	void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir);	//	�`��֐�
	void Release();


};

