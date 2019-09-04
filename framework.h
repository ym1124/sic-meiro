#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <d3d11.h>
#include <memory>

#include "misc.h"
#include "high_resolution_timer.h"

#include "Sprite.h"
#include "Geometric_primitive.h"
#include "StaticMesh.h"

#include "SkinMesh.h"

#include "blender.h"
#include "Input.h"


const LONG SCREEN_WIDTH = 1280;
const LONG SCREEN_HEIGHT = 720;
const LONG FRAME_RATE = 60;			//	これを変更しても実際のフレームレートは変わらない　どこで変わっているかは自分で調査しなさい

class framework
{
public:
	CONST HWND hwnd;
	static CONST LONG SCREEN_WIDTH = 1280;
	static CONST LONG SCREEN_HEIGHT = 720;

	D3D_DRIVER_TYPE         g_driverType;
	D3D_FEATURE_LEVEL       g_featureLevel;
	ID3D11Device			*Device;
	ID3D11DeviceContext		*DeviceContext;
	IDXGISwapChain			*SwapChain;
	ID3D11RenderTargetView	*RenderTargetView;
	ID3D11DepthStencilView	*DepthStencilView;

	////////////////////////////////////////////////////
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMFLOAT4 light_direction;

	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 _angle;
	DirectX::XMFLOAT3 pos;

	void Move();
	inline static ID3D11Device*			getDevice()	{ return framework::getInstance().Device; };
	inline static ID3D11DeviceContext*	getContext(){ return framework::getInstance().DeviceContext; };
	inline static HWND					getHWND()	{ return framework::getInstance().hwnd; };

	float GetTimeIntarval()
	{
		return timer.time_interval();
	}

	static framework& getInstance(HWND hwnd = nullptr)
	{
		static framework instance(hwnd);
		return instance;
	}

	/////////////////////////////////////////////////////

	//std::unique_ptr<Sprite> sprite[1024];
	//std::unique_ptr<Sprite> particle;

	//std::unique_ptr<GeometricPrimitive> cube;

	//std::unique_ptr<StaticMesh> static_mesh;

	//std::unique_ptr<SkinMesh> skin_mesh;

	framework(HWND hwnd) : hwnd(hwnd)
	{

	}
	~framework()
	{
		Release();
	}
	int run()
	{
		MSG msg = {};

		if (!initialize()) return 0;
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				timer.tick();
				calculate_frame_stats();

				std::ostringstream outs;
				outs.precision(6);
				outs << "自作lib,ゲーム";
				SetWindowTextA(hwnd, outs.str().c_str());

				update(timer.time_interval());
				render(timer.time_interval());
			}
		}
		return static_cast<int>(msg.wParam);
	}

	LRESULT CALLBACK handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		Mouse::GetState(msg);

		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CREATE:
			break;
		case WM_KEYDOWN:
			//ESCAPEキーが押されたら強制終了
			if (wparam == VK_ESCAPE) PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case WM_ENTERSIZEMOVE:
			// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
			timer.stop();
			break;
		case WM_EXITSIZEMOVE:
			// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
			// Here we reset everything based on the new window dimensions.
			timer.start();
			break;

		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

private:
	bool initialize();
	void update(float elapsed_time/*Elapsed seconds from last frame*/);
	void render(float elapsed_time/*Elapsed seconds from last frame*/);
	void Release();

private:
	high_resolution_timer timer;
	void calculate_frame_stats()
	{
		// Code computes the average frames per second, and also the 
		// average time it takes to render one frame.  These stats 
		// are appended to the window caption bar.
		static int frames = 0;
		static float time_tlapsed = 0.0f;

		frames++;

		// Compute averages over one second period.
		if ((timer.time_stamp() - time_tlapsed) >= 1.0f)
		{
			float fps = static_cast<float>(frames); // fps = frameCnt / 1
			float mspf = 1000.0f / fps;
			std::ostringstream outs;
			outs.precision(6);
			outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
			SetWindowTextA(hwnd, outs.str().c_str());

			// Reset for next average.
			frames = 0;
			time_tlapsed += 1.0f;
		}
	}
};

#define	FRAMEWORK	framework::getInstance()
