#include	"..\Header\blender.h"

#include	<assert.h>


ID3D11BlendState*		blender::blend_state[MODE_MAX] = {0};
bool					blender::bLoad = false;
blender::BLEND_MODE		blender::nowMode;
ID3D11DeviceContext*	blender::keepContext;


struct _blend_data
{
	D3D11_BLEND		SrcBlend;
	D3D11_BLEND		DestBlend;
	D3D11_BLEND_OP	BlendOp;
	D3D11_BLEND		SrcBlendAlpha;
	D3D11_BLEND		DestBlendAlpha;
	D3D11_BLEND_OP	BlendOpAlpha;

}
blend_data[blender::MODE_MAX] =
{
	{	//	NONE
		D3D11_BLEND_ONE,				//	SrcBlend
		D3D11_BLEND_ZERO,				//	DestBlend
		D3D11_BLEND_OP_ADD,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ZERO,				//	DestBlendAlpha
		D3D11_BLEND_OP_ADD				//	BlendOpAlpha
	},	//	NONE
	{	//	ALPHA
		D3D11_BLEND_SRC_ALPHA,			//	SrcBlend
		D3D11_BLEND_INV_SRC_ALPHA,		//	DestBlend
		D3D11_BLEND_OP_ADD,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ZERO,				//	DestBlendAlpha
		D3D11_BLEND_OP_ADD				//	BlendOpAlpha
	},	//	ALPHA
	{	//	ADD
		D3D11_BLEND_SRC_ALPHA,			//	SrcBlend
		D3D11_BLEND_ONE,				//	DestBlend
		D3D11_BLEND_OP_ADD,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ZERO,				//	DestBlendAlpha
		D3D11_BLEND_OP_ADD,				//	BlendOpAlpha
	},	//	ADD
	{	//	SUB
		D3D11_BLEND_SRC_ALPHA,			//	SrcBlend
		D3D11_BLEND_ONE,				//	DestBlend
		D3D11_BLEND_OP_REV_SUBTRACT,	//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ZERO,				//	DestBlendAlpha
		D3D11_BLEND_OP_ADD,				//	BlendOpAlpha
	},	//	SUB
	{	//	REPLACE
		D3D11_BLEND_SRC_ALPHA,			//	SrcBlend
		D3D11_BLEND_ZERO,				//	DestBlend
		D3D11_BLEND_OP_ADD,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ZERO,				//	DestBlendAlpha
		D3D11_BLEND_OP_ADD				//	BlendOpAlpha
	},	//	REPLACE
	{	//	MULTIPLY
		D3D11_BLEND_ZERO,				//	SrcBlend
		D3D11_BLEND_SRC_COLOR,			//	DestBlend
		D3D11_BLEND_OP_ADD,				//	BlendOp
		D3D11_BLEND_DEST_ALPHA,			//	SrcBlendAlpha
		D3D11_BLEND_ZERO,				//	DestBlendAlpha
		D3D11_BLEND_OP_ADD				//	BlendOpAlpha
	},	//	MULTIPLY
	{	//	LIGHTEN
		D3D11_BLEND_ONE,				//	SrcBlend
		D3D11_BLEND_ONE,				//	DestBlend
		D3D11_BLEND_OP_MAX,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ONE,				//	DestBlendAlpha
		D3D11_BLEND_OP_MAX				//	BlendOpAlpha
	},	//	LIGHTEN
	{	//	DARKEN
		D3D11_BLEND_ONE,				//	SrcBlend
		D3D11_BLEND_ONE,				//	DestBlend
		D3D11_BLEND_OP_MIN,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_ONE,				//	DestBlendAlpha
		D3D11_BLEND_OP_MIN				//	BlendOpAlpha
	},	//	DARKEN
	{	//	SCREEN
		D3D11_BLEND_SRC_ALPHA,			//	SrcBlend
		D3D11_BLEND_INV_SRC_COLOR,		//	DestBlend
		D3D11_BLEND_OP_ADD,				//	BlendOp
		D3D11_BLEND_ONE,				//	SrcBlendAlpha
		D3D11_BLEND_INV_SRC_ALPHA,		//	DestBlendAlpha
		D3D11_BLEND_OP_ADD				//	BlendOpAlpha
	}	//	SCREEN
};

void blender::Init(ID3D11Device* device, ID3D11DeviceContext* context)
{
	if (bLoad)	return;			//	�ǂݍ��ݍς݂Ȃ牽�x���������Ȃ�

	HRESULT	hr;
	for (int i = 0; i < MODE_MAX; i++)
	{
		_blend_data& _bd = blend_data[i];

		D3D11_BLEND_DESC bDesc;
		ZeroMemory(&bDesc, sizeof(bDesc));
		bDesc.AlphaToCoverageEnable = FALSE;
		bDesc.IndependentBlendEnable = FALSE;

		D3D11_RENDER_TARGET_BLEND_DESC &rtbd = bDesc.RenderTarget[0];
		rtbd.BlendEnable			= (i != NONE ? TRUE : FALSE);
		rtbd.SrcBlend				= _bd.SrcBlend;
		rtbd.DestBlend				= _bd.DestBlend;
		rtbd.BlendOp				= _bd.BlendOp;
		rtbd.SrcBlendAlpha			= _bd.SrcBlendAlpha;
		rtbd.DestBlendAlpha			= _bd.DestBlendAlpha;
		rtbd.BlendOpAlpha			= _bd.BlendOpAlpha;
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&bDesc, &blend_state[i]);
		if (FAILED(hr))		assert(0 && "�u�����h�쐬���s");
	}

	keepContext = context;
	nowMode = MODE_MAX;		//	�ُ�l(���ݒ���)

	bLoad = true;
}

void blender::Set(BLEND_MODE mode, ID3D11DeviceContext* context)
{
	//	�G���[�`�F�b�N
	if (!bLoad)							return;
	if (mode < 0 || mode >= MODE_MAX)	return;
	if (mode == nowMode)				return;

	//	��2�����ȗ����A���O�ɐݒ肵�Ă���context���g�p
	if (!context)	context = keepContext;

	//	�u�����h�̐ݒ�
	context->OMSetBlendState(blend_state[mode], nullptr, 0xFFFFFFFF);
	nowMode = mode;
}

void blender::Release()
{
	for (auto &p : blend_state)
	{
		if (p)		p->Release();
		p = nullptr;
	}

	keepContext = nullptr;
	nowMode = MODE_MAX;				//	�ُ�l(���ݒ���)
	bLoad = false;
}