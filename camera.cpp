#include	"..\Header\camera.h"

#include	"..\Header\framework.h"

#include	"..\Header\player.h"

extern	Player	player;

Camera::Camera()
{
	Initialize();
}

void Camera::Initialize()
{
	position = { 0.0f,10.0f,-15.0f };
	point_view = { 0.0f,0.0f,0.0f };

	float view_angle = 30.0f*3.14f / 180.0f;		//����p
	float aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);	//�A�X�y�N�g��
	float recently = 0.1f;							//�ŋߒl
	float farthest = 1000.0f;						//�ŉ��l

	mode = MODE::UP;

	SetPerspectiveProjection(view_angle, aspect, recently, farthest);
}

//���s���e�s����Z�o����֐�
DirectX::XMMATRIX Camera::SetParallelProjection(float width, float height, float recently, float farthest)
{
	project_matrix = DirectX::XMMatrixOrthographicLH(width, height, recently, farthest);

	return project_matrix;
}


//�������e�s����Z�o����֐�
DirectX::XMMATRIX Camera::SetPerspectiveProjection(float width, float height, float recently, float farthest)
{
	project_matrix = DirectX::XMMatrixPerspectiveFovLH(width, height, recently, farthest);

	return project_matrix;
}


//�r���[�s����擾����֐�
DirectX::XMMATRIX Camera::GetViewMatrix()
{
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f);
	DirectX::XMVECTOR focus = DirectX::XMVectorSet(point_view.x, point_view.y, point_view.z, 1.0f);
	DirectX::XMVECTOR up_vector = { 0.0f,1.0f,0.0f,0.0f };

	return DirectX::XMMatrixLookAtLH(pos, focus, up_vector);
}


/*******************************************************************************
�J�����p�X�V�֐��̎���
*******************************************************************************/
void	Camera::Update()
{
	//	�J�����̍X�V
	switch (mode)
	{
	case WATCH:			Watch();		break;		//	�u�Ď��J�����v
	case DIRECTIONAL:	Directional();	break;		//	�u���Έʒu�Œ�J�����v
	//case CHASE-1:			Chase();		break;		//	�u�ǐՃJ�����v
	//case TPS:			Tps();			break;		//	�u3�l�̃J����(TPS)�v
	case FPS-2:			Fps();			break;		//	�u1�l�̃J����(FPS)�v
	case UP:			Up();			break;
	}


	//	�J�����̐؂�ւ�
	if (kb.PushTrigger('C') || pad.PushTrigger(XINPUT_GAMEPAD_X, GamePad::PlayerID::_1P))
	{
		mode = (mode + 1) % (MODE::MAX - 2);		//	1�s�ł���������
	}
}


/*******************************************************************************
�u�Ď��J�����v�̎���
*******************************************************************************/
void	Camera::Watch()
{
	position = DirectX::XMFLOAT3(.0f, 10.0f, -15.0f);
	point_view = player.pos;
}
/*******************************************************************************
�u���Έʒu�Œ�J�����v�̎���
*******************************************************************************/
void	Camera::Directional()
{
	position.x = player.pos.x + 8.0f;
	position.y = player.pos.y + 5.0f;
	position.z = player.pos.z - 8.0f;

	point_view = player.pos;
}
/*******************************************************************************
�u3�l�̃J�����v�̎���
*******************************************************************************/
void	Camera::Tps()
{
	const float	Distance = 12.0f;

	float	dx = sinf(player.angle);
	float	dz = cosf(player.angle);

	position.x = player.pos.x - Distance * dx;
	position.z = player.pos.z - Distance * dz;

	point_view = player.pos;
}
/*******************************************************************************
�u1�l�̃J�����v�̎���
*******************************************************************************/
void	Camera::Fps()
{
	const float	Height = 0.75f;

	float	dx = sinf(player.angle);
	float	dz = cosf(player.angle);

	position.x = player.pos.x;
	position.y = player.pos.y + Height;
	position.z = player.pos.z;

	point_view.x = position.x + dx;
	point_view.y = position.y;
	point_view.z = position.z + dz;
}
/*******************************************************************************
�u�ǐՃJ�����v�̎���
*******************************************************************************/
void	Camera::Chase()
{
	const float	minDistance = 5.0f;
	const float	maxDistance = 15.0f;

	float	dx = player.pos.x - position.x;
	float	dz = player.pos.z - position.z;
	float	distance = sqrtf(dx*dx + dz*dz);

	if (distance < minDistance)
	{
		dx /= distance;		//	�x�N�g���̐��K��
		dz /= distance;		//	
		position.x = player.pos.x - minDistance * dx;
		position.z = player.pos.z - minDistance * dz;
	}
	if (distance > maxDistance)
	{
		dx /= distance;		//	�x�N�g���̐��K��
		dz /= distance;		//	
		position.x = player.pos.x - maxDistance * dx;
		position.z = player.pos.z - maxDistance * dz;
	}

	point_view = player.pos;
}

void Camera::Up() 
{
	const float Height = 15.0f;

	position.x = player.pos.x;
	position.y = player.pos.y + Height;
	position.z = player.pos.z - 20.0f;

	point_view = player.pos;
}