#include "..\Header\SkinMesh.h"

#include "..\Header\misc.h"

#include <fbxsdk.h> 
using namespace fbxsdk;
#include <vector> 
#include <functional>  

#include "..\Header\ResourceManager.h"
#include "..\Header\Input.h"

#include "..\Header\player.h"

extern Light player_light;

DirectX::XMFLOAT3 lanthanums_pos[6] = { DirectX::XMFLOAT3(4.0f,1.0f,-13.0f),DirectX::XMFLOAT3(8.0f,1.0f,-10.0f) ,DirectX::XMFLOAT3(12.0f,1.0f,-7.0f),DirectX::XMFLOAT3(4.0f,1.0f,-4.0f),DirectX::XMFLOAT3(8.0f,1.0f,-1.0f) };
DirectX::XMINT4 light_exist[6] = { DirectX::XMINT4(.0f,.0f,.0f,.0f), DirectX::XMINT4(.0f,.0f,.0f,.0f), DirectX::XMINT4(.0f,.0f,.0f,.0f), DirectX::XMINT4(.0f,.0f,.0f,.0f), DirectX::XMINT4(.0f,.0f,.0f,.0f), DirectX::XMINT4(.0f,.0f,.0f,.0f) };

void Move(DirectX::XMFLOAT3 &tes, DirectX::XMFLOAT4 &test)
{
	static float speed = 0.005f;
	static float rotate_speed = 0.5f;

	if (kb.Push(VK_UP)) test.z += speed;
	if (kb.Push(VK_DOWN)) test.z -= speed;
	if (kb.Push(VK_RIGHT)) test.x += speed;
	if (kb.Push(VK_LEFT)) test.x -= speed;

	if (kb.Push(VK_SPACE)) test.y += speed;
	if (kb.Push('Z')) test.y -= speed;

	if (kb.Push(VK_SHIFT)) {
		if (kb.Push(VK_UP)) tes.z += speed / 100.0f;
		if (kb.Push(VK_DOWN)) tes.z -= speed / 100.0f;
		if (kb.Push(VK_RIGHT)) tes.x += speed;
		if (kb.Push(VK_LEFT)) tes.x -= speed;

		if (kb.Push(VK_SPACE)) tes.y += speed / 10.0f;
		if (kb.Push('Z')) tes.y -= speed / 10.0f;
	}
	//if (test.x > 1.0f) test.x = 1.0f;
	//if (test.x < 0.0f) test.x = 0.0f;

	//if (test.y > 1.0f) test.y = 1.0f;
	//if (test.y < 0.0f) test.y = 0.0f;

	//if (test.z > 1.0f) test.z = 1.0f;
	//if (test.z < 0.0f) test.z = 0.0f;

}

struct bone_influence {
	int index; // index of bone
	float weight; // weight of bone 
};

typedef std::vector<bone_influence> bone_influences_per_control_point;

void fetch_bone_influences(const FbxMesh *fbx_mesh, std::vector<bone_influences_per_control_point> &influences);
void fetch_bone_matrices(FbxMesh *fbx_mesh, std::vector<SkinMesh::bone> &skeletal, FbxTime time);
void fetch_animations(FbxMesh *fbx_mesh, SkinMesh::skeletal_animation &skeletal_animation, u_int sampling_rate = 0);

void fbxamatrix_to_xmfloat4x4(const FbxAMatrix &fbxamatrix, DirectX::XMFLOAT4X4 &xmfloat4x4);


SkinMesh::SkinMesh(ID3D11Device *device, const std::string& fbx_filename)
{
	LoadFBX(device, fbx_filename);

	// Initialize Direct3D COM objects 
	HRESULT hr = S_OK;

	//���̓��C�A�E�g�I�u�W�F�N�g�̐��� 
	D3D11_INPUT_ELEMENT_DESC VertexDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//���_�V�F�[�_�[�I�u�W�F�N�g�̐��� 
	if (!ResourceManager::LoadVertexShaders(device, "Shader/skin_mesh_vs.cso", VertexDesc, ARRAYSIZE(VertexDesc), &vertex_shader, &input_layout)) {
		return;
	}

	if (!ResourceManager::LoadVertexShaders(device, "Shader/geometric_primitive_vs.cso", VertexDesc, ARRAYSIZE(VertexDesc), &no_bone_vertex_shader, &no_bone_input_layout)) {
		return;
	}

	if (!ResourceManager::LoadVertexShaders(device, "Shader/test_vs.cso", VertexDesc, ARRAYSIZE(VertexDesc), &test_vs, &test_layout)) {
		return;
	}

	if (!ResourceManager::LoadVertexShaders(device, "Shader/energized_vs.cso", VertexDesc, ARRAYSIZE(VertexDesc), &energized_vs, &energized_layout)) {
		return;
	}

	if (!ResourceManager::LoadVertexShaders(device, "Shader/load_vs.cso", VertexDesc, ARRAYSIZE(VertexDesc), &load_vs, &load_layout)) {
		return;
	}

	//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�̐��� 
	if (!ResourceManager::LoadPixelShaders(device, "Shader/skin_mesh_ps.cso", &pixel_shader)) {
		return;
	}

	if (!ResourceManager::LoadPixelShaders(device, "Shader/geometric_primitive_ps.cso", &no_texture_pixel_shader)) {
		return;
	}

	if (!ResourceManager::LoadPixelShaders(device, "Shader/test_ps.cso", &test_ps)) {
		return;
	}

	if (!ResourceManager::LoadPixelShaders(device, "Shader/energized_ps.cso", &energized_ps)) {
		return;
	}

	if (!ResourceManager::LoadPixelShaders(device, "Shader/load_ps.cso", &load_ps)) {
		return;
	}

	//culling_clockwise = true;

	// create rasterizer state : solid mode
	{
		D3D11_RASTERIZER_DESC rasterizer_desc = {};
		rasterizer_desc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
		rasterizer_desc.CullMode = D3D11_CULL_BACK; //D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK   
		rasterizer_desc.FrontCounterClockwise = culling_clockwise;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0;
		rasterizer_desc.SlopeScaledDepthBias = 0;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.ScissorEnable = FALSE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.AntialiasedLineEnable = FALSE;
		hr = device->CreateRasterizerState(&rasterizer_desc, &solid_rasterizer_state);
		if (FAILED(hr)) return;
	}
	// create rasterizer state : wireframe mode
	{
		D3D11_RASTERIZER_DESC rasterizer_desc = {};
		rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
		rasterizer_desc.CullMode = D3D11_CULL_BACK; //D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK   
		rasterizer_desc.FrontCounterClockwise = culling_clockwise;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0;
		rasterizer_desc.SlopeScaledDepthBias = 0;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.ScissorEnable = FALSE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.AntialiasedLineEnable = FALSE;
		hr = device->CreateRasterizerState(&rasterizer_desc, &wire_rasterizer_state);
		if (FAILED(hr)) return;
	}

	D3D11_SAMPLER_DESC smpDesc = {};

	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; //D3D11_FILTER_ANISOTROPIC
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.MipLODBias = 0;
	smpDesc.MaxAnisotropy = 16;
	smpDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	memcpy(smpDesc.BorderColor, &DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), sizeof(DirectX::XMFLOAT4));
	smpDesc.MinLOD = 0;
	smpDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&smpDesc, &sampler_state);
	if (FAILED(hr)) return;

	// create depth stencil state
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
		depth_stencil_desc.StencilEnable = FALSE;
		depth_stencil_desc.StencilReadMask = 0xFF;
		depth_stencil_desc.StencilWriteMask = 0xFF;
		depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		hr = device->CreateDepthStencilState(&depth_stencil_desc, &depth_stencil);
		if (FAILED(hr)) return;
	}
	// create constant buffer
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(cbuffer);
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		hr = device->CreateBuffer(&buffer_desc, nullptr, &const_buffer);
		if (FAILED(hr)) return;
	}
}

void SkinMesh::Render(
	ID3D11DeviceContext *device_context,				//�f�o�C�X�R���e�L�X�g 
	const DirectX::XMFLOAT4X4 & world_view_projection,	//���[���h�E�r���[�E�v���W�F�N�V���������s�� 
	const DirectX::XMFLOAT4X4 & world,					//���[���h�ϊ��s�� 
	const DirectX::XMFLOAT4 & light_direction,			//���C�g�i�s���� 
	const DirectX::XMFLOAT4 & material_color,			//�ގ��F 
	float elapsed_time,
	bool mode											//���E�h��Ԃ��`��t���O 
	)
{
	//	�v���~�e�B�u���[�h�̐ݒ�
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	���̓��C�A�E�g�̃o�C���h
	device_context->IASetInputLayout(input_layout);

	//	���X�^���C�U�[�̐ݒ�
	if (mode)	device_context->RSSetState(solid_rasterizer_state);
	else		device_context->RSSetState(wire_rasterizer_state);

	//	�V�F�[�_�[(2��)�̐ݒ�
	device_context->VSSetShader(vertex_shader, nullptr, 0);
	device_context->PSSetShader(pixel_shader, nullptr, 0);

	//	�[�x�e�X�g�̐ݒ�
	device_context->OMSetDepthStencilState(depth_stencil, 0);

	if (!is_bone) {
		device_context->IASetInputLayout(no_bone_input_layout);

		device_context->VSSetShader(no_bone_vertex_shader, nullptr, 0);

		coordinate_conversion =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}
	else {
		coordinate_conversion =
		{
			-1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	//	�v���~�e�B�u�̕`��(index�t��)
	//device_context->DrawIndexed(36, 0, 0);
	for (unsigned int i = 0; i < meshes.size(); i++) {
		// ���_�o�b�t�@�̃o�C���h
		UINT stride = sizeof(vertex);
		UINT offset = 0;
		device_context->IASetVertexBuffers(0, 1, &meshes[i].vertex_buffer, &stride, &offset);

		//	�C���f�b�N�X�o�b�t�@�̃o�C���h
		device_context->IASetIndexBuffer(meshes[i].index_buffer, DXGI_FORMAT_R32_UINT, 0);


		for (unsigned int j = 0; j < meshes[i].subsets.size(); j++) {
			//	�萔�o�b�t�@�̍쐬
			cbuffer cb;

			//cb.world_view_projection = world_view_projection;
			//cb.world = world;
			if (meshes[i].skeletal_animation.size() > 0) {
				int frame = static_cast<int>(meshes[i].skeletal_animation.animation_tick / meshes[i].skeletal_animation.sampling_time);

				if (static_cast<unsigned int>(frame) > meshes[i].skeletal_animation.size() - 1) {
					frame = 0;
					meshes[i].skeletal_animation.animation_tick = 0;
				}

				std::vector<bone> &skeletal = meshes[i].skeletal_animation.at(frame);
				size_t number_of_bones = skeletal.size();
				_ASSERT_EXPR(number_of_bones < MAX_BONES, L"'the number_of_bones' exceeds MAX_BONES.");

				for (size_t i = 0; i < number_of_bones; i++) {
					XMStoreFloat4x4(&cb.bone_transforms[i], XMLoadFloat4x4(&skeletal.at(i).transform));
				}

				meshes[i].skeletal_animation.animation_tick += elapsed_time;
			}
			if (meshes[i].subsets.size() == 1) {
				meshes[i].global_transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

			}

			DirectX::XMStoreFloat4x4(&cb.world_view_projection, DirectX::XMLoadFloat4x4(&meshes[i].global_transform) * DirectX::XMLoadFloat4x4(&coordinate_conversion) * DirectX::XMLoadFloat4x4(&world_view_projection));
			DirectX::XMStoreFloat4x4(&cb.world, DirectX::XMLoadFloat4x4(&meshes[i].global_transform) *  DirectX::XMLoadFloat4x4(&coordinate_conversion) * DirectX::XMLoadFloat4x4(&world));

			cb.light_direction = light_direction;
			cb.material_color = material_color;
			device_context->UpdateSubresource(const_buffer, 0, nullptr, &cb, 0, 0);
			device_context->VSSetConstantBuffers(0, 1, &const_buffer);

			if (meshes[i].subsets[j].diffuse.shader_resource_view) {
				device_context->PSSetShaderResources(0, 1, &meshes[i].subsets[j].diffuse.shader_resource_view);
				device_context->PSSetSamplers(0, 1, &sampler_state);
			}
			else {
				device_context->PSSetShader(no_texture_pixel_shader, nullptr, 0);
			}

			device_context->DrawIndexed(meshes[i].subsets[j].index_count, meshes[i].subsets[j].index_start, 0);
		}
	}
}

void SkinMesh::Render(
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
	const Light &light
	)
{
	//	�v���~�e�B�u���[�h�̐ݒ�
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	���̓��C�A�E�g�̃o�C���h
	device_context->IASetInputLayout(input_layout);

	//	���X�^���C�U�[�̐ݒ�
	if (mode)	device_context->RSSetState(solid_rasterizer_state);
	else		device_context->RSSetState(wire_rasterizer_state);

	//	�V�F�[�_�[(2��)�̐ݒ�
	device_context->VSSetShader(vertex_shader, nullptr, 0);
	device_context->PSSetShader(pixel_shader, nullptr, 0);

	//	�[�x�e�X�g�̐ݒ�
	device_context->OMSetDepthStencilState(depth_stencil, 0);

	if (!is_bone) {
		device_context->IASetInputLayout(test_layout);

		device_context->VSSetShader(test_vs, nullptr, 0);

		coordinate_conversion =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}
	else {
		coordinate_conversion =
		{
			-1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	//	�v���~�e�B�u�̕`��(index�t��)
	//device_context->DrawIndexed(36, 0, 0);
	for (unsigned int i = 0; i < meshes.size(); i++) {
		// ���_�o�b�t�@�̃o�C���h
		UINT stride = sizeof(vertex);
		UINT offset = 0;
		device_context->IASetVertexBuffers(0, 1, &meshes[i].vertex_buffer, &stride, &offset);

		//	�C���f�b�N�X�o�b�t�@�̃o�C���h
		device_context->IASetIndexBuffer(meshes[i].index_buffer, DXGI_FORMAT_R32_UINT, 0);


		for (unsigned int j = 0; j < meshes[i].subsets.size(); j++) {
			//	�萔�o�b�t�@�̍쐬
			//cbuffer cb;
			cbuffer_light cb_light;

			if (meshes[i].skeletal_animation.size() > 0) {
				int frame = static_cast<int>(meshes[i].skeletal_animation.animation_tick / meshes[i].skeletal_animation.sampling_time);

				if (static_cast<unsigned int>(frame) > meshes[i].skeletal_animation.size() - 1) {
					frame = 0;
					meshes[i].skeletal_animation.animation_tick = 0;
				}

				std::vector<bone> &skeletal = meshes[i].skeletal_animation.at(frame);
				size_t number_of_bones = skeletal.size();
				_ASSERT_EXPR(number_of_bones < MAX_BONES, L"'the number_of_bones' exceeds MAX_BONES.");

				/*for (size_t i = 0; i < number_of_bones; i++) {
				XMStoreFloat4x4(&cb.bone_transforms[i], XMLoadFloat4x4(&skeletal.at(i).transform));
				}*/

				meshes[i].skeletal_animation.animation_tick += elapsed_time;
			}

			/*DirectX::XMStoreFloat4x4(&cb.world_view_projection, DirectX::XMLoadFloat4x4(&meshes[i].global_transform) * DirectX::XMLoadFloat4x4(&coordinate_conversion) * DirectX::XMLoadFloat4x4(&world_view_projection));
			DirectX::XMStoreFloat4x4(&cb.world, DirectX::XMLoadFloat4x4(&meshes[i].global_transform) *  DirectX::XMLoadFloat4x4(&coordinate_conversion) * DirectX::XMLoadFloat4x4(&world));*/

			cb_light.world_view_projection = world_view_projection;
			cb_light.world = world;
			cb_light.view = view;
			cb_light.projection = projection;

			//DirectX::XMVECTOR light = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
			//DirectX::XMStoreFloat4(&cb_light.light_direction, light);
			//cb_light.light_point[1] = { light.pos.x,light.pos.y,light.pos.z,0.0f };

			//�Ɩ��̐ݒu
			Light::setLanthanum(lanthanums_pos, lanthanum, 6);
			for (int i = 0; i < 6; i++)
			{
				DirectX::XMFLOAT4 temp;
				temp.x = lanthanum[i].pos.x;
				temp.y = lanthanum[i].pos.y;
				temp.z = lanthanum[i].pos.z;
				temp.w = 0.0f;
				cb_light.light_point[i + 1] = temp;
			}

			cb_light.light_point[0] = { player.pos.x,player.pos.y+3.0f,player.pos.z,0.0f };

			//if (kb.Push('A'))
			//	Move(light.attenution, light.color);
			cb_light.attenution[0] = { 0.3f,0.0f,0.2f,0.0f };
			cb_light.attenution[1] = { 1.0f, 0.0f, 0.2f, 0.0f };

			cb_light.light_color[0] = { light.color.x,light.color.y,light.color.z,1.0f };
			cb_light.light_color[1] = player_light.color;

			cb_light.light_directional = { 0.0f,-0.4f,0.4f,0.0f };
			cb_light.light_directional = { 0.0f,0.0f,0.0f,0.0f };

			//cb_light.obj_color = meshes[i].subsets[j].diffuse.color;
			cb_light.obj_color = material_color;

			for (int i = 0; i < 6; i++)
			{
				cb_light.light_exist[i] = light_exist[i];
			}

			//cb.light_direction = light_direction;

			device_context->UpdateSubresource(const_buffer, 0, nullptr, &cb_light, 0, 0);

			device_context->VSSetConstantBuffers(0, 1, &const_buffer);
			device_context->PSSetConstantBuffers(0, 1, &const_buffer);

			if (meshes[i].subsets[j].diffuse.shader_resource_view) {
				device_context->PSSetShaderResources(0, 1, &meshes[i].subsets[j].diffuse.shader_resource_view);
				device_context->PSSetSamplers(0, 1, &sampler_state);
			}
			else {
				device_context->PSSetShader(test_ps, nullptr, 0);
			}
			device_context->DrawIndexed(meshes[i].subsets[j].index_count, meshes[i].subsets[j].index_start, 0);
		}
	}
}

void SkinMesh::EnergizedRender(
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
	const Light &light
	)
{
	//	�v���~�e�B�u���[�h�̐ݒ�
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	���̓��C�A�E�g�̃o�C���h
	device_context->IASetInputLayout(energized_layout);

	//	���X�^���C�U�[�̐ݒ�
	if (mode)	device_context->RSSetState(solid_rasterizer_state);
	else		device_context->RSSetState(wire_rasterizer_state);

	//	�V�F�[�_�[(2��)�̐ݒ�
	device_context->VSSetShader(energized_vs, nullptr, 0);
	device_context->PSSetShader(energized_ps, nullptr, 0);

	//	�[�x�e�X�g�̐ݒ�
	device_context->OMSetDepthStencilState(depth_stencil, 0);

	if (!is_bone) {
		device_context->IASetInputLayout(energized_layout);

		//device_context->VSSetShader(energized_vs, nullptr, 0);

		coordinate_conversion =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}
	else {
		coordinate_conversion =
		{
			-1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	//	�v���~�e�B�u�̕`��(index�t��)
	//device_context->DrawIndexed(36, 0, 0);
	for (unsigned int i = 0; i < meshes.size(); i++) {
		// ���_�o�b�t�@�̃o�C���h
		UINT stride = sizeof(vertex);
		UINT offset = 0;
		device_context->IASetVertexBuffers(0, 1, &meshes[i].vertex_buffer, &stride, &offset);

		//	�C���f�b�N�X�o�b�t�@�̃o�C���h
		device_context->IASetIndexBuffer(meshes[i].index_buffer, DXGI_FORMAT_R32_UINT, 0);


		for (unsigned int j = 0; j < meshes[i].subsets.size(); j++) {
			//	�萔�o�b�t�@�̍쐬
			//cbuffer cb;
			cbuffer_light cb_ene;

			if (meshes[i].skeletal_animation.size() > 0) {
				int frame = static_cast<int>(meshes[i].skeletal_animation.animation_tick / meshes[i].skeletal_animation.sampling_time);

				if (static_cast<unsigned int>(frame) > meshes[i].skeletal_animation.size() - 1) {
					frame = 0;
					meshes[i].skeletal_animation.animation_tick = 0;
				}

				std::vector<bone> &skeletal = meshes[i].skeletal_animation.at(frame);
				size_t number_of_bones = skeletal.size();
				_ASSERT_EXPR(number_of_bones < MAX_BONES, L"'the number_of_bones' exceeds MAX_BONES.");

				/*for (size_t i = 0; i < number_of_bones; i++) {
				XMStoreFloat4x4(&cb.bone_transforms[i], XMLoadFloat4x4(&skeletal.at(i).transform));
				}*/

				meshes[i].skeletal_animation.animation_tick += elapsed_time;
			}

			/*DirectX::XMStoreFloat4x4(&cb.world_view_projection, DirectX::XMLoadFloat4x4(&meshes[i].global_transform) * DirectX::XMLoadFloat4x4(&coordinate_conversion) * DirectX::XMLoadFloat4x4(&world_view_projection));
			DirectX::XMStoreFloat4x4(&cb.world, DirectX::XMLoadFloat4x4(&meshes[i].global_transform) *  DirectX::XMLoadFloat4x4(&coordinate_conversion) * DirectX::XMLoadFloat4x4(&world));*/

			cb_ene.world_view_projection = world_view_projection;
			cb_ene.world = world;
			cb_ene.view = view;
			cb_ene.projection = projection;

			//�Ɩ��̐ݒu
			Light::setLanthanum(lanthanums_pos, lanthanum, 6);
			for (int i = 0; i < 6; i++)
			{
				DirectX::XMFLOAT4 temp;
				temp.x = lanthanum[i].pos.x;
				temp.y = lanthanum[i].pos.y;
				temp.z = lanthanum[i].pos.z;
				temp.w = 0.0f;
				cb_ene.light_point[i + 1] = temp;
			}
			cb_ene.light_point[0] = { player_light.pos.x,player_light.pos.y + 3.0f,player_light.pos.z,0.0f };
			//DirectX::XMVECTOR light = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
			//DirectX::XMStoreFloat4(&cb_light.light_direction, light);
			//cb_ene.light_point[0] = { light.pos.x,light.pos.y,light.pos.z,0.0f };
			cb_ene.light_directional = { 0.0f,-0.4f,0.4f,0.0f };
			cb_ene.light_directional = { 0.0f,0.0f,0.0f,0.0f };

			//if (kb.Push('A'))
			//	Move(light.attenution, light.color);


			cb_ene.attenution[0] = { light.attenution.x,light.attenution.y,light.attenution.z,0.0f };
			cb_ene.attenution[1] = { player_light.attenution.x,player_light.attenution.y,player_light.attenution.z,0.0f };

			cb_ene.light_color[0] = { light.color.x,light.color.y,light.color.z,1.0f };
			cb_ene.light_color[1] = player_light.color;

			//cb_light.obj_color = meshes[i].subsets[j].diffuse.color;
			cb_ene.obj_color = DirectX::XMFLOAT4(0.0f, 0.8f, 0.9f, 1.0f);

			for (int i = 0; i < 6; i++)
			{
				cb_ene.light_exist[i] = light_exist[i];
			}

			//cb.light_direction = light_direction;

			device_context->UpdateSubresource(const_buffer, 0, nullptr, &cb_ene, 0, 0);

			device_context->VSSetConstantBuffers(0, 1, &const_buffer);
			device_context->PSSetConstantBuffers(0, 1, &const_buffer);

			if (meshes[i].subsets[j].diffuse.shader_resource_view) {
				device_context->PSSetShaderResources(0, 1, &meshes[i].subsets[j].diffuse.shader_resource_view);
				device_context->PSSetSamplers(0, 1, &sampler_state);
			}
			else {
				device_context->PSSetShader(energized_ps, nullptr, 0);
			}
			device_context->DrawIndexed(meshes[i].subsets[j].index_count, meshes[i].subsets[j].index_start, 0);
		}
	}
}

void SkinMesh::Release()
{
	if (const_buffer)				const_buffer->Release();

	for (size_t i = 0; i < meshes.size(); i++) {
		if (meshes[i].index_buffer)		meshes[i].index_buffer->Release();
		if (meshes[i].vertex_buffer)	meshes[i].vertex_buffer->Release();
	}

	if (depth_stencil)				depth_stencil->Release();
	if (solid_rasterizer_state)		solid_rasterizer_state->Release();
	if (wire_rasterizer_state)		wire_rasterizer_state->Release();

	ResourceManager::ReleasePixelShaders(energized_ps);
	ResourceManager::ReleaseVertexShaders(energized_vs, energized_layout);
}

void SkinMesh::LoadRender(
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
	)
{
	//	�v���~�e�B�u���[�h�̐ݒ�
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	���̓��C�A�E�g�̃o�C���h
	device_context->IASetInputLayout(load_layout);

	//	���X�^���C�U�[�̐ݒ�
	if (mode)	device_context->RSSetState(solid_rasterizer_state);
	else		device_context->RSSetState(wire_rasterizer_state);

	//	�V�F�[�_�[(2��)�̐ݒ�
	device_context->VSSetShader(load_vs, nullptr, 0);
	device_context->PSSetShader(load_ps, nullptr, 0);

	//	�[�x�e�X�g�̐ݒ�
	device_context->OMSetDepthStencilState(depth_stencil, 0);

	if (!is_bone) {
		device_context->IASetInputLayout(load_layout);

		device_context->VSSetShader(load_vs, nullptr, 0);

		coordinate_conversion =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}
	else {
		coordinate_conversion =
		{
			-1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	//	�v���~�e�B�u�̕`��(index�t��)
	//device_context->DrawIndexed(36, 0, 0);
	for (unsigned int i = 0; i < meshes.size(); i++) {
		// ���_�o�b�t�@�̃o�C���h
		UINT stride = sizeof(vertex);
		UINT offset = 0;
		device_context->IASetVertexBuffers(0, 1, &meshes[i].vertex_buffer, &stride, &offset);

		//	�C���f�b�N�X�o�b�t�@�̃o�C���h
		device_context->IASetIndexBuffer(meshes[i].index_buffer, DXGI_FORMAT_R32_UINT, 0);


		for (unsigned int j = 0; j < meshes[i].subsets.size(); j++) {
			//	�萔�o�b�t�@�̍쐬
			cbuffer cb;

			//cb.world_view_projection = world_view_projection;
			//cb.world = world;
			if (meshes[i].skeletal_animation.size() > 0) {
				int frame = static_cast<int>(meshes[i].skeletal_animation.animation_tick / meshes[i].skeletal_animation.sampling_time);

				if (static_cast<unsigned int>(frame) > meshes[i].skeletal_animation.size() - 1) {
					frame = 0;
					meshes[i].skeletal_animation.animation_tick = 0;
				}

				std::vector<bone> &skeletal = meshes[i].skeletal_animation.at(frame);
				size_t number_of_bones = skeletal.size();
				_ASSERT_EXPR(number_of_bones < MAX_BONES, L"'the number_of_bones' exceeds MAX_BONES.");

				for (size_t i = 0; i < number_of_bones; i++) {
					XMStoreFloat4x4(&cb.bone_transforms[i], XMLoadFloat4x4(&skeletal.at(i).transform));
				}

				meshes[i].skeletal_animation.animation_tick += elapsed_time;
			}
			if (meshes[i].subsets.size() == 1) {
				meshes[i].global_transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

			}

			DirectX::XMStoreFloat4x4(&cb.world_view_projection, DirectX::XMLoadFloat4x4(&meshes[i].global_transform) * DirectX::XMLoadFloat4x4(&coordinate_conversion) * DirectX::XMLoadFloat4x4(&world_view_projection));
			DirectX::XMStoreFloat4x4(&cb.world, DirectX::XMLoadFloat4x4(&meshes[i].global_transform) *  DirectX::XMLoadFloat4x4(&coordinate_conversion) * DirectX::XMLoadFloat4x4(&world));

			cb.light_direction = light_direction;
			cb.material_color = material_color;
			device_context->UpdateSubresource(const_buffer, 0, nullptr, &cb, 0, 0);
			device_context->VSSetConstantBuffers(0, 1, &const_buffer);

			if (meshes[i].subsets[j].diffuse.shader_resource_view) {
				device_context->PSSetShaderResources(0, 1, &meshes[i].subsets[j].diffuse.shader_resource_view);
				device_context->PSSetSamplers(0, 1, &sampler_state);
			}
			else {
				device_context->PSSetShader(load_ps, nullptr, 0);
			}

			device_context->DrawIndexed(meshes[i].subsets[j].index_count, meshes[i].subsets[j].index_start, 0);
		}
	}
}

void SkinMesh::CreateBuffers(ID3D11Device *device, vertex *vertices, int num_vertices, u_int *indices, int num_indices, mesh &mesh)
{
	HRESULT hr;
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		buffer_desc.ByteWidth = sizeof(vertex)*num_vertices;
		//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = vertices;
		subresource_data.SysMemPitch = 0; //Not use for vertex buffers.mm 
		subresource_data.SysMemSlicePitch = 0; //Not use for vertex buffers.

		hr = device->CreateBuffer(&buffer_desc, &subresource_data, &mesh.vertex_buffer);
		if (FAILED(hr)) return;
	}
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		buffer_desc.ByteWidth = sizeof(u_int)*num_indices;
		//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = indices;
		subresource_data.SysMemPitch = 0; //Not use for index buffers.
		subresource_data.SysMemSlicePitch = 0; //Not use for index buffers.
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, &mesh.index_buffer);
		if (FAILED(hr)) return;
	}
}

void SkinMesh::LoadFBX(ID3D11Device* device, const std::string& fbx_filename)
{
	// Create the FBX SDK manager 
	FbxManager* manager = FbxManager::Create();


	// Create an IOSettings object. IOSROOT is defined in Fbxiosettingspath.h. 
	manager->SetIOSettings(FbxIOSettings::Create(manager, IOSROOT));


	// Create an importer. 
	FbxImporter* importer = FbxImporter::Create(manager, "");


	// Initialize the importer. 
	bool import_status = false;
	import_status = importer->Initialize(fbx_filename.c_str(), -1, manager->GetIOSettings());
	_ASSERT_EXPR_A(import_status, importer->GetStatus().GetErrorString());


	// Create a new scene so it can be populated by the imported file. 
	FbxScene* scene = FbxScene::Create(manager, "");


	// Import the contents of the file into the scene. 
	import_status = importer->Import(scene);
	_ASSERT_EXPR_A(import_status, importer->GetStatus().GetErrorString());

	// Convert mesh, NURBS and patch into triangle mesh 
	fbxsdk::FbxGeometryConverter geometry_converter(manager);
	geometry_converter.Triangulate(scene, /*replace*/true);


	// Fetch node attributes and materials under this node recursively. Currently only mesh.  
	std::vector<FbxNode*> fetched_meshes;
	std::function<void(FbxNode*)> traverse = [&](FbxNode* node) {
		if (node) {
			FbxNodeAttribute *fbx_node_attribute = node->GetNodeAttribute();
			if (fbx_node_attribute) {
				switch (fbx_node_attribute->GetAttributeType()) {
				case FbxNodeAttribute::eMesh:
					fetched_meshes.push_back(node);
					break;
				}
			}
			for (int i = 0; i < node->GetChildCount(); i++)
				traverse(node->GetChild(i));
		}
	};
	traverse(scene->GetRootNode());


	meshes.resize(fetched_meshes.size());
	for (unsigned int i = 0; i < fetched_meshes.size(); i++) {
		FbxMesh *fbx_mesh = fetched_meshes.at(i)->GetMesh();
		mesh &mesh = meshes.at(i);

		// Fetch mesh data 

		std::vector<bone_influences_per_control_point> bone_influences;
		fetch_bone_influences(fbx_mesh, bone_influences);

		if (fbx_mesh->GetDeformerCount(FbxDeformer::eSkin) == 0) {
			is_bone = false;
			culling_clockwise = false;
		}
		else {
			is_bone = true;
			culling_clockwise = true;
		}

		FbxTime::EMode time_mode = fbx_mesh->GetScene()->GetGlobalSettings().GetTimeMode();
		FbxTime frame_time = {};
		frame_time.SetTime(0, 0, 0, 1, 0, time_mode);

		//���f�X�g�f�[�^�Ƃ��� 20 �t���[���ڂ̃|�[�Y���擾���Ă��� 
		//fetch_bone_matrices(fbx_mesh, mesh.skeletal, frame_time * 20); // pose at frame 20  

		fetch_animations(fbx_mesh, mesh.skeletal_animation);

		// Count the polygon count of each material  
		int number_of_materials = fbx_mesh->GetNode()->GetMaterialCount();
		mesh.subsets.resize(number_of_materials);

		if (number_of_materials > 0) {   // Count the faces of each material   
			const int number_of_polygons = fbx_mesh->GetPolygonCount();
			for (int index_of_polygon = 0; index_of_polygon < number_of_polygons; ++index_of_polygon) {
				const u_int material_index = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(index_of_polygon);
				mesh.subsets.at(material_index).index_count += 3;
			}

			// Record the offset (how many vertex)  
			int offset = 0;
			for (subset &subset : mesh.subsets) {
				subset.diffuse.shader_resource_view = nullptr;
				subset.diffuse.tex2d_desc = nullptr;

				subset.index_start = offset;
				offset += subset.index_count;    // This will be used as counter in the following procedures, reset to zero    
				subset.index_count = 0;
			}
		}

		std::vector<vertex> vertices; // Vertex buffer 
		std::vector<u_int> indices;  // Index buffer 
		u_int vertex_count = 0;


		const FbxVector4 *array_of_control_points = fbx_mesh->GetControlPoints();
		const int number_of_polygons = fbx_mesh->GetPolygonCount();
		indices.resize(number_of_polygons * 3);

		for (int index_of_polygon = 0; index_of_polygon < number_of_polygons; index_of_polygon++) {
			// The material for current face. 
			int index_of_material = 0;
			if (number_of_materials > 0) {
				index_of_material = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(index_of_polygon);
			}   // Where should I save the vertex attribute index, according to the material 

			subset &subset = mesh.subsets.at(index_of_material);
			const int index_offset = subset.index_start + subset.index_count;

			for (int index_of_vertex = 0; index_of_vertex < 3; index_of_vertex++) {
				vertex vertex;
				const int index_of_control_point = fbx_mesh->GetPolygonVertex(index_of_polygon, index_of_vertex);
				vertex.position.x = static_cast<float>(array_of_control_points[index_of_control_point][0]);
				vertex.position.y = static_cast<float>(array_of_control_points[index_of_control_point][1]);
				vertex.position.z = static_cast<float>(array_of_control_points[index_of_control_point][2]);

				FbxVector4 normal;
				fbx_mesh->GetPolygonVertexNormal(index_of_polygon, index_of_vertex, normal);
				vertex.normal.x = static_cast<float>(normal[0]);
				vertex.normal.y = static_cast<float>(normal[1]);
				vertex.normal.z = static_cast<float>(normal[2]);

				FbxStringList uv_names;
				fbx_mesh->GetUVSetNames(uv_names);

				if (fbx_mesh->GetUVLayerCount() > 0) {
					FbxVector2 uv;
					bool unmapped_uv;
					fbx_mesh->GetPolygonVertexUV(index_of_polygon, index_of_vertex, uv_names[0], uv, unmapped_uv);
					vertex.uv.x = static_cast<float>(uv[0]);
					vertex.uv.y = 1.0f - static_cast<float>(uv[1]);
				}

				bone_influences_per_control_point bone = bone_influences[index_of_control_point];

				for (size_t index_of_influence = 0; index_of_influence < bone.size(); index_of_influence++)
				{

					if (index_of_influence < MAX_BONE_INFLUENCES)
					{
						vertex.bone_weights[index_of_influence] = bone.at(index_of_influence).weight;
						vertex.bone_indices[index_of_influence] = bone.at(index_of_influence).index;
					}
				}

				vertices.push_back(vertex);
				//indices.push_back(vertex_count);  

				indices.at(index_offset + index_of_vertex) = static_cast<u_int>(vertex_count);
				vertex_count += 1;
			}
			subset.index_count += 3;
		}

		CreateBuffers(device, vertices.data(), vertices.size(), indices.data(), indices.size(), mesh);

		FbxAMatrix global_transform = fbx_mesh->GetNode()->EvaluateGlobalTransform(0);
		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				mesh.global_transform(row, column) = static_cast<float>(global_transform[row][column]);
			}
		}

		// Fetch material properties.

		for (int index_of_material = 0; index_of_material < number_of_materials; ++index_of_material)
		{
			subset &subset = mesh.subsets.at(index_of_material);

			const FbxSurfaceMaterial *surface_material = fbx_mesh->GetNode()->GetMaterial(index_of_material);

			const FbxProperty property = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			const FbxProperty factor = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
			if (property.IsValid() && factor.IsValid())
			{
				FbxDouble3 color = property.Get<FbxDouble3>();
				double f = factor.Get<FbxDouble>();
				subset.diffuse.color.x = static_cast<float>(color[0] * f);
				subset.diffuse.color.y = static_cast<float>(color[1] * f);
				subset.diffuse.color.z = static_cast<float>(color[2] * f);
				subset.diffuse.color.w = 1.0f;
			}

			if (property.IsValid()) {
				const int number_of_textures = property.GetSrcObjectCount<FbxFileTexture>();
				if (number_of_textures)
				{
					const FbxFileTexture* file_texture = property.GetSrcObject<FbxFileTexture>();
					if (file_texture)
					{
						//file_texture->GetName(); �Ȃ���������������
						//std::string filename = file_texture->GetRelativeFileName();
						std::string filename = file_texture->GetFileName();

						filename.erase(filename.begin(), filename.begin() + filename.find_last_of("/") + 1);

						std::string file_name = fbx_filename.substr(0, fbx_filename.find_last_of("/") + 1) + filename;

						std::wstring name(file_name.begin(), file_name.end());

						assert(ResourceManager::LoadShaderResourceView(device, name.c_str(), &subset.diffuse.shader_resource_view, &subset.diffuse.tex2d_desc));
					}
				}
			}
		}
	}

	manager->Destroy();
}

//�{�[���e���x�� FBX �f�[�^����擾����֐�
void fetch_bone_influences(const FbxMesh *fbx_mesh, std::vector<bone_influences_per_control_point> &influences)
{
	const int number_of_control_points = fbx_mesh->GetControlPointsCount();
	influences.resize(number_of_control_points);

	const int number_of_deformers = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int index_of_deformer = 0; index_of_deformer < number_of_deformers; ++index_of_deformer) {
		FbxSkin *skin = static_cast<FbxSkin *>(fbx_mesh->GetDeformer(index_of_deformer, FbxDeformer::eSkin));

		const int number_of_clusters = skin->GetClusterCount();
		for (int index_of_cluster = 0; index_of_cluster < number_of_clusters; ++index_of_cluster) {
			FbxCluster* cluster = skin->GetCluster(index_of_cluster);

			const int number_of_control_point_indices = cluster->GetControlPointIndicesCount();
			const int *array_of_control_point_indices = cluster->GetControlPointIndices();
			const double *array_of_control_point_weights = cluster->GetControlPointWeights();

			for (int i = 0; i < number_of_control_point_indices; ++i) {
				bone_influences_per_control_point &influences_per_control_point = influences.at(array_of_control_point_indices[i]);
				bone_influence influence;
				influence.index = index_of_cluster;
				influence.weight = static_cast<float>(array_of_control_point_weights[i]);
				influences_per_control_point.push_back(influence);
			}
		}
	}
}

//�{�[���s��� FBX �f�[�^����擾����֐�
void fetch_bone_matrices(FbxMesh *fbx_mesh, std::vector<SkinMesh::bone> &skeletal, FbxTime time)
{
	const int number_of_deformers = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int index_of_deformer = 0; index_of_deformer < number_of_deformers; ++index_of_deformer) {
		FbxSkin *skin = static_cast<FbxSkin *>(fbx_mesh->GetDeformer(index_of_deformer, FbxDeformer::eSkin));

		const int number_of_clusters = skin->GetClusterCount();
		skeletal.resize(number_of_clusters);
		for (int index_of_cluster = 0; index_of_cluster < number_of_clusters; ++index_of_cluster) {
			SkinMesh::bone &bone = skeletal.at(index_of_cluster);

			FbxCluster *cluster = skin->GetCluster(index_of_cluster);

			// this matrix trnasforms coordinates of the initial pose from mesh space to global space 
			FbxAMatrix reference_global_init_position;
			cluster->GetTransformMatrix(reference_global_init_position);

			// this matrix trnasforms coordinates of the initial pose from bone space to global space   
			FbxAMatrix cluster_global_init_position;
			cluster->GetTransformLinkMatrix(cluster_global_init_position);

			// this matrix trnasforms coordinates of the current pose from bone space to global space  
			FbxAMatrix cluster_global_current_position;
			cluster_global_current_position = cluster->GetLink()->EvaluateGlobalTransform(time);

			// this matrix trnasforms coordinates of the current pose from mesh space to global space    
			FbxAMatrix reference_global_current_position;
			reference_global_current_position = fbx_mesh->GetNode()->EvaluateGlobalTransform(time);

			// Matrices are defined using the Column Major scheme. When a FbxAMatrix represents a transformation 
			// (translation, rotation and scale), the last row of the matrix represents the translation part of the 
			// transformation.   
			FbxAMatrix transform = reference_global_current_position.Inverse() * cluster_global_current_position * cluster_global_init_position.Inverse() * reference_global_init_position;

			// convert FbxAMatrix(transform) to XMDLOAT4X4(bone.transform) 
			fbxamatrix_to_xmfloat4x4(transform, bone.transform);
		}
	}
}

//�A�j���[�V�������擾����֐�
void fetch_animations(FbxMesh *fbx_mesh, SkinMesh::skeletal_animation &skeletal_animation, u_int sampling_rate)
{
	// Get the list of all the animation stack.  
	FbxArray<FbxString *> array_of_animation_stack_names;
	fbx_mesh->GetScene()->FillAnimStackNameArray(array_of_animation_stack_names);

	// Get the number of animations.  
	int number_of_animations = array_of_animation_stack_names.Size();

	if (number_of_animations > 0) {    // Get the FbxTime per animation's frame.    
		FbxTime::EMode time_mode = fbx_mesh->GetScene()->GetGlobalSettings().GetTimeMode();
		FbxTime frame_time;
		frame_time.SetTime(0, 0, 0, 1, 0, time_mode);

		sampling_rate = sampling_rate > 0 ? sampling_rate : static_cast<u_int>(frame_time.GetFrameRate(time_mode));
		float sampling_time = 1.0f / sampling_rate;    skeletal_animation.sampling_time = sampling_time;
		skeletal_animation.animation_tick = 0.0f;

		FbxString *animation_stack_name = array_of_animation_stack_names.GetAt(0);
		FbxAnimStack * current_animation_stack = fbx_mesh->GetScene()->FindMember<FbxAnimStack>(animation_stack_name->Buffer());
		fbx_mesh->GetScene()->SetCurrentAnimationStack(current_animation_stack);

		FbxTakeInfo *take_info = fbx_mesh->GetScene()->GetTakeInfo(animation_stack_name->Buffer());
		FbxTime start_time = take_info->mLocalTimeSpan.GetStart();
		FbxTime end_time = take_info->mLocalTimeSpan.GetStop();

		FbxTime sampling_step;
		sampling_step.SetTime(0, 0, 1, 0, 0, time_mode);
		sampling_step = static_cast<FbxLongLong>(sampling_step.Get() * sampling_time);

		for (FbxTime current_time = start_time; current_time < end_time; current_time += sampling_step) {
			SkinMesh::skeletal skeletal;
			fetch_bone_matrices(fbx_mesh, skeletal, current_time);
			skeletal_animation.push_back(skeletal);
		}
	}

	for (int i = 0; i < number_of_animations; i++) {
		delete array_of_animation_stack_names[i];
	}
}


void fbxamatrix_to_xmfloat4x4(const FbxAMatrix &fbxamatrix, DirectX::XMFLOAT4X4 &xmfloat4x4)
{
	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			xmfloat4x4.m[row][column] = static_cast<float>(fbxamatrix[row][column]);
		}
	}
}