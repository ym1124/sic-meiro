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

	// フォントを作成し設定する
	font = CreateFont(
		debug::font_size,		// フォントの高さ(大きさ)。
		0,						// フォントの幅。普通０。
		angle,					// 角度。０でＯＫ。
		0,						// 同じく角度。これも０。
		FW_DONTCARE,			// 文字の太さ。
		FALSE,					// フォントがイタリックならTRUEを指定。
		FALSE,					// 下線を引くならTRUE。
		FALSE,					// 取り消し線を引くならTRUE。
		SHIFTJIS_CHARSET,		// フォントの文字セット。このままでＯＫ。
		OUT_DEFAULT_PRECIS,		// 出力精度の設定。このままでＯＫ。
		CLIP_DEFAULT_PRECIS,	// クリッピング精度。このままでＯＫ。
		DRAFT_QUALITY,			// フォントの出力品質。このままでＯＫ。
		DEFAULT_PITCH,			// フォントのピッチとファミリを指定。このままでＯＫ。
		_T("HGP創英角ﾎﾟｯﾌﾟ体")	// フォントのタイプフェイス名の指定。これは見たまんま。
	);

	//文字列の間の間隔設定
	SetTextCharacterExtra(hdc, debug::font_extra);

	//文字列の色の設定
	SetTextColor(hdc, RGB(color.x, color.y, color.z));

	//文字列の背景の設定
	SetBkMode(hdc, TRANSPARENT);

	return font;
}
