#include "..\Header\debug.h"

void SetFontExtra(int extra)
{
	debug::font_extra = extra;
}

void InitFontExtra()
{
	debug::font_extra = 0;
}

void SetFontSize(int size)
{
	debug::font_size = size;
}

void InitFontSize()
{
	debug::font_size = 32;
}

HFONT SetString(const HDC& hdc, int angle, DirectX::XMINT3 color)
{
	HFONT font;

	// �t�H���g���쐬���ݒ肷��
	font = CreateFont(
		debug::font_size,		// �t�H���g�̍���(�傫��)�B
		0,						// �t�H���g�̕��B���ʂO�B
		angle,					// �p�x�B�O�łn�j�B
		0,						// �������p�x�B������O�B
		FW_DONTCARE,			// �����̑����B
		FALSE,					// �t�H���g���C�^���b�N�Ȃ�TRUE���w��B
		FALSE,					// �����������Ȃ�TRUE�B
		FALSE,					// ���������������Ȃ�TRUE�B
		SHIFTJIS_CHARSET,		// �t�H���g�̕����Z�b�g�B���̂܂܂łn�j�B
		OUT_DEFAULT_PRECIS,		// �o�͐��x�̐ݒ�B���̂܂܂łn�j�B
		CLIP_DEFAULT_PRECIS,	// �N���b�s���O���x�B���̂܂܂łn�j�B
		DRAFT_QUALITY,			// �t�H���g�̏o�͕i���B���̂܂܂łn�j�B
		DEFAULT_PITCH,			// �t�H���g�̃s�b�`�ƃt�@�~�����w��B���̂܂܂łn�j�B
		_T("HGP�n�p�p�߯�ߑ�")	// �t�H���g�̃^�C�v�t�F�C�X���̎w��B����͌����܂�܁B
	);

	//������̊Ԃ̊Ԋu�ݒ�
	SetTextCharacterExtra(hdc, debug::font_extra);

	//������̐F�̐ݒ�
	SetTextColor(hdc, RGB(color.x, color.y, color.z));

	//������̔w�i�̐ݒ�
	SetBkMode(hdc, TRANSPARENT);

	return font;
}
