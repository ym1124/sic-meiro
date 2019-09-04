#pragma once

//#include <afxtempl.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>

class Sprite
{
private:
	void Release();

public:
	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;

		DirectX::XMFLOAT2 texcoord;
	};

	ID3D11VertexShader		*vertex_shader;
	ID3D11PixelShader		*pixel_shader;
	ID3D11InputLayout		*input_layout;
	ID3D11Buffer			*buffer;
	ID3D11RasterizerState	*rasterizer_state;

	ID3D11ShaderResourceView *shader_resource_view;
	D3D11_TEXTURE2D_DESC	*tex2d_desc;
	ID3D11SamplerState		*sampler_state;

	ID3D11DepthStencilState	*depth_stencil;

	void Render(ID3D11DeviceContext*,
		float dx, float dy, float dw, float dh,
		float tex_dx, float tex_dy, float tex_dw, float tex_dh,
		float angle, float r, float g, float b, float a);

	Sprite(ID3D11Device*, const wchar_t* /*Texture file name*/);
	~Sprite()
	{
		Release();
	}

	float Get_TextureWidth()
	{
		return static_cast<float>(tex2d_desc->Width);
	}

	//�摜�̏c����Ԃ�
	float Get_TextureHeight()
	{
		return static_cast<float>(tex2d_desc->Height);
	}
};


//bool ReadShader(const char* csoName, CArray<BYTE>*byteArray);

//+ Sprite�^�̃��[�h
//? pSprite : unique_ptr<Sprite> �̃|�C���^�[
//? file_name : �ǂݍ��݂����摜�̃p�X
void Sprite_Load(std::unique_ptr<Sprite> *pSprite, const wchar_t *file_name);

//+ Sprite�^�̕`��
//? pSprite : unique_ptr<Sprite>
//? pos_x, pos_y : �摜�̍��W
//? scale_x, scale_y : �g��l
//? tex_x, tex_y : �摜�̐؂���J�n���W
//? tex_w, tex_h : �摜�̐؂��蕝
//? center_flg : true = ���S, false = �E��
//?
void Sprite_Render(std::unique_ptr<Sprite> &pSprite,
	const float &pos_x, const float &pos_y,
	const float &scale_x = 1.0f, const float &scale_y = 1.0f,
	const float &tex_x = 0.0f, const float &tex_y = 0.0f,
	const float &tex_w = 0.0f, const float &tex_h = 0.0f,
	const bool  &center_flg = true,
	const float &angle = 0.0f,
	const float &red = 1.0f, const float &green = 1.0f, const float &blue = 1.0f,
	const float &alpha = 1.0f);
