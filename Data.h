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
		//	解放済みを確認　終了します
		if (count <= 0)	return;
		//	残機を削減後、削除命令の必要性を検討します
		if (--count <= 0)	bForce = true;

		//	削除命令をを確認、Releaseします
		if (bForce)
		{
			shader_resource_view->Release();
			ZeroMemory(this, sizeof(ShaderResourceViewData));	//	念の為に完全消去します
		}
	}
};
