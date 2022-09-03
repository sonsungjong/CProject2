#include "framework.h"
#include "D2DImgRotate.h"
#include <d2d1.h>
#include <wincodec.h>
#pragma comment(lib, "D2D1.lib")

using namespace D2D1;

ID2D1Factory* gp_factory;
ID2D1HwndRenderTarget* gp_target;
ID2D1Bitmap* gp_bitmap;
D2D1_RECT_F g_image_rect;
D2D1_POINT_2F g_center_pos;
FLOAT g_degree = 0.0f;

void ResizeTarget(IWICBitmapFrameDecode* ap_image_frame)
{
	UINT width, height;
	ap_image_frame->GetSize(&width, &height);
	if (g_image_rect.right != width || g_image_rect.bottom != height) {
		gp_target->Resize(D2D1::SizeU(width, height));
		g_image_rect.right = (FLOAT)width;
		g_image_rect.bottom = (FLOAT)height;
		g_center_pos.x = g_image_rect.right / 2;
		g_center_pos.y = g_image_rect.bottom / 2;
	}
}

INT MakeD2D1_Bitmap(IWICImagingFactory* ap_wic_factory, IWICBitmapFrameDecode* ap_image_frame)
{
	IWICFormatConverter* p_converter;
	INT result = 0;
	if (S_OK == ap_wic_factory->CreateFormatConverter(&p_converter)) {
		if (S_OK == p_converter->Initialize(ap_image_frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom)) {
			if (S_OK == gp_target->CreateBitmapFromWicBitmap(p_converter, NULL, &gp_bitmap)) {
				result = 1;			// 성공적으로 생성
			}
		}
		p_converter->Release();
	}
	return result;
}

INT LoadMyImage(const TCHAR* ap_path)
{
	if (gp_bitmap != NULL) {
		gp_bitmap->Release();
		gp_bitmap = NULL;
	}
	IWICImagingFactory* p_wic_factory;
	(void)CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&p_wic_factory));
	IWICBitmapDecoder* p_decoder;
	IWICBitmapFrameDecode* p_frame;
	INT result = 0;
	if (S_OK == p_wic_factory->CreateDecoderFromFilename(ap_path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &p_decoder)) {
		if (S_OK == p_decoder->GetFrame(0, &p_frame)) {
			ResizeTarget(p_frame);
			result = MakeD2D1_Bitmap(p_wic_factory, p_frame);
			p_frame->Release();
		}
		p_decoder->Release();
	}
	p_wic_factory->Release();
	return result;
}

void OnPaint()
{
	gp_target->BeginDraw();
	gp_target->Clear(ColorF(0.0f, 0.8f, 1.0f));
	if (gp_bitmap != NULL) {
		gp_target->SetTransform(Matrix3x2F::Rotation(g_degree, g_center_pos));
		gp_target->DrawBitmap(gp_bitmap, &g_image_rect);
	}
	gp_target->EndDraw();
}

void OnCreate(HWND ah_wnd)
{
	RECT client_rect;
	RECT win_rect;
	GetClientRect(ah_wnd, &client_rect);

	gp_factory->CreateHwndRenderTarget(RenderTargetProperties(), HwndRenderTargetProperties(ah_wnd, SizeU(client_rect.right, client_rect.bottom)), &gp_target);

	if (1 == LoadMyImage(_T("../img/Lenna.png"))) {
		GetWindowRect(ah_wnd, &win_rect);
		SetWindowPos(ah_wnd, NULL, 0, 0, win_rect.right - win_rect.left + (INT)(g_image_rect.right) - client_rect.right, win_rect.bottom - win_rect.top + (INT)(g_image_rect.bottom) - client_rect.bottom, SWP_NOMOVE);
		SetTimer(ah_wnd, 1, 50, NULL);
	}
}

void OnTimer(HWND ah_wnd, UINT a_timer_id)
{
	if (a_timer_id == 1) {
		g_degree = g_degree + 0.5f;
		if (g_degree >= 360) { g_degree = 0.0f; }
		InvalidateRect(ah_wnd, NULL, FALSE);
	}
}

void OnDestroy(HWND ah_wnd)
{
	KillTimer(ah_wnd, 1);
	if (gp_target != NULL) {
		gp_target->Release();
		gp_target = NULL;
	}
	PostQuitMessage(0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_PAINT) {
		ValidateRect(hWnd, NULL);
		OnPaint();
		return 0;
	}
	else if (msg == WM_CREATE) {
		OnCreate(hWnd);
		return 0;
	}
	else if (msg == WM_TIMER) {
		OnTimer(hWnd, static_cast<UINT>(wParam));
		return 0;
	}
	else if (msg == WM_DESTROY) {
		OnDestroy(hWnd);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

ImgRotate::ImgRotate(HINSTANCE ah_instance, const TCHAR* ap_class_name)
{
	(void)CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gp_factory);
	mh_instance = ah_instance;
	_tcscpy_s(m_class_name, 32, ap_class_name);
	mh_wnd = nullptr;
	m_exit_state = 0;
}

ImgRotate::~ImgRotate()
{
	if (gp_bitmap != NULL) {
		gp_bitmap->Release();
	}
	gp_factory->Release();
	CoUninitialize();
}

INT ImgRotate::GetExitState()
{
	return m_exit_state;
}

void ImgRotate::InitApplication()
{
	WNDCLASS wc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = mh_instance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = m_class_name;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
}

int ImgRotate::InitInstance()
{
	mh_wnd = CreateWindow(m_class_name, _T("D2D를 사용한 이미지회전"), WS_OVERLAPPEDWINDOW, 50, 50, 300, 200, NULL, NULL, mh_instance, NULL);
	ShowWindow(mh_wnd, SW_SHOW);
	UpdateWindow(mh_wnd);

	return 1;
}

void ImgRotate::Run()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	m_exit_state = static_cast<INT>(msg.wParam);
}

INT WINAPI _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ INT nCmdShow)
{
	ImgRotate win_class(hInstance, _T("tipssoft"));

	win_class.InitApplication();
	win_class.InitInstance();
	win_class.Run();

	return win_class.GetExitState();
}