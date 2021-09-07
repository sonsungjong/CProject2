#include "pch.h"
#include "framework.h"
#include "Resource.h"
#include <time.h>

class CDC
{
protected:
	int x, y, r;
	HWND mh_wnd;
	HDC mh_dc;
	COLORREF color;
public:
	CDC(HWND ah_wnd)
	{
		x = 0;		// 0~799
		y = 0;		// 0~799
		r = 0;		// 0~49 +10
		color = RGB(0, 0, 0);		// RGB값 초기화
		mh_wnd = ah_wnd;
	}
	virtual ~CDC() {

	}
	HDC GetHandle()
	{
		return mh_dc;
	}
	operator HDC()
	{
		return mh_dc;
	}
	HGDIOBJ SelectObject(HGDIOBJ ah_obj)
	{
		return ::SelectObject(mh_dc, ah_obj);
	}
	HGDIOBJ SelectStockObject(int a_obj_index)
	{
		return ::SelectObject(mh_dc, GetStockObject(a_obj_index));
	}
	COLORREF SetDCBrushColor()
	{
		color = RGB(rand() % 256, rand() % 256, rand() % 256);		// RGB난수
		return ::SetDCBrushColor(mh_dc, color);
	}
	void Ellipse()
	{
		x = rand() % 800;		// 0~799
		y = rand() % 800;		// 0~799
		r = rand() % 50 + 10;		// 0~49 +10
		::Ellipse(mh_dc, x - r, y - r, x + r, y + r);
	}

	void Rectangle(int a_sx, int a_sy, int a_ex, int a_ey)
	{
		::Rectangle(mh_dc, a_sx, a_sy, a_ex, a_ey);
	}

	void FillSolidRect()
	{
		SelectStockObject(DC_BRUSH);
		SetDCBrushColor();
		x = rand() % 800;		// 0~799
		y = rand() % 800;		// 0~799
		r = rand() % 50 + 10;		// 0~49 +10
		::Rectangle(mh_dc, x - r, y - r, x + r, y + r);
	}
};

class CPaintDC : public CDC
{
protected:
	PAINTSTRUCT m_ps;
public:
	CPaintDC(HWND ah_wnd) : CDC(ah_wnd)
	{
		mh_dc = BeginPaint(mh_wnd, &m_ps);
	}
	virtual ~CPaintDC()
	{
		EndPaint(mh_wnd, &m_ps);
	}
};

class CClientDC : public CDC
{
protected:
	
public:
	CClientDC(HWND ah_wnd) : CDC(ah_wnd)
	{
		mh_dc = GetDC(mh_wnd);
	}
	virtual ~CClientDC()
	{
		ReleaseDC(mh_wnd, mh_dc);
	}
};

void Draw(CDC* ap_dc) {
	// 사각형 막그리기
	ap_dc->FillSolidRect();

	// 원 막그리기
	ap_dc->SelectStockObject(DC_BRUSH);
	ap_dc->SelectStockObject(DC_PEN);
	ap_dc->SetDCBrushColor();
	ap_dc->Ellipse();
}

// CWnd
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) {
		// WM_CLOSE 처리 후에 들어오는 메시지
		KillTimer(hWnd, 1);
		PostQuitMessage(0);		// 메시지 큐애 WM_QUIT를 put
	}
	else if (uMsg == WM_PAINT) {
		CPaintDC dc(hWnd);
		Draw(&dc);
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		// 마우스 좌측버튼 추적
		unsigned int x = lParam & 0x0000FFFF;
		unsigned int y = (lParam >> 16) & 0x0000FFFF;
	}
	else if (uMsg == WM_CREATE) {
		srand((unsigned int)time(NULL));		// 난수패턴 기준 초기화
		// 타이머 설정
		SetTimer(hWnd, 1, 100, NULL);		// 콜백함수를 NULL로 하면 100ms마다 WM_TIMER (ID : 1)
	}
	else if (uMsg == WM_TIMER) {
		if (wParam == 1) {
			/*
			HDC h_dc = GetDC(hWnd);

			int x = rand() % 800;		// 0~799
			int y = rand() % 800;		// 0~799
			int r = rand() % 50+10;		// 0~49 +10
			COLORREF color = RGB(rand()%256, rand()%256, rand()%256);		// RGB값 난수
			SelectObject(h_dc, GetStockObject(DC_BRUSH));
			SetDCBrushColor(h_dc, color);
			Ellipse(h_dc, x - r, y - r, x + r, y + r);

			ReleaseDC(hWnd, h_dc);
			

			CClientDC dc(hWnd);
			// 사각형 막그리기
			dc.FillSolidRect();

			// 원 막그리기
			dc.SelectStockObject(DC_BRUSH);
			dc.SetDCBrushColor();
			dc.Ellipse();
			*/

			CClientDC dc(hWnd);
			Draw(&dc);
		}
	}

	// 개발자가 처리하지 않은 메시지들을 처리
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

class CWinApp 
{
protected:
	HINSTANCE mh_inst;
public:
	CWinApp(HINSTANCE ah_inst)
	{
		mh_inst = ah_inst;
	}

	void InitApplication() {
		// 윈도우 클래스 등록
		WNDCLASS wc;

		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		// 배경색
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// 커서 지정
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);			// 로고 아이콘
		wc.hInstance = mh_inst;
		wc.lpfnWndProc = WndProc;			// 기본 메시지 처리기 함수
		wc.lpszClassName = L"name";		// 윈도우 클래스 이름
		wc.lpszMenuName = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		RegisterClass(&wc);		// 윈도우 클래스 등록
	}

	void InitInstance(int nCmdShow) {
		// 메인 윈도우 생성
		HWND hWnd = CreateWindowW(L"name", L"www.third.com", WS_OVERLAPPEDWINDOW, 100, 90, 800, 800, NULL, NULL, mh_inst, NULL);
		ShowWindow(hWnd, nCmdShow);		// 어떻게 출력할 것인가 지정
		UpdateWindow(hWnd);		// WM_PAINT 메시지가 대기상태라면 즉시 수행하게함
	}

	int Run() {
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))		// WM_QUIT 메시지가 발생할 때까지 반복
		{
			TranslateMessage(&msg);		// 키보드 메시지 발생시에 추가 메시지를 발생할지 여부 체크
			DispatchMessage(&msg);		// 발생된 메시지 처리
		}
		return msg.wParam;
	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CWinApp app(hInstance);

	app.InitApplication();
	app.InitInstance(nCmdShow);
	return app.Run();
	
}

/*
	MFC : CWinApp + CWnd
	CWinApp : WinMain()
	CWnd : WndProc()

	CFrameWnd : 틀을 가진 윈도우창
	CView : 툴바에 따른 좌표가 바뀌는 윈도우창
	CDocument : 데이터파일 입출력
	-> CWinApp + CFrameWnd + CView + CDocument

	SDI : CWinApp + CFrameWnd + CView
	MDI : 하나의 프로세스창에 여러 창들이 있음, 메모리절약용, 안씀
	Dialog-based : CWinApp + CDialog

	두꺼운 프레임 : dick Frame = 대화상자 리사이즈
	시스템 메뉴 : 아이콘, X버튼 등
	정보 상자 : 만든이정보
	공용 컨트롤 매니페스트 : 체크하면 웹형식 버튼리소스, 체크해제하면 옛날형식 버튼리소스

	공부는 같은내용을 3번씩 본다
	첫번째는 실습하지않고 빠르게 쭉 본다.
	두번째는 실습하며 본다.
	세번째는 안보면서 기억을 더듬어가며 실습하며 본다.
	정말 이해가안되는거나 기억이 안나는 부분은 체크하고 다음번에 체크된 것만 한번 더 본다.

	진도가 너무 느리면 까먹는다.
	그러면 앞부분만 계속 빙글빙글 돌게된다.
	부족하다느껴도 진도를 계속 빼야한다.
	
*/