#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include <sstream>
#include <vector>

#include "..\Header\Light.h"

#define MAX_BONE_INFLUENCES 4  //�P�̒��_���e�����󂯂�{�[���̍ő吔 
#define MAX_BONES 128


#define LIGHT_MAX 16

extern DirectX::XMINT4 light_exist[6];

class SkinMesh
{
public:
	struct bone
	{
		DirectX::XMFLOAT4X4 transform;
	};

	typedef std::vector<bone> skeletal;
	struct skeletal_animation :public std::vector<skeletal>
	{
		float sampling_time = 1 / 24.0f;    float animation_tick = 0.0f;//�o�ߎ���
	};

protected:
	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;

		FLOAT bone_weights[MAX_BONE_INFLUENCES] = { 1, 0, 0, 0 };
		INT bone_indices[MAX_BONE_INFLUENCES] = {};
	};

	struct cbuffer
	{
		DirectX::XMFLOAT4X4 world_view_projection;	//���[���h�E�r���[�E�v���W�F�N�V���������s�� 
		DirectX::XMFLOAT4X4 world;					//���[���h�ϊ��s�� 
		DirectX::XMFLOAT4 material_color;			//�ގ��F 
		DirectX::XMFLOAT4 light_direction;			//���C�g�i�s����
		DirectX::XMFLOAT4X4 bone_transforms[MAX_BONES];
	};

	struct cbuffer_light
	{
		DirectX::XMFLOAT4X4 world_view_projection;		//���[���h�E�r���[�E�v���W�F�N�V���������s�� 
		DirectX::XMFLOAT4X4 world;						//���[���h�ϊ��s�� 
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4 light_point[LIGHT_MAX];	//���C�g�̈ʒu
		DirectX::XMFLOAT4 attenution[LIGHT_MAX];		//����̌����̒���

		DirectX::XMFLOAT4 light_color[LIGHT_MAX];		//���C�g�̐F
		DirectX::XMFLOAT4 obj_color;					//obj�̐F
		DirectX::XMFLOAT4 light_directional;
		DirectX::XMINT4	  light_exist[LIGHT_MAX];
	};

	struct material
	{
		DirectX::XMFLOAT4 color = { 0.8f, 0.8f, 0.8f, 1.0f };

		ID3D11ShaderResourceView *shader_resource_view;
		D3D11_TEXTURE2D_DESC	*tex2d_desc;
	};

	//material diffuse;
	struct subset
	{
		u_int index_start = 0; // start number of index buffer
		u_int index_count = 0; // number of vertices (indices)
		material diffuse;
	};

	struct mesh
	{
		ID3D11Buffer			*vertex_buffer;
		ID3D11Buffer			*index_buffer;
		std::vector<subset> subsets;
		DirectX::XMFLOAT4X4 global_transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		//std::vector<SkinMesh::bone> skeletal;
		skeletal_animation skeletal_animation;
	};

	std::vector<mesh> meshes;

	ID3D11VertexShader		*vertex_shader;
	ID3D11VertexShader		*no_bone_vertex_shader;
	ID3D11VertexShader		*test_vs;
	ID3D11VertexShader		*energized_vs;
	ID3D11VertexShader		*load_vs;

	ID3D11PixelShader		*pixel_shader;
	ID3D11PixelShader		*no_texture_pixel_shader;
	ID3D11PixelShader		*test_ps;
	ID3D11PixelShader		*energized_ps;
	ID3D11PixelShader		*load_ps;

	ID3D11InputLayout		*input_layout;
	ID3D11InputLayout		*no_bone_input_layout;
	ID3D11InputLayout		*test_layout;
	ID3D11InputLayout		*energized_layout;
	ID3D11InputLayout		*load_layout;

	ID3D11Buffer			*const_buffer;

	ID3D11RasterizerState	*wire_rasterizer_state;
	ID3D11RasterizerState	*solid_rasterizer_state;

	ID3D11DepthStencilState	*depth_stencil;

	ID3D11SamplerState		*sampler_state;

public:

	bool is_bone;
	bool culling_clockwise;

	DirectX::XMFLOAT4X4 coordinate_conversion =
	{
		1, 0, 0, 0,
		0, 0, 1, 0,
		0, 1, 0, 0,
		0, 0, 0, 1
	};

	SkinMesh(ID3D11Device*, const std::string& fbx_filename);
	~SkinMesh() { Release(); }

	void Render(
		ID3D11DeviceContext *device_context,				//�f�o�C�X�R���e�L�X�g 
		const DirectX::XMFLOAT4X4 & world_view_projection,	//���[���h�E�r���[�E�v���W�F�N�V���������s�� 
		const DirectX::XMFLOAT4X4 & world,					//���[���h�ϊ��s�� 
		const DirectX::XMFLOAT4 & light_direction,			//���C�g�i�s���� 
		const DirectX::XMFLOAT4 & material_color,			//�ގ��F 
		float elapsed_time,
		bool mode											//���E�h��Ԃ��`��t���O 
		);

	void Render(
		ID3D11DeviceContext *device_context,				//�f�o�C�X�R���e�L�X�g 
		const DirectX::XMFLOAT4X4 & world_view_projection,	//���[���h�E�r���[�E�v���W�F�N�V���������s�� 
		const DirectX::XMFLOAT4X4 & world,					//���[���h�ϊ��s�� 
		const DirectX::XMFLOAT4X4 & view,
		const DirectX::XMFLOAT4X4 & projection,
		const DirectX::XMFLOAT4 & light_direction,			//���C�g�i�s���� 
		const DirectX::XMFLOAT4 & material_color,			//�ގ��F 
		float elapsed_time,
		bool mode,											//���E�h��Ԃ��`��t���O 

		const DirectX::XMFLOAT3 &pos,
		const int &light_num,
		const Light &_light
		);

	void EnergizedRender(
		ID3D11DeviceContext *device_context,				//�f�o�C�X�R���e�L�X�g 
		const DirectX::XMFLOAT4X4 & world_view_projection,	//���[���h�E�r���[�E�v���W�F�N�V���������s�� 
		const DirectX::XMFLOAT4X4 & world,					//���[���h�ϊ��s�� 
		const DirectX::XMFLOAT4X4 & view,
		const DirectX::XMFLOAT4X4 & projection,
		const DirectX::XMFLOAT4 & light_direction,			//���C�g�i�s���� 
		const DirectX::XMFLOAT4 & material_color,			//�ގ��F 
		float elapsed_time,
		bool mode,											//���E�h��Ԃ��`��t���O 

		const DirectX::XMFLOAT3 &pos,
		const int &light_num,
		const Light &_light
		);

	void LoadRender(
		ID3D11DeviceContext *device_context,				//�f�o�C�X�R���e�L�X�g 
		const DirectX::XMFLOAT4X4 & world_view_projection,	//���[���h�E�r���[�E�v���W�F�N�V���������s�� 
		const DirectX::XMFLOAT4X4 & world,					//���[���h�ϊ��s�� 
		const DirectX::XMFLOAT4X4 & view,
		const DirectX::XMFLOAT4X4 & projection,
		const DirectX::XMFLOAT4 & light_direction,			//���C�g�i�s���� 
		const DirectX::XMFLOAT4 & material_color,			//�ގ��F 
		float elapsed_time,
		bool mode,											//���E�h��Ԃ��`��t���O 

		const DirectX::XMFLOAT3 &pos,
		const int &light_num,
		const Light &_light
		);

	void Release();

	//void CreateBuffers(ID3D11Device *device, vertex *vertices, int num_vertices, u_int *indices, int num_indices);
	void CreateBuffers(ID3D11Device *device, vertex *vertices, int num_vertices, u_int *indices, int num_indices, mesh &mesh);

	void LoadFBX(ID3D11Device*, const std::string& fbx_filename);
};