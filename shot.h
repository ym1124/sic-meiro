#pragma once

#include	"..\Header\Mesh.h"


/*******************************************************************************
	弾丸クラスの定義
*******************************************************************************/
class Shot
{
public:
	static constexpr float	MAX_DISTANCE = 30.0f;
	static const int		FADE_TIMER = 15;

	GeometricMesh		obj;
	DirectX::XMFLOAT3	pos;
	float				angle;
	DirectX::XMFLOAT3	speed;
	bool				exist;
	int					timer;
	float				alpha;

	void				Move();
};


/*******************************************************************************
	弾丸管理クラスの定義
*******************************************************************************/
class ShotManager
{
public:
	static const int	MAX = 3;

private:
	Shot				data[MAX];
	ShotManager(){ Initialize(); }		//	プライベートに宣言し、初期化関数を呼び出す

public:
	virtual ~ShotManager(){ Release(); }

	void	Initialize();
	void	Release();
	void	Update();
	void	Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj, const DirectX::XMFLOAT4& light_dir);
	Shot*	Set(DirectX::XMFLOAT3 pos, float angle, float speed, float scale = 1.0f , DirectX::XMFLOAT3 color = DirectX::XMFLOAT3(1,1,1));
	Shot*	Get(int no)
	{
		if ((unsigned int)no >= MAX)	return	nullptr;
		return	&data[no];
	}

	static ShotManager*	getInstance()
	{
		static ShotManager instance;
		return &instance;
	}
};

#define	pShotManager	ShotManager::getInstance()