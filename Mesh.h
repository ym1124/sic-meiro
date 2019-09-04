#pragma once

#include <directxmath.h>

#include "..\Header\SkinMesh.h"
#include "..\Header\Geometric_primitive.h"
#include "..\Header\Light.h"

class MyMesh
{
private:
	bool				bLoad;

public:
	SkinMesh*	obj;

	DirectX::XMFLOAT3	pos;				//	「位置」
	DirectX::XMFLOAT3	angle;				//	「(回転)角度」
	DirectX::XMFLOAT3	scale;				//	「大きさ」

	DirectX::XMFLOAT4	color;

	float	anim_timer;

	void	Initialize();
	bool	Load(const char* fbx_filename);
	//void	SetPrimitive(GeometricPrimitive* primitive);
	void	SetMesh(MyMesh& org);
	void	Release();
	void	Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, float timer = 0.0f);
	void	Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, int renderType = 0, const DirectX::XMFLOAT3& pos = { 0.0f,0.0f,0.0f }, const int light_num = 1, const Light &light = {}, float timer = 0.0f);

	DirectX::XMMATRIX	GetWorldMatrix();

};

class GeometricMesh
{
private:
	bool				bLoad;

public:
	GeometricPrimitive*	obj;

	DirectX::XMFLOAT3	pos;				//	「位置」
	DirectX::XMFLOAT3	angle;				//	「(回転)角度」
	DirectX::XMFLOAT3	scale;				//	「大きさ」

	DirectX::XMFLOAT4	color;

	void	Initialize();
	//bool	Load(const char* fbx_filename);
	void	SetPrimitive(GeometricPrimitive* primitive);
	void	SetMesh(GeometricMesh& org);
	void	Release();
	void	Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, float timer = 0.0f);

	DirectX::XMMATRIX	GetWorldMatrix();
};


