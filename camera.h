#pragma once

#include	<DirectXMath.h>


/*******************************************************************************
�u�J�����v�N���X
*******************************************************************************/
class Camera
{
public:
	/*******************************************************************************
	�J������ԊǗ��ԍ��̐錾
	*******************************************************************************/
	enum MODE
	{
		WATCH,				//	�u�Ď��J�����v
		DIRECTIONAL,		//	�u���Έʒu�Œ�J�����v
		CHASE,				//	�u�ǐՃJ�����v
		TPS,				//	�u3�l�̃J����(TPS)�v
		FPS,				//	�u1�l�̃J����(FPS)�v
		UP,					//�@�u�^��J�����v
		MAX					//	�u�J�����Ǘ��ԍ��ő�l�v	
	};

private:
	/*******************************************************************************
	�J������ԕۑ��p�ϐ��̒�`
	*******************************************************************************/
	int		mode;

public:
	DirectX::XMFLOAT3 position;				//	�u�ʒu(���W)�v
	DirectX::XMFLOAT3 point_view;			//	�u�����_�v
	DirectX::XMMATRIX project_matrix;		//	�u���e�s��v

	Camera();
	/*******************************************************************************
	�J�����X�V�֐��̐錾
	*******************************************************************************/
	void				Update();					//	�X�V�֐�

private:		//	�ǉ������o�֐�(�O������Ăяo���\�肪�����̂�private��)(�����g�ݕ����˂��c)
	void				Watch();		//	�Ď��J�����֐�
	void				Directional();	//	���Έʒu�Œ�J�����֐�
	void				Chase();		//	�ǐՃJ�����֐�
	void				Tps();			//	3�l�̃J����(TPS)�֐�
	void				Fps();			//	1�l�̃J����(FPS)�֐�
	void				Up();
public:

	void Initialize();

	DirectX::XMMATRIX SetParallelProjection(float width, float height, float recently, float farthest);
	DirectX::XMMATRIX SetPerspectiveProjection(float view_angle, float aspect, float recently, float farthest);
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix() { return project_matrix; }

};