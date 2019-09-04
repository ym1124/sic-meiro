#include "..\Header\ResourceManager.h"
#include "..\Header\Sprite.h"

Sprite::Sprite(ID3D11Device* device, const wchar_t* tex_filename)
{
	HRESULT hr = S_OK;

	//頂点情報のセット

	vertex vertices[] = {
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMFLOAT2(0, 0) },
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMFLOAT2(0, 0) },
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMFLOAT2(0, 0) },
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMFLOAT2(0, 0) },
	};

	//頂点バッファオブジェクトの生成
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//CPU アクセスのフラグ
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pSysMem = vertices;
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	hr = device->CreateBuffer(&bufferDesc, &subResourceData, &buffer);
	if (FAILED(hr)) return;

	/*入力レイアウト　：入力されたバッファーがどのようなデータ構造なのかを記述するもの*/

	//入力レイアウトオブジェクトの生成
	D3D11_INPUT_ELEMENT_DESC VertexDesc[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//頂点シェーダーオブジェクトの生成

	if (!ResourceManager::LoadVertexShaders(device, "Shader/sprite_vs.cso", VertexDesc, ARRAYSIZE(VertexDesc), &vertex_shader, &input_layout)) {
		return;
	}

	//ピクセルシェーダーオブジェクトの生成

	if (!ResourceManager::LoadPixelShaders(device, "Shader/sprite_ps.cso", &pixel_shader)) {
		return;
	}

	//ラスタライザーステートの設定

	D3D11_RASTERIZER_DESC rasterizerDesc = {};

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;	// ワイヤーフレーム
	rasterizerDesc.CullMode = D3D11_CULL_NONE;		// 裏面ポリゴンをカリングします
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = FALSE;
	rasterizerDesc.DepthClipEnable = FALSE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;


	hr = device->CreateRasterizerState(&rasterizerDesc, &rasterizer_state);
	if (FAILED(hr)) return;

	//画像ファイルのロードとシェーダー リソース ビュー オブジェクト(ID3D11ShaderResourceView)の生成 

	if (!ResourceManager::LoadShaderResourceView(device, tex_filename, &shader_resource_view, &tex2d_desc)) {
		return;
	}
	//サンプラーの作成

	/*テクスチャがオブジェクトに張り付く時には、引き延ばされたり縮められたりします。
	拡大縮小される時の補間方法などを指定するのがサンプラーの役割*/

	D3D11_SAMPLER_DESC smpDesc = {};

	//::ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
	//smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//拡大・縮小時の色の取得方法
	//smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;		//UV座標が0～1の範囲外の場合の色の取得方法
	//smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;		//↑
	//smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;		//↑

	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
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

	//	深度ステンシルStateの作成
	D3D11_DEPTH_STENCIL_DESC dssDesc = {};
	dssDesc.DepthEnable = FALSE;				//	深度テスト無効
	dssDesc.StencilEnable = FALSE;				//	ついでにステンシルも無効
	hr = device->CreateDepthStencilState(&dssDesc, &depth_stencil);
	if (FAILED(hr)) return;
}

void Sprite::Render(ID3D11DeviceContext* device_context,
	float dx, float dy, float dw, float dh,
	float tex_dx, float tex_dy, float tex_dw, float tex_dh,
	float angle, float r, float g, float b, float a)
{
	// dx, dy : Coordinate of sprite's left-top corner in screen space
	// dw, dh : Size of sprite in screen space
	// angle : Raotation angle (Rotation centre is sprite's centre), Unit is degree
	//r, g, b, a : Color of sprite's each vertices  

	D3D11_VIEWPORT viewport;
	UINT num_viewports = 1;
	device_context->RSGetViewports(&num_viewports, &viewport);
	float screen_width = viewport.Width;
	float screen_height = viewport.Height;

	UINT strides = sizeof(vertex);
	UINT offsets = 0;

	// UNIT.03 
	DirectX::XMFLOAT2 position[4] = {
		{ dx,		dy },	//left_top
		{ dx + dw,	dy },	//right_top
		{ dx,		dy + dh },	//left_down
		{ dx + dw,	dy + dh }	//right_down
	};

	const DirectX::XMFLOAT2 center = { dx + dw * 0.5f,dy + dh * 0.5f };
	//position[0].x -= center.x;
	//position[0].y -= center.y;
	//position[1].x -= center.x;
	//position[1].y -= center.y;
	//position[2].x -= center.x;
	//position[2].y -= center.y;
	//position[3].x -= center.x;
	//position[3].y -= center.y;

	for (int i = 0; i < 4; i++) {
		position[i].x -= center.x;
		position[i].y -= center.y;
	}

	float rx, ry;
	float cos = cosf(angle*0.01745f);
	float sin = sinf(angle*0.01745f);
	//rx = position[0].x;
	//ry = position[0].y;
	//position[0].x = cos*rx + -sin*ry;
	//position[0].y = sin*rx + cos*ry;
	//rx = position[1].x;
	//ry = position[1].y;
	//position[1].x = cos*rx + -sin*ry;
	//position[1].y = sin*rx + cos*ry;
	//rx = position[2].x;
	//ry = position[2].y;
	//position[2].x = cos*rx + -sin*ry;
	//position[2].y = sin*rx + cos*ry;
	//rx = position[3].x;
	//ry = position[3].y;
	//position[3].x = cos*rx + -sin*ry;
	//position[3].y = sin*rx + cos*ry;

	for (int i = 0; i < 4; i++) {
		rx = position[i].x;
		ry = position[i].y;
		position[i].x = cos*rx + -sin*ry;
		position[i].y = sin*rx + cos*ry;
	}

	//position[0].x += center.x;
	//position[0].y += center.y;
	//position[1].x += center.x;
	//position[1].y += center.y;
	//position[2].x += center.x;
	//position[2].y += center.y;
	//position[3].x += center.x;
	//position[3].y += center.y;

	for (int i = 0; i < 4; i++) {
		position[i].x += center.x;
		position[i].y += center.y;
	}

	// Convert to NDC space
	//position[0].x = 2.0f*position[0].x / screen_width - 1.0f;
	//position[0].y = 1.0f - 2.0f*position[0].y / screen_height;
	//position[1].x = 2.0f*position[1].x / screen_width - 1.0f;
	//position[1].y = 1.0f - 2.0f*position[1].y / screen_height;
	//position[2].x = 2.0f*position[2].x / screen_width - 1.0f;
	//position[2].y = 1.0f - 2.0f*position[2].y / screen_height;
	//position[3].x = 2.0f*position[3].x / screen_width - 1.0f;
	//position[3].y = 1.0f - 2.0f*position[3].y / screen_height;

	for (int i = 0; i < 4; i++) {
		position[i].x = 2.0f*position[i].x / screen_width - 1.0f;
		position[i].y = 1.0f - 2.0f*position[i].y / screen_height;
	}

	HRESULT hr = S_OK;
	D3D11_MAP map = D3D11_MAP_WRITE_DISCARD;
	D3D11_MAPPED_SUBRESOURCE mapped_buffer = {};

	hr = device_context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);
	if (FAILED(hr)) return;

	vertex *vertices = static_cast<vertex *>(mapped_buffer.pData);
	//vertices[0].position.x = position[0].x;
	//vertices[0].position.y = position[0].y;
	//vertices[1].position.x = position[1].x;
	//vertices[1].position.y = position[1].y;
	//vertices[2].position.x = position[2].x;
	//vertices[2].position.y = position[2].y;
	//vertices[3].position.x = position[3].x;
	//vertices[3].position.y = position[3].y;
	//vertices[0].position.z = vertices[1].position.z = vertices[2].position.z = vertices[3].position.z = 0.0f;

	for (int i = 0; i < 4; i++) {
		vertices[i].position.x = position[i].x;
		vertices[i].position.y = position[i].y;
		vertices[i].position.z = 0.0f;
	}

	DirectX::XMFLOAT4 color(r, g, b, a);
	vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

	vertices[0].texcoord.x = static_cast<FLOAT>(tex_dx) / tex2d_desc->Width;
	vertices[0].texcoord.y = static_cast<FLOAT>(tex_dy) / tex2d_desc->Height;
	vertices[1].texcoord.x = static_cast<FLOAT>(tex_dx + tex_dw) / tex2d_desc->Width;
	vertices[1].texcoord.y = static_cast<FLOAT>(tex_dy) / tex2d_desc->Height;
	vertices[2].texcoord.x = static_cast<FLOAT>(tex_dx) / tex2d_desc->Width;
	vertices[2].texcoord.y = static_cast<FLOAT>(tex_dy + tex_dh) / tex2d_desc->Height;
	vertices[3].texcoord.x = static_cast<FLOAT>(tex_dx + tex_dw) / tex2d_desc->Width;
	vertices[3].texcoord.y = static_cast<FLOAT>(tex_dy + tex_dh) / tex2d_desc->Height;

	device_context->Unmap(buffer, 0);

	//頂点バッファーのバインド
	device_context->IASetVertexBuffers(0, 1, &buffer, &strides, &offsets);

	//プリミティブタイプおよびデータの順序に関する情報のバインド
	//{	D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED,
	//	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	//	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	//	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	//	D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
	//	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
	//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
	//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
	//	}

	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//入力レイアウトオブジェクトのバインド
	device_context->IASetInputLayout(input_layout);

	//ラスタライザーステートの設定
	device_context->RSSetState(rasterizer_state);

	//深度ステンシルステートの設定
	device_context->OMSetDepthStencilState(depth_stencil, 0);

	//シェーダーの設定
	device_context->VSSetShader(vertex_shader, NULL, 0);
	device_context->PSSetShader(pixel_shader, NULL, 0);

	device_context->PSSetShaderResources(0, 1, &shader_resource_view);
	device_context->PSSetSamplers(0, 1, &sampler_state);

	//プリミティブの描画

	device_context->Draw(4, 0);
}

void Sprite::Release()
{
	if (buffer) {
		buffer->Release();
	}
	if (rasterizer_state) {
		rasterizer_state->Release();
	}
	if (sampler_state) {
		sampler_state->Release();
	}
	if (depth_stencil) {
		depth_stencil->Release();
	}

	ResourceManager::ReleaseVertexShaders(vertex_shader, input_layout);
	ResourceManager::ReleasePixelShaders(pixel_shader);
	ResourceManager::ReleaseShaderResourceView(shader_resource_view);
}