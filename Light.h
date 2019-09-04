#pragma once

#include <DirectXMath.h>

class Light
{
public:
	Light(const DirectX::XMFLOAT3& _pos = { 0.0f,0.0f,0.0f },
		const DirectX::XMFLOAT4& _color = { 0.01f,0.0f,0.0f,1.0f },
		const DirectX::XMFLOAT3& _attenution = { 0.0f,0.0f,0.0f }
		) :pos(_pos), attenution(_attenution), color(_color)
	{}

	~Light() {}

	static void setLanthanum(DirectX::XMFLOAT3 *arrey, Light *lanthanum, int loop)
	{
		for (int i = 0; i < loop; i++)
		{
			lanthanum[i].pos = arrey[i];
		}
	}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 attenution;
	DirectX::XMFLOAT4 color;
};

extern Light light;
extern Light lanthanum[6];
