#pragma once

#include "..\Header\framework.h"
#include <Windows.h>
#include <vector>
#include <cstdio>

namespace debug
{
	static int font_size = 32;
	static int font_extra = 0;
}

void SetFontExtra(int extra);
void InitFontExtra();

void SetFontSize(int size);
void InitFontSize();

HFONT SetString(const HDC& hdc, int angle, DirectX::XMINT3 color);

template <typename ... Args>
std::wstring format(const std::wstring& fmt, Args ... args)
{
	size_t len = swprintf(nullptr, 0, fmt.c_str(), args ...);
	std::vector<wchar_t> buf(len + 1);
	swprintf(&buf[0], len + 1, fmt.c_str(), args ...);
	return std::wstring(&buf[0], &buf[0] + len);
}

template <typename ... Args>
void DrawString(int x, int y, DirectX::XMINT3 color, const std::wstring& string, Args ... args)
{
	HDC hdc;
	HGDIOBJ font;

	hdc = GetDC(FRAMEWORK.getHWND());

	font = SelectObject(hdc, SetString(hdc, 0, color));

	std::wstring test;
	test = format(string, args ...);

	//•`‰æ
	TextOut(hdc, x, y, test.c_str(), test.size());

	DeleteObject(font);

	ReleaseDC(FRAMEWORK.getHWND(), hdc);
}

template <typename ... Args>
void DrawRotateString(int x, int y, float angle, DirectX::XMINT3 color, const std::wstring& string, Args ... args)
{
	HDC hdc;
	HGDIOBJ font;

	hdc = GetDC(FRAMEWORK.getHWND());

	font = SelectObject(hdc, SetString(hdc, static_cast<int>(DirectX::XMConvertToDegrees(angle) * 10), color));

	std::wstring test;
	test = format(string, args ...);

	//•`‰æ
	TextOut(hdc, x, y, test.c_str(), test.size());

	DeleteObject(font);

	ReleaseDC(FRAMEWORK.getHWND(), hdc);
}
