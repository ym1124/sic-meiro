#include	"..\Header\shot.h"

#include	"..\Header\framework.h"
#include	"..\Header\player.h"

#include "..\Header\Light.h"

extern	Player player;



/*******************************************************************************
	�e�ۃN���X�̈ړ��֐�
*******************************************************************************/
void	Shot::Move()
{
	//	�ő勗����2������O�Ɍv�Z
	const float		DIST_SQUARE = MAX_DISTANCE * MAX_DISTANCE;

	//	�ړ�
	pos.x += speed.x;
	pos.y += speed.y;
	pos.z += speed.z;

	light.pos = pos;

	//	�v���C���[�Ƃ̋����Z�o
	DirectX::XMFLOAT3 vec;
	vec.x = pos.x - player.pos.x;
	vec.y = pos.y - player.pos.y;
	vec.z = pos.z - player.pos.z;
	float	d_sq = vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;

	//	�����̔�r
	if (d_sq < DIST_SQUARE)	return;

	//	�s�����x�Z�o
	alpha = timer / (float)FADE_TIMER;


	//	�^�C�}�[�Ǘ�
	timer--;
	if (timer < 0)
	{
		//	���ŏ���
		obj.Release();
		exist = false;
	}
}


/*******************************************************************************
	�e�ۊǗ��N���X�̏������֐�
*******************************************************************************/
void	ShotManager::Initialize()
{
	for (auto& s : data)
	{
		s.exist = false;
		s.obj.Initialize();
	}
}


/*******************************************************************************
	�e�ۊǗ��N���X�̉���֐�
*******************************************************************************/
void	ShotManager::Release()
{
	for (auto& s : data)
	{
		s.exist = false;
		s.obj.Release();
	}
}


/*******************************************************************************
	�e�ۊǗ��N���X�̍X�V�֐�
*******************************************************************************/
void	ShotManager::Update()
{
	for (auto& s : data)
	{
		if( !s.exist )	continue;

		s.Move();
	}
}


/*******************************************************************************
	�e�ۊǗ��N���X�̕`��֐�
*******************************************************************************/
void	ShotManager::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj, const DirectX::XMFLOAT4& light_dir)
{
	for (auto& s : data)
	{
		if( !s.exist )	continue;

		s.obj.pos = s.pos;
		s.obj.angle.y = s.angle;
		s.obj.color.w = s.alpha;

		s.obj.Render(view, proj, light_dir);
	}
}


/*******************************************************************************
	�e�ۊǗ��N���X�̔��ˊ֐�
*******************************************************************************/
Shot*	ShotManager::Set(DirectX::XMFLOAT3 pos, float angle, float speed, float scale, DirectX::XMFLOAT3 color)
{
	for (auto& s : data)
	{
		if( s.exist )	continue;

		s.obj.Release();		//	�O�̈׉��
		s.obj.SetPrimitive(new GeometricSphere(FRAMEWORK.getDevice()));
		s.obj.scale = DirectX::XMFLOAT3(scale, scale, scale);
		s.obj.color = DirectX::XMFLOAT4(color.x, color.y, color.z, 1.0f );
		s.pos = pos;
		s.angle = angle;
		s.speed = DirectX::XMFLOAT3(sinf(angle)*speed, .0f, cosf(angle)*speed);
		s.timer = Shot::FADE_TIMER;
		s.alpha = 1.0f;
		s.exist = true;

		return	&s;
	}

	return	nullptr;
}


