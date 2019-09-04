#include "..\Header\StaticMesh.h"

#include <memory>
#include <vector>
#include <fstream>

#include "..\Header\ResourceManager.h"

void StaticMesh::Init(ID3D11Device* device, const wchar_t *obj_filename, const wchar_t *mtl_filename)
{
	vertex_shader = nullptr;
	input_layout = nullptr;

	vertex_buffer = nullptr;
	index_buffer = nullptr;
	const_buffer = nullptr;

	wire_rasterizer_state = nullptr;
	solid_rasterizer_state = nullptr;

	depth_stencil = nullptr;

	sampler_state = nullptr;

	LoadOBJ(device, &subsets, obj_filename);

	LoadMTL(device, &materials, mtl_filename);

	HRESULT hr = S_OK;

	//���̓��C�A�E�g�I�u�W�F�N�g�̐��� 
	D3D11_INPUT_ELEMENT_DESC VertexDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//���_�V�F�[�_�[�I�u�W�F�N�g�̐��� 
	if (!ResourceManager::LoadVertexShaders(device, "Shader/static_mesh_vs.cso", VertexDesc, ARRAYSIZE(VertexDesc), &vertex_shader, &input_layout)) {
		return;
	}

	//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�̐��� 
	if (!ResourceManager::LoadPixelShaders(device, "Shader/static_mesh_ps.cso", &pixel_shader)) {
		return;
	}

	// create rasterizer state : solid mode
	{
		D3D11_RASTERIZER_DESC rasterizer_desc = {};
		rasterizer_desc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
		rasterizer_desc.CullMode = D3D11_CULL_BACK; //D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK   
		rasterizer_desc.FrontCounterClockwise = FALSE;
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
		rasterizer_desc.FrontCounterClockwise = FALSE;
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

	for (auto &material : materials) {
		//hr = device->CreateSamplerState(&smpDesc, &material.sampler_state);
		//if (FAILED(hr)) return;

		if (!ResourceManager::LoadShaderResourceView(device, material.map_Kd.c_str(), &material.shader_resource_view, &material.tex2d_desc)) {
			return;
		}

		material.sampler_state = nullptr;
	}

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

void StaticMesh::Render(ID3D11DeviceContext *immediate_context, const DirectX::XMFLOAT4X4 &world_view_projection, const DirectX::XMFLOAT4X4 &world, const DirectX::XMFLOAT4 &light_direction, const DirectX::XMFLOAT4 &material_color, bool wireframe)
{
	u_int stride = sizeof(vertex);
	u_int offset = 0;
	immediate_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
	immediate_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
	immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediate_context->IASetInputLayout(input_layout);

	immediate_context->VSSetShader(vertex_shader, nullptr, 0);
	immediate_context->PSSetShader(pixel_shader, nullptr, 0);

	immediate_context->OMSetDepthStencilState(depth_stencil, 0);
	if (wireframe)
	{
		immediate_context->RSSetState(wire_rasterizer_state);
	}
	else
	{
		immediate_context->RSSetState(solid_rasterizer_state);
	}

	for (unsigned int i = 0; i < materials.size(); i++) {
		cbuffer data = {};
		data.world_view_projection = world_view_projection;
		data.world = world;
		data.light_direction = light_direction;
		data.material_color.x = material_color.x*materials[i].Kd.x;
		data.material_color.y = material_color.x*materials[i].Kd.y;
		data.material_color.z = material_color.x*materials[i].Kd.z;
		data.material_color.w = material_color.w;

		immediate_context->UpdateSubresource(const_buffer, 0, 0, &data, 0, 0);

		immediate_context->VSSetConstantBuffers(0, 1, &const_buffer);

		immediate_context->PSSetShaderResources(0, 1, &materials[i].shader_resource_view);
		immediate_context->PSSetSamplers(0, 1, &sampler_state);

		for (auto &subset : subsets) {
			if (materials[i].newmtl == subset.usemtl) {
				immediate_context->DrawIndexed(subset.index_count, subset.index_start, 0);
			}
		}
	}
}

void StaticMesh::Release()
{
	if (const_buffer)				const_buffer->Release();
	if (index_buffer)				index_buffer->Release();
	if (vertex_buffer)				vertex_buffer->Release();

	if (depth_stencil)				depth_stencil->Release();
	if (solid_rasterizer_state)		solid_rasterizer_state->Release();
	if (wire_rasterizer_state)		wire_rasterizer_state->Release();

	ResourceManager::ReleasePixelShaders(pixel_shader);
	ResourceManager::ReleaseVertexShaders(vertex_shader, input_layout);
}

void StaticMesh::CreateBuffers(ID3D11Device *device, vertex *vertices, int num_vertices, u_int *indices, int num_indices)
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

		hr = device->CreateBuffer(&buffer_desc, &subresource_data, &vertex_buffer);
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
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, &index_buffer);
		if (FAILED(hr)) return;
	}
}

void StaticMesh::LoadOBJ(ID3D11Device *device, std::vector<subset> *psubset, const wchar_t *obj_filename)
{
	std::wifstream fin(obj_filename);
	_ASSERT_EXPR(fin, L"'OBJ file not fuund.");

	std::vector<vertex> vertices;
	std::vector<u_int> indices;
	u_int current_index = 0;

	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<DirectX::XMFLOAT2> texcoords;

	bool is_obj = false;

	subset sub = {};

	wchar_t command[256];
	while (fin)
	{
		fin >> command;

		if (0 == wcscmp(command, L"usemtl"))
		{
			if (is_obj) {
				psubset->push_back(sub);
				sub.index_start = sub.index_count;
			}

			is_obj = true;

			wchar_t usemtl[256];
			fin >> usemtl;
			sub.usemtl = usemtl;

			fin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(command, L"v"))
		{
			float x, y, z;
			fin >> x >> y >> z;
			positions.push_back(DirectX::XMFLOAT3(x, y, z));
			fin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(command, L"vt"))
		{
			float u = 0.0f;
			float v = 0.0f;
			fin >> u >> v;
			texcoords.push_back(DirectX::XMFLOAT2(u, 1 - v));
			fin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(command, L"vn"))
		{
			FLOAT i, j, k;
			fin >> i >> j >> k;
			normals.push_back(DirectX::XMFLOAT3(i, j, k));
			fin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(command, L"f"))
		{
			for (u_int i = 0; i < 3; i++)
			{
				vertex vertex;
				u_int v, vt, vn;

				fin >> v;
				vertex.position = positions[v - 1];
				if (L'/' == fin.peek())
				{
					fin.ignore();
					if (L'/' != fin.peek())
					{
						fin >> vt;
						vertex.texcoord = texcoords[vt - 1];
					}
					if (L'/' == fin.peek())
					{
						fin.ignore();
						fin >> vn;
						vertex.normal = normals[vn - 1];
					}
				}
				vertices.push_back(vertex);
				indices.push_back(current_index++);
			}
			sub.index_count = current_index;
			fin.ignore(1024, L'\n');
		}
		else
		{
			fin.ignore(1024, L'\n');
		}
	}
	fin.close();
	if (is_obj) psubset->push_back(sub);

	CreateBuffers(device, vertices.data(), vertices.size(), indices.data(), indices.size());
}

void StaticMesh::LoadMTL(ID3D11Device *device, std::vector<material> *pmaterial, const wchar_t *mtl_filename)
{
	std::wifstream mfin(mtl_filename);
	_ASSERT_EXPR(mfin, L"'MTL file not fuund.");
	wchar_t tex_name[256];
	wchar_t com[256];

	bool is_material = false;

	material mat = {};

	while (mfin)
	{
		mfin >> com;	//�L�[���[�h�ǂݍ���

		if (0 == wcscmp(com, L"newmtl"))	//�}�e���A����
		{
			if (is_material) pmaterial->push_back(mat);

			is_material = true;

			wchar_t newmtl[256];
			mfin >> newmtl;
			mat.newmtl = newmtl;

			mfin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(com, L"Ka"))	//Ka�@�A���r�G���g
		{
			FLOAT i, j, k;
			mfin >> i >> j >> k;

			mat.Ka = DirectX::XMFLOAT3(i, j, k);

			mfin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(com, L"Kd"))	//Kd�@�f�B�t���[�Y
		{
			FLOAT r, g, b;
			mfin >> r >> g >> b;

			mat.Kd = DirectX::XMFLOAT3(r, g, b);

			mfin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(com, L"Ks"))	//Ks�@�X�y�L�����[
		{
			FLOAT i, j, k;
			mfin >> i >> j >> k;

			mat.Ks = DirectX::XMFLOAT3(i, j, k);

			mfin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(com, L"illum"))
		{
			u_int i;
			mfin >> i;

			mat.illum = i;

			mfin.ignore(1024, L'\n');

		}
		else if (0 == wcscmp(com, L"map_Kd"))	//map_Kd�@�e�N�X�`��
		{
			mfin >> tex_name;

			std::wstring file_name = mtl_filename;

			mat.map_Kd = file_name.substr(0, file_name.find_last_of(L"/") + 1);
			mat.map_Kd += tex_name;

			mfin.ignore(1024, L'\n');
		}
		else
		{
			mfin.ignore(1024, L'\n');
		}
	}
	mfin.close();
	if (is_material) pmaterial->push_back(mat);
}