#include	"..\Header\Mesh.h"

#include	"..\Header\framework.h"



/*******************************************************************************
���b�V��������
*******************************************************************************/
void	MyMesh::Initialize()
{
	//	��񏉊��l
	pos = DirectX::XMFLOAT3(0, 0, 0);
	angle = DirectX::XMFLOAT3(0, 0, 0);
	scale = DirectX::XMFLOAT3(1, 1, 1);


	color = DirectX::XMFLOAT4(1, 1, 1, 1);

	anim_timer = 0.0f;
	//	���f���ǂݍ���
	obj = nullptr;
}

/*******************************************************************************
���b�V�����f���ǂݍ���
����
fbx_filename:���b�V���t�@�C����(FBX�`��)
*******************************************************************************/
bool MyMesh::Load(const char* fbx_filename)
{
	//	���f���ǂݍ���
	obj = new SkinMesh(FRAMEWORK.getDevice(), fbx_filename);
	bLoad = true;

	return	true;
}

/*******************************************************************************
���b�V�����f���ݒ�
����
primitive:���b�V�����f��
*******************************************************************************/
//void	MyMesh::SetPrimitive(GeometricPrimitive* primitive)
//{
//	obj = primitive;
//	bLoad = true;
//}

/*******************************************************************************
���b�V�����f���ݒ�(�g���񂵌^)
����
primitive:���b�V�����f��
*******************************************************************************/
void	MyMesh::SetMesh(MyMesh& org)
{
	*this = org;
	bLoad = false;
}

/*******************************************************************************
���b�V�����
*******************************************************************************/
void	MyMesh::Release()
{
	if (obj && bLoad)
	{
		delete	obj;
	}
	obj = nullptr;
	bLoad = false;
}

/*******************************************************************************
���b�V���`��
����
view:�r���[�ϊ��s��
projection:���e�ϊ��s��
light_dir:���s���x�N�g��
*******************************************************************************/
void	MyMesh::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, float timer)
{
	//	���f����������Ε`�揈������؍s��Ȃ�
	if (!obj)		return;

	//	���[���h�ϊ��s��擾
	DirectX::XMMATRIX worldM = GetWorldMatrix();

	//	Matrix -> Float4x4 �ϊ�
	DirectX::XMFLOAT4X4 world_view_projection;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world_view_projection, worldM * view * projection);
	DirectX::XMStoreFloat4x4(&world, worldM);

	//	�`��
	obj->Render(FRAMEWORK.getContext(), world_view_projection, world, light_dir, color, timer, true);
}

void MyMesh::Render(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& projectionM, const DirectX::XMFLOAT4& light_dir, int renderType, const DirectX::XMFLOAT3& pos, const int light_num, const Light &light, float timer)
{
	//	���f����������Ε`�揈������؍s��Ȃ�
	if (!obj)		return;

	//	���[���h�ϊ��s��擾
	DirectX::XMMATRIX worldM = GetWorldMatrix();

	//	Matrix -> Float4x4 �ϊ�
	DirectX::XMFLOAT4X4 world_view_projection;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMStoreFloat4x4(&world_view_projection, worldM * viewM * projectionM);
	DirectX::XMStoreFloat4x4(&world, worldM);
	DirectX::XMStoreFloat4x4(&view, viewM);
	DirectX::XMStoreFloat4x4(&projection, projectionM);

	//	�`��
	switch (renderType)
	{
	case 0:
		obj->Render(FRAMEWORK.getContext(), world_view_projection, world, view, projection, light_dir, color, timer, true, pos, light_num, light);
		break;
	case 1:
		obj->EnergizedRender(FRAMEWORK.getContext(), world_view_projection, world, view, projection, light_dir, color, timer, true, pos, light_num, light);
		break;
	case 2:
		obj->LoadRender(FRAMEWORK.getContext(), world_view_projection, world, view, projection, light_dir, color, timer, true, pos, light_num, light);
		break;
	}
}

/*******************************************************************************
���b�V����񂩂烏�[���h�ϊ��s��擾
*******************************************************************************/
DirectX::XMMATRIX	MyMesh::GetWorldMatrix()
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX S, R, Rx, Ry, Rz, T;

	//	������
	world = DirectX::XMMatrixIdentity();

	//	�g��E�k��
	S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	//	��]
	Rx = DirectX::XMMatrixRotationX(angle.x);				//	X������Ƃ�����]�s��
	Ry = DirectX::XMMatrixRotationY(angle.y);				//	Y������Ƃ�����]�s��
	Rz = DirectX::XMMatrixRotationZ(angle.z);				//	Z������Ƃ�����]�s��
	R = Rz * Ry * Rx;

	//	���s�ړ�
	T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	//	���[���h�ϊ��s��
	world = S * R * T;


	return	world;
}










/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GeometricMesh::Initialize()
{
	//	��񏉊��l
	pos = DirectX::XMFLOAT3(0, 0, 0);
	angle = DirectX::XMFLOAT3(0, 0, 0);
	scale = DirectX::XMFLOAT3(1, 1, 1);


	color = DirectX::XMFLOAT4(1, 1, 1, 1);

	//	���f���ǂݍ���
	obj = nullptr;
}

void GeometricMesh::SetPrimitive(GeometricPrimitive* primitive)
{
	obj = primitive;
	bLoad = true;
}

/*******************************************************************************
���b�V�����f���ݒ�(�g���񂵌^)
����
primitive:���b�V�����f��
*******************************************************************************/
void GeometricMesh::SetMesh(GeometricMesh& org)
{
	*this = org;
	bLoad = false;
}

/*******************************************************************************
���b�V�����
*******************************************************************************/
void GeometricMesh::Release()
{
	if (obj && bLoad)
	{
		delete	obj;
	}
	obj = nullptr;
	bLoad = false;
}

/*******************************************************************************
���b�V���`��
����
view:�r���[�ϊ��s��
projection:���e�ϊ��s��
light_dir:���s���x�N�g��
*******************************************************************************/
void GeometricMesh::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, float timer)
{
	//	���f����������Ε`�揈������؍s��Ȃ�
	if (!obj)		return;

	//	���[���h�ϊ��s��擾
	DirectX::XMMATRIX worldM = GetWorldMatrix();

	//	Matrix -> Float4x4 �ϊ�
	DirectX::XMFLOAT4X4 world_view_projection;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world_view_projection, worldM * view * projection);
	DirectX::XMStoreFloat4x4(&world, worldM);

	//	�`��
	obj->Render(FRAMEWORK.getContext(), world_view_projection, world, light_dir, color, true);
}


/*******************************************************************************
���b�V����񂩂烏�[���h�ϊ��s��擾
*******************************************************************************/
DirectX::XMMATRIX GeometricMesh::GetWorldMatrix()
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX S, R, Rx, Ry, Rz, T;

	//	������
	world = DirectX::XMMatrixIdentity();

	//	�g��E�k��
	S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	//	��]
	Rx = DirectX::XMMatrixRotationX(angle.x);				//	X������Ƃ�����]�s��
	Ry = DirectX::XMMatrixRotationY(angle.y);				//	Y������Ƃ�����]�s��
	Rz = DirectX::XMMatrixRotationZ(angle.z);				//	Z������Ƃ�����]�s��
	R = Rz * Ry * Rx;

	//	���s�ړ�
	T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	//	���[���h�ϊ��s��
	world = S * R * T;


	return	world;
}
