#pragma once

#include	<DirectXMath.h>


/*******************************************************************************
「カメラ」クラス
*******************************************************************************/
class Camera
{
public:
	/*******************************************************************************
	カメラ状態管理番号の宣言
	*******************************************************************************/
	enum MODE
	{
		WATCH,				//	「監視カメラ」
		DIRECTIONAL,		//	「相対位置固定カメラ」
		CHASE,				//	「追跡カメラ」
		TPS,				//	「3人称カメラ(TPS)」
		FPS,				//	「1人称カメラ(FPS)」
		UP,					//　「真上カメラ」
		MAX					//	「カメラ管理番号最大値」	
	};

private:
	/*******************************************************************************
	カメラ状態保存用変数の定義
	*******************************************************************************/
	int		mode;

public:
	DirectX::XMFLOAT3 position;				//	「位置(座標)」
	DirectX::XMFLOAT3 point_view;			//	「注視点」
	DirectX::XMMATRIX project_matrix;		//	「投影行列」

	Camera();
	/*******************************************************************************
	カメラ更新関数の宣言
	*******************************************************************************/
	void				Update();					//	更新関数

private:		//	追加メンバ関数(外部から呼び出す予定が無いのでprivate化)(汚い組み方だねぇ…)
	void				Watch();		//	監視カメラ関数
	void				Directional();	//	相対位置固定カメラ関数
	void				Chase();		//	追跡カメラ関数
	void				Tps();			//	3人称カメラ(TPS)関数
	void				Fps();			//	1人称カメラ(FPS)関数
	void				Up();
public:

	void Initialize();

	DirectX::XMMATRIX SetParallelProjection(float width, float height, float recently, float farthest);
	DirectX::XMMATRIX SetPerspectiveProjection(float view_angle, float aspect, float recently, float farthest);
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix() { return project_matrix; }

};