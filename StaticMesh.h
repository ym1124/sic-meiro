#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

class StaticMesh
{
private:
	void Init(ID3D11Device*, const wchar_t *obj_filename, const wchar_t *mtl_filename);
	void Release();

public:
	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;

		DirectX::XMFLOAT2 texcoord;
	};

	struct cbuffer
	{
		DirectX::XMFLOAT4X4 world_view_projection;	//ワールド・ビュー・プロジェクション合成行列 
		DirectX::XMFLOAT4X4 world;					//ワールド変換行列 
		DirectX::XMFLOAT4 material_color;			//材質色 
		DirectX::XMFLOAT4 light_direction;			//ライト進行方向 
	};

	ID3D11VertexShader		*vertex_shader;
	ID3D11PixelShader		*pixel_shader;
	ID3D11InputLayout		*input_layout;

	ID3D11Buffer			*vertex_buffer;
	ID3D11Buffer			*index_buffer;
	ID3D11Buffer			*const_buffer;

	ID3D11RasterizerState	*wire_rasterizer_state;
	ID3D11RasterizerState	*solid_rasterizer_state;

	ID3D11DepthStencilState	*depth_stencil;

	struct subset
	{
		std::wstring usemtl;
		u_int index_start = 0;
		u_int index_count = 0;
	};

	std::vector<subset> subsets;

	ID3D11SamplerState		*sampler_state;

	struct material
	{
		std::wstring newmtl;
		DirectX::XMFLOAT3 Ka = { 0.2f,0.2f,0.2f };
		DirectX::XMFLOAT3 Kd = { 0.8f,0.8f,0.8f };
		DirectX::XMFLOAT3 Ks = { 1.0f,1.0f,1.0f };
		u_int illum = 1;
		std::wstring map_Kd;

		ID3D11ShaderResourceView *shader_resource_view;
		D3D11_TEXTURE2D_DESC	*tex2d_desc;
		ID3D11SamplerState		*sampler_state;
	};

	std::vector<material> materials;

	StaticMesh(ID3D11Device* device, const wchar_t *obj_filename, const wchar_t *mtl_filename) { Init(device, obj_filename, mtl_filename); }
	~StaticMesh() { Release(); }

	void CreateBuffers(ID3D11Device *device, vertex *vertices, int num_vertices, u_int *indices, int num_indices);

	void LoadOBJ(ID3D11Device *device, std::vector<subset> *psubset, const wchar_t *obj_filename);
	void LoadMTL(ID3D11Device *device, std::vector<material> *pmaterial, const wchar_t *mtl_filename);

	void Render(ID3D11DeviceContext *immediate_context, const DirectX::XMFLOAT4X4 &world_view_projection, const DirectX::XMFLOAT4X4 &world, const DirectX::XMFLOAT4 &light_direction, const DirectX::XMFLOAT4 &material_color, bool wireframe);
};