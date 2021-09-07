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
		color = RGB(0, 0, 0);		// RGB�� �ʱ�ȭ
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
		color = RGB(rand() % 256, rand() % 256, rand() % 256);		// RGB����
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
	// �簢�� ���׸���
	ap_dc->FillSolidRect();

	// �� ���׸���
	ap_dc->SelectStockObject(DC_BRUSH);
	ap_dc->SelectStockObject(DC_PEN);
	ap_dc->SetDCBrushColor();
	ap_dc->Ellipse();
}

// CWnd
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) {
		// WM_CLOSE ó�� �Ŀ� ������ �޽���
		KillTimer(hWnd, 1);
		PostQuitMessage(0);		// �޽��� ť�� WM_QUIT�� put
	}
	else if (uMsg == WM_PAINT) {
		CPaintDC dc(hWnd);
		Draw(&dc);
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		// ���콺 ������ư ����
		unsigned int x = lParam & 0x0000FFFF;
		unsigned int y = (lParam >> 16) & 0x0000FFFF;
	}
	else if (uMsg == WM_CREATE) {
		srand((unsigned int)time(NULL));		// �������� ���� �ʱ�ȭ
		// Ÿ�̸� ����
		SetTimer(hWnd, 1, 100, NULL);		// �ݹ��Լ��� NULL�� �ϸ� 100ms���� WM_TIMER (ID : 1)
	}
	else if (uMsg == WM_TIMER) {
		if (wParam == 1) {
			/*
			HDC h_dc = GetDC(hWnd);

			int x = rand() % 800;		// 0~799
			int y = rand() % 800;		// 0~799
			int r = rand() % 50+10;		// 0~49 +10
			COLORREF color = RGB(rand()%256, rand()%256, rand()%256);		// RGB�� ����
			SelectObject(h_dc, GetStockObject(DC_BRUSH));
			SetDCBrushColor(h_dc, color);
			Ellipse(h_dc, x - r, y - r, x + r, y + r);

			ReleaseDC(hWnd, h_dc);
			

			CClientDC dc(hWnd);
			// �簢�� ���׸���
			dc.FillSolidRect();

			// �� ���׸���
			dc.SelectStockObject(DC_BRUSH);
			dc.SetDCBrushColor();
			dc.Ellipse();
			*/

			CClientDC dc(hWnd);
			Draw(&dc);
		}
	}

	// �����ڰ� ó������ ���� �޽������� ó��
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
		// ������ Ŭ���� ���
		WNDCLASS wc;

		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		// ����
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Ŀ�� ����
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);			// �ΰ� ������
		wc.hInstance = mh_inst;
		wc.lpfnWndProc = WndProc;			// �⺻ �޽��� ó���� �Լ�
		wc.lpszClassName = L"name";		// ������ Ŭ���� �̸�
		wc.lpszMenuName = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		RegisterClass(&wc);		// ������ Ŭ���� ���
	}

	void InitInstance(int nCmdShow) {
		// ���� ������ ����
		HWND hWnd = CreateWindowW(L"name", L"www.third.com", WS_OVERLAPPEDWINDOW, 100, 90, 800, 800, NULL, NULL, mh_inst, NULL);
		ShowWindow(hWnd, nCmdShow);		// ��� ����� ���ΰ� ����
		UpdateWindow(hWnd);		// WM_PAINT �޽����� �����¶�� ��� �����ϰ���
	}

	int Run() {
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))		// WM_QUIT �޽����� �߻��� ������ �ݺ�
		{
			TranslateMessage(&msg);		// Ű���� �޽��� �߻��ÿ� �߰� �޽����� �߻����� ���� üũ
			DispatchMessage(&msg);		// �߻��� �޽��� ó��
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

	CFrameWnd : Ʋ�� ���� ������â
	CView : ���ٿ� ���� ��ǥ�� �ٲ�� ������â
	CDocument : ���������� �����
	-> CWinApp + CFrameWnd + CView + CDocument

	SDI : CWinApp + CFrameWnd + CView
	MDI : �ϳ��� ���μ���â�� ���� â���� ����, �޸������, �Ⱦ�
	Dialog-based : CWinApp + CDialog

	�β��� ������ : dick Frame = ��ȭ���� ��������
	�ý��� �޴� : ������, X��ư ��
	���� ���� : ����������
	���� ��Ʈ�� �Ŵ��佺Ʈ : üũ�ϸ� ������ ��ư���ҽ�, üũ�����ϸ� �������� ��ư���ҽ�

	���δ� ���������� 3���� ����
	ù��°�� �ǽ������ʰ� ������ �� ����.
	�ι�°�� �ǽ��ϸ� ����.
	����°�� �Ⱥ��鼭 ����� ������ �ǽ��ϸ� ����.
	���� ���ذ��ȵǴ°ų� ����� �ȳ��� �κ��� üũ�ϰ� �������� üũ�� �͸� �ѹ� �� ����.

	������ �ʹ� ������ ��Դ´�.
	�׷��� �պκи� ��� ���ۺ��� ���Եȴ�.
	�����ϴٴ����� ������ ��� �����Ѵ�.
	
*/