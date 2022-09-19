#include "pch.h"
#include "TW_WinApp.h"

ID2D1Factory *gp_factory;

extern LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

TW_WinApp::TW_WinApp(HINSTANCE ah_instance, const TCHAR* ap_class_name)
{
	(void)CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gp_factory);

	mh_instance = ah_instance;
	_tcscpy_s(m_class_name, 32, ap_class_name);
	mh_wnd = NULL;
	m_exit_state = 0;
}

TW_WinApp::~TW_WinApp()
{
	gp_factory->Release();
	CoUninitialize();
}

void TW_WinApp::InitApplication()
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

int TW_WinApp::InitInstance()
{
	return 1;
}

void TW_WinApp::Run()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	m_exit_state = msg.wParam;
}