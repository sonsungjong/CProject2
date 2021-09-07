#include "pch.h"
#include "framework.h"
#include "Resource.h"

// 버튼 핸들 저장용
//HWND gh_btn_wnd;
// 폰트 핸들 저장용
HFONT gh_font;

LRESULT CALLBACK SSJProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) {
		// WM_CLOSE 처리 후에 들어오는 메시지
		KillTimer(hWnd, 1);		// 1번 타이머를 죽이기
		DeleteObject(gh_font);		// 윈도우 파괴될때 폰트 사라지게끔
		PostQuitMessage(0);		// 메시지 큐애 WM_QUIT를 put
		//DestroyWindow(gh_btn_wnd);
	}
	else if (uMsg == WM_PAINT) {
		PAINTSTRUCT ps;		// 데이터 백업을 위한 구조체
		HDC h_dc = BeginPaint(hWnd, &ps);
		SelectObject(h_dc, GetStockObject(DC_BRUSH));
		SelectObject(h_dc, GetStockObject(DC_PEN));
		SetDCBrushColor(h_dc, RGB(230, 177, 91));
		SetDCPenColor(h_dc, RGB(0,0,0));

		EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		// 마우스 좌측버튼 추적
		unsigned int x = lParam & 0x0000FFFF;
		unsigned int y = (lParam >> 16) & 0x0000FFFF;
	}
	else if (uMsg == WM_CREATE) {
		// 버튼 생성
		//HWND hWNd = CreateWindowW(L"Button", L"버튼1", WS_CHILD | WS_VISIBLE, 10, 10, 150, 30, hWnd, (HMENU)1600, NULL, NULL);
		// hWnd 창에 1번 타이머를 설정, 0.1초마다 동작

		// 폰트 셋팅
		gh_font = CreateFont(48, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"굴림체");

		// 타이머 설정
		SetTimer(hWnd, 1, 1000, NULL);		// 콜백함수를 NULL로 하면 WM_TIMER
	}
	else if (uMsg == WM_TIMER) {
		if (wParam == 1) {
			HDC h_dc = GetDC(hWnd);
			SYSTEMTIME cur_time;		// 시스템시간 구조체
			GetLocalTime(&cur_time);		// 윈도우 설정시간
			wchar_t str[32];
			int len = swprintf_s(str, 32, L"%04d-%02d-%02d %02d:%02d:%02d",
				cur_time.wYear, cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond);
			SelectObject(h_dc, gh_font);
			SetTextColor(h_dc, RGB(255, 255, 0));		// 글자색
			SetBkColor(h_dc, RGB(0, 0, 0));		// 글자배경색
			TextOut(h_dc, 10, 10, str, len);
			ReleaseDC(hWnd, h_dc);
		}
	}
	else if (uMsg == WM_COMMAND) {
		if (wParam == 1600) {
			//MessageBox(hWnd, L"버튼1이 눌러짐", L"버튼1 확인", MB_OK);
		}
	}

	// 개발자가 처리하지 않은 메시지들을 처리
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 윈도우 클래스 등록
	WNDCLASS wc;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		// 배경색
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// 커서 지정
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);			// 로고 아이콘
	wc.hInstance = hInstance;
	wc.lpfnWndProc = SSJProc;			// 기본 메시지 처리기 함수
	wc.lpszClassName = L"name";		// 윈도우 클래스 이름
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);		// 윈도우 클래스 등록

	// 윈도우 생성
	HWND hWnd = CreateWindowW(L"name", L"www.address.com", WS_OVERLAPPEDWINDOW, 100, 90, 600, 600, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);		// 어떻게 출력할 것인가 지정
	UpdateWindow(hWnd);		// WM_PAINT 메시지가 대기상태라면 즉시 수행하게함

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}