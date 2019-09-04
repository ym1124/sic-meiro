#pragma once

#include	<DirectXMath.h>

#include	"..\Header\Mesh.h"

#define HP_MAX 300

/*******************************************************************************
	�u�v���C���[�v�N���X
*******************************************************************************/
class Player
{
private:
	//hp
	float electric;

	float elect_power;
	float shadow_timer;

	bool brock_move_flg;
	int p;

	int move_se = -1;
	int electric_se;
	int catch_se;
	int rele_se;
	int no_move_se;

public:
	float GetElectric() { return electric; }
	void SetElectric(float _electric) { electric = _electric; }

	MyMesh				obj;				//	�u���f���I�u�W�F�N�g�v
	MyMesh				head;				//	�u���f���I�u�W�F�N�g�v
	MyMesh				tail;				//	�u���f���I�u�W�F�N�g�v

	MyMesh				shadow[4];				//	�u���f���I�u�W�F�N�g�v

	DirectX::XMFLOAT3	pos;				//	�u�ʒu(���W)�v
	float				angle;				//	�u��]�p�x�v

	int e_timer;
	bool elec_heal_flg;
	bool brock_elec_flg;
	bool goal_flg;

	void	Initialize( const char* );																							//	�������֐�
	void	Release();																											//	����֐�
	void	Move();																												//	�ړ��֐�
	void	Render( const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, int renderType=0);	//	�`��֐�
};


extern	Player player;
