#pragma once

#include <directxmath.h>
#include "..\Header\Mesh.h"
#include "..\Header\Light.h"
#include	<DirectXMath.h>

#define FLOAR_DAMAGE 0.1f
#define ELECTRIC_DAMAGE 0.1f
#define LIGHT_HEAL 1.0f

class Trap
{
public:

	bool lift_flg;
	bool gate_flg;
	void Init();
	void Update();
	void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, const DirectX::XMFLOAT3& pos, const int light_num, const Light &light);
	void Release();
};


class Lift
{
public:

	MyMesh obj;

	float timer;
	DirectX::XMFLOAT3 pos;
	float angle;
	float max_pos;

	void Init();
	void Update();
	void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, const DirectX::XMFLOAT3& pos, const int light_num, const Light &light);
	void Release();
};

class Gate
{
public:

	bool switch_flg;

	bool power_charge_flg;
	int elec_power;//電力

	float pos_check_x;
	float pos_check_z;

	MyMesh obj[3];
	bool start_up_flg;
	float timer;
	DirectX::XMFLOAT3 pos[3];
	float angle[3];
	float max_pos;
	float min_pos;
	float start_pos[3];

	void Init();
	void Update();
	void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, const DirectX::XMFLOAT3& pos, const int light_num, const Light &light);
	void Release();
};

class PointLight 
{
public:
	int battery;//バッテリー


};