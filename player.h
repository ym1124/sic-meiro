#pragma once

#include	<DirectXMath.h>

#include	"..\Header\Mesh.h"

#define HP_MAX 300

/*******************************************************************************
	「プレイヤー」クラス
*******************************************************************************/
class Player
{
private:
	//hp
	float electric;

	float elect_power;
	float shadow_timer;

	bool brock_move_flg;
	int p;

	int move_se = -1;
	int electric_se;
	int catch_se;
	int rele_se;
	int no_move_se;

public:
	float GetElectric() { return electric; }
	void SetElectric(float _electric) { electric = _electric; }

	MyMesh				obj;				//	「モデルオブジェクト」
	MyMesh				head;				//	「モデルオブジェクト」
	MyMesh				tail;				//	「モデルオブジェクト」

	MyMesh				shadow[4];				//	「モデルオブジェクト」

	DirectX::XMFLOAT3	pos;				//	「位置(座標)」
	float				angle;				//	「回転角度」

	int e_timer;
	bool elec_heal_flg;
	bool brock_elec_flg;
	bool goal_flg;

	void	Initialize( const char* );																							//	初期化関数
	void	Release();																											//	解放関数
	void	Move();																												//	移動関数
	void	Render( const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection, const DirectX::XMFLOAT4& light_dir, int renderType=0);	//	描画関数
};


extern	Player player;
