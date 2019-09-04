#include	"..\Header\particle.h"

#include	"..\Header\framework.h"


/*******************************************************************************
	�p�[�e�B�N���Ǘ��N���X�̍X�V�֐�
*******************************************************************************/
void	ParticleManager::Update()
{
	for (auto& pd : data)
	{
		if (!pd.exist)	continue;

		pd.speed.x	+= pd.accel.x;		pd.speed.y	+= pd.accel.y;		pd.speed.z	+= pd.accel.z;
		pd.pos.x	+= pd.speed.x;		pd.pos.y	+= pd.speed.y;		pd.pos.z	+= pd.speed.z;

		pd.dispTimer--;
		if (pd.dispTimer < 0)	pd.exist = false;
	}
}


/*******************************************************************************
	�p�[�e�B�N���Ǘ��N���X�̕`��֐�
*******************************************************************************/
void	ParticleManager::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)
{
	DirectX::XMFLOAT4X4	view2;
	DirectX::XMMATRIX	inv_view2;

	//	���ʂ̍s��Z�o(����view�t�s��AViewProjection�s��)
	DirectX::XMStoreFloat4x4(&view2, view);						//	��U4x4�ɕϊ�
	view2._41 = 0.0f; view2._42 = 0.0f; 						//	view�̈ʒu��񂾂����폜
	view2._43 = 0.0f; view2._44 = 1.0f;							//	
	inv_view2 = DirectX::XMLoadFloat4x4(&view2);				//	Matrix�֍ĕϊ�
	inv_view2 = DirectX::XMMatrixInverse(nullptr, inv_view2);	//	view�s��̋t�s��

	DirectX::XMMATRIX	VP;
	VP	= view * projection;									//	���e�s�񍇐�
	
	//	�`�揀�����`��
	DirectX::XMFLOAT4 light_dir(0, 0, 1, 0);					//	��������������㖳����

	blender::Set(blender::ADD);

	DirectX::XMMATRIX	S, T, W;
	DirectX::XMFLOAT4X4	world, wvp;
	for ( int n = 0; n < MAX; n++ ){
		Particle&	pd	= data[n];
		if (!pd.exist)		continue;

		//	���[���h�s�񍇐�(��]�s��s�v)
		S = DirectX::XMMatrixScaling(pd.scale, pd.scale, pd.scale);
		T = DirectX::XMMatrixTranslation(pd.pos.x, pd.pos.y, pd.pos.z);
		W = S * T;

		//	�`��p�s��(4x4)����
		DirectX::XMStoreFloat4x4(&world, W);					//	���[���h�ϊ��s���4x4�ɕϊ�
		W = inv_view2 * W * VP;									//	WorldViewProjection�s�񍇐�(view�t�s��ɂ��ŏ���)
		DirectX::XMStoreFloat4x4(&wvp, W);						//	WorldViewProjection�s���4x4�ɕϊ�
		polygon->Render( FRAMEWORK.getContext(), wvp, world, light_dir, pd.color ); 		//	�`��
	}

	blender::Set(blender::ALPHA);
}


/*******************************************************************************
	�p�[�e�B�N���Ǘ��N���X�̐ݒu�֐�
*******************************************************************************/
bool	ParticleManager::Set(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 speed, DirectX::XMFLOAT3 accel,
							float scale, DirectX::XMFLOAT4 color, int timer)
{
	Particle	d;
	d.pos	= pos;
	d.scale	= scale;
	d.color	= color;

	d.speed = speed;
	d.accel = accel;
	d.dispMaxTimer	= d.dispTimer	= timer;

	return	Set(d);
}
bool	ParticleManager::Set(const Particle& d)
{
	for (int n = 0; n < MAX; n++){
		Particle&	pd = data[n];
		if (pd.exist)		continue;

		pd			= d;
		pd.exist	= true;
		return	true;
	}

	return	false;
}
