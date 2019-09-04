#pragma once

#include <d3d11.h>

struct ShaderResourceViewData
{
	wchar_t						filename[256];
	ID3D11ShaderResourceView*	shader_resource_view;
	D3D11_TEXTURE2D_DESC		texDesc;
	int							count;

	ShaderResourceViewData() : shader_resource_view(nullptr), count(0)
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
			shader_resource_view->Release();
			ZeroMemory(this, sizeof(ShaderResourceViewData));	//	�O�ׂ̈Ɋ��S�������܂�
		}
	}
};
