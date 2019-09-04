#pragma once

#include	<d3d11.h>

class blender
{
public:
	typedef enum {
		NONE = 0,	//	��������
		ALPHA,		//	������
		ADD,		//	���Z����
		SUBTRACT,	//	���Z����
		REPLACE,	//	�u������
		MULTIPLY,	//	��Z
		LIGHTEN,	//	��r(��)
		DARKEN,		//	��r(��)
		SCREEN,		//	�X�N���[��

		MODE_MAX
	}BLEND_MODE;

private:
	static ID3D11BlendState*		blend_state[MODE_MAX];
	static bool						bLoad;
	static BLEND_MODE				nowMode;
	static ID3D11DeviceContext*		keepContext;

	blender(){};			//	���̊m�ۋ֎~

public:
	static void	Init(ID3D11Device* device, ID3D11DeviceContext* context);
	static void Set(BLEND_MODE mode, ID3D11DeviceContext* context = nullptr);
	static void Release();
};
