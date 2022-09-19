// WinMainClass1.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "WinMainClass1.h"
#include "TW_WinApp.h"

#pragma comment(lib, "D2D1.lib")
#include <wincodec.h>

ID2D1HwndRenderTarget* gp_target;
ID2D1Bitmap* gp_bitmap;
D2D1_RECT_F g_image_rect;
D2D1_POINT_2F g_center_pos;
float g_degree = 0.0f;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_PAINT)
	{
		ValidateRect(hWnd, NULL);
		OnPaint();
		return 0;
	}
	else if (uMsg == WM_CREATE)
	{
		OnCreate(hWnd);
		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		OnTimer(hWnd, wParam);
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		OnDestroy(hWnd);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TW_WinApp myapp(hInstance, _T("myapp1"));

	myapp.InitApplication();
	myapp.InitInstance();
	myapp.Run();
	return myapp.GetExitState();
}