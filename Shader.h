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
		//	����ς݂��m�F�@�I�����܂�
		if (count <= 0)	return;
		//	�c�@���팸��A�폜���߂̕K�v�����������܂�
		if (--count <= 0)	bForce = true;

		//	�폜���߂����m�F�ARelease���܂�
		if (bForce)
		{
			vertex_shader->Release();
			input_layout->Release();
			ZeroMemory(this, sizeof(VertexShadersData));	//	�O�ׂ̈Ɋ��S�������܂�
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
		//	����ς݂��m�F�@�I�����܂�
		if (count <= 0)	return;
		//	�c�@���팸��A�폜���߂̕K�v�����������܂�
		if (--count <= 0)	bForce = true;

		//	�폜���߂����m�F�ARelease���܂�
		if (bForce)
		{
			pixel_shader->Release();
			ZeroMemory(this, sizeof(PixelShadersData));	//	�O�ׂ̈Ɋ��S�������܂�
		}
	}
};