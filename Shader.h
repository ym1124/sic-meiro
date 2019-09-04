#pragma once

#include <d3d11.h>

struct VertexShadersData
{
	char						filename[256];
	ID3D11VertexShader*			vertex_shader;
	ID3D11InputLayout*			input_layout;
	int							count;

	VertexShadersData() : vertex_shader(nullptr), input_layout(nullptr), count(0)
	{
		ZeroMemory(filename, sizeof(filename));
	}

	void Release(bool bForce = false)
	{
		//	解放済みを確認　終了します
		if (count <= 0)	return;
		//	残機を削減後、削除命令の必要性を検討します
		if (--count <= 0)	bForce = true;

		//	削除命令をを確認、Releaseします
		if (bForce)
		{
			vertex_shader->Release();
			input_layout->Release();
			ZeroMemory(this, sizeof(VertexShadersData));	//	念の為に完全消去します
		}
	}
};

struct PixelShadersData
{
	char						filename[256];
	ID3D11PixelShader*			pixel_shader;
	int							count;

	PixelShadersData() : pixel_shader(nullptr), count(0)
	{
		ZeroMemory(filename, sizeof(filename));
	}

	void Release(bool bForce = false)
	{
		//	解放済みを確認　終了します
		if (count <= 0)	return;
		//	残機を削減後、削除命令の必要性を検討します
		if (--count <= 0)	bForce = true;

		//	削除命令をを確認、Releaseします
		if (bForce)
		{
			pixel_shader->Release();
			ZeroMemory(this, sizeof(PixelShadersData));	//	念の為に完全消去します
		}
	}
};