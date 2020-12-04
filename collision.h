#pragma once

#include	<DirectXMath.h>


/*******************************************************************************
	衝突判定用クラス
*******************************************************************************/
class Collision
{
public:
	static bool	HitSphere(const DirectX::XMFLOAT3& p1, float r1, const DirectX::XMFLOAT3& p2, float r2 );
};
