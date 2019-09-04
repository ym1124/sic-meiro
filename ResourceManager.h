#pragma once

#include "Shader.h"
#include "Data.h"

class ResourceManager
{
	static const int RESOURCE_MAX = 128;		//	最大スタック数

	static ShaderResourceViewData	srvData[RESOURCE_MAX];
	static VertexShadersData		vsData[RESOURCE_MAX];
	static PixelShadersData			psData[RESOURCE_MAX];

	ResourceManager() {}
	~ResourceManager() { Release(); }

public:
	static void Release()
	{
		for (int n = 0; n < RESOURCE_MAX; n++)
		{
			srvData[n].Release(true);
			vsData[n].Release(true);
			psData[n].Release(true);
		}
	}

	static bool LoadShaderResourceView(ID3D11Device* device, const wchar_t* filename,
		ID3D11ShaderResourceView** srv, D3D11_TEXTURE2D_DESC** texDesc);
	static void ReleaseShaderResourceView(ID3D11ShaderResourceView* srv);

	static bool LoadVertexShaders(ID3D11Device* device, const char* filename,
		D3D11_INPUT_ELEMENT_DESC *elementDescs, int numElement,
		ID3D11VertexShader** vs, ID3D11InputLayout** il);
	static void ReleaseVertexShaders(ID3D11VertexShader* vs, ID3D11InputLayout* il);


	static bool LoadPixelShaders(ID3D11Device* device, const char* filename, ID3D11PixelShader** ps);
	static void ReleasePixelShaders(ID3D11PixelShader* ps);
}; 
