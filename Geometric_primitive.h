#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class GeometricPrimitive
{
protected:
	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
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
	ID3D11Buffer			*constant_buffer;

	ID3D11RasterizerState	*wireframe_rasterizer_state;
	ID3D11RasterizerState	*solid_rasterizer_state;

	ID3D11DepthStencilState	*depth_stencil_state;
	
	int						numIndices;
	bool					isMakeBuffer;

	bool	CreateBuffers(ID3D11Device* device,
		vertex* vertices, int numV,
		unsigned int* indices, int numI);

	void	_init(ID3D11Device* device,
		const char* vsName, D3D11_INPUT_ELEMENT_DESC *inputElementDescs, int numElement,
		const char* psName);

protected:
	GeometricPrimitive() :
		vertex_shader(nullptr), pixel_shader(nullptr), input_layout(nullptr),
		vertex_buffer(nullptr), index_buffer(nullptr), constant_buffer(nullptr),
		wireframe_rasterizer_state(nullptr), solid_rasterizer_state(nullptr),
		depth_stencil_state(nullptr), numIndices(0), isMakeBuffer(false)
	{}

public:
	GeometricPrimitive(ID3D11Device* device) :
		vertex_shader(nullptr), pixel_shader(nullptr), input_layout(nullptr),
		vertex_buffer(nullptr), index_buffer(nullptr), constant_buffer(nullptr),
		wireframe_rasterizer_state(nullptr), solid_rasterizer_state(nullptr),
		depth_stencil_state(nullptr), numIndices(0), isMakeBuffer(false)
	{
		// 入力レイアウトの定義
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                         D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(DirectX::XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		const char* vsName = "Shader/geometric_primitive_vs.cso";
		const char* psName = "Shader/geometric_primitive_ps.cso";

		//	初期化
		_init(device, vsName, layout, ARRAYSIZE(layout), psName);
	}
	GeometricPrimitive(ID3D11Device* device,
		const char* vsName, D3D11_INPUT_ELEMENT_DESC *inputElementDescs, int numElement,
		const char* psName) :
		vertex_shader(nullptr), pixel_shader(nullptr), input_layout(nullptr),
		vertex_buffer(nullptr), index_buffer(nullptr), constant_buffer(nullptr),
		wireframe_rasterizer_state(nullptr), solid_rasterizer_state(nullptr),
		depth_stencil_state(nullptr), numIndices(0), isMakeBuffer(false)

	{
		_init(device, vsName, inputElementDescs, numElement, psName);
	}
	virtual ~GeometricPrimitive() { Release(); }

	virtual void Render(
		ID3D11DeviceContext* device_context,
		const DirectX::XMFLOAT4X4& world_view_projection,
		const DirectX::XMFLOAT4X4& world,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& material_color = DirectX::XMFLOAT4(1, 1, 1, 1),
		bool bSolid = true);

	////////////////////////自作///////////////////////

	//GeometricPrimitive(ID3D11Device*);
	//GeometricPrimitive(ID3D11Device*,const char* vs_filename,const char* ps_filename);
	//
	//~GeometricPrimitive() { Release(); }
	//
	//void Render(
	//	ID3D11DeviceContext *device_context,				//デバイスコンテキスト 
	//	const DirectX::XMFLOAT4X4 & world_view_projection,	//ワールド・ビュー・プロジェクション合成行列 
	//	const DirectX::XMFLOAT4X4 & world,					//ワールド変換行列 
	//	const DirectX::XMFLOAT4 & light_direction,			//ライト進行方向 
	//	const DirectX::XMFLOAT4 & material_color,			//材質色 
	//	bool mode											//線・塗りつぶし描画フラグ 
	//);

	/////////////////////////////////////////////////

	void Release();
};

class GeometricRect : public GeometricPrimitive
{
public:
	GeometricRect(ID3D11Device* device);
	~GeometricRect() {};
};


class GeometricBoard : public GeometricPrimitive
{
public:
	GeometricBoard(ID3D11Device* device);
	~GeometricBoard() {};
};


class GeometricCube : public GeometricPrimitive
{
public:
	GeometricCube(ID3D11Device* device);
	~GeometricCube() {};
};


class GeometricSphere : public GeometricPrimitive
{
public:
	GeometricSphere(ID3D11Device* device, u_int slices = 16, u_int stacks = 16);
	~GeometricSphere() {};
};


class GeometricSphere2 : public GeometricPrimitive
{
private:
	int		numVertices;
	inline	vertex _makeVertex(const DirectX::XMFLOAT3& p);
public:
	//	形状は slices = (div-1) * 4, stacks = (div-1) * 2 と同じ
	GeometricSphere2(ID3D11Device* device, u_int div = 8);
	~GeometricSphere2() {};

	virtual void Render(ID3D11DeviceContext* context,
		const DirectX::XMFLOAT4X4& wvp,
		const DirectX::XMFLOAT4X4& world,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& material_color = DirectX::XMFLOAT4(1, 1, 1, 1),
		bool bSolid = true);
};