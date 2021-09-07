#include "pch.h"        // 미리 컴파일된 헤더 (컴파일 속도 증가)
#include "framework.h"      // 윈도우즈 프레임워크에서 사용하는 헤더파일들
#include "Resource.h"

struct ClickInfo 
{
    int x, y;
    int flag;   // 0 -> 사격형, !0 -> 원
};

#define MAX_COUNT 100
#define AR_COUNT 20

ClickInfo g_click_pos[MAX_COUNT];
int g_count;        // 횟수

unsigned char g_flag[AR_COUNT][AR_COUNT];
unsigned char g_step = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY) {   // WM_CLOSE 후에 들어오는 메시지
        PostQuitMessage(0);     // 자신의 메시지 큐에 WM_QUIT 메시지를 넣는다.
    }
    else if (uMsg == WM_PAINT) {        // WM_PAINT코드 작동시 그림저장 (복구하는 코드)
        PAINTSTRUCT ps;     // 지역밖에서도 paint를 유지하기위해 구조체를 따로 선언
        HDC h_dc = BeginPaint(hWnd, &ps);
        SelectObject(h_dc, GetStockObject(DC_BRUSH));
        SelectObject(h_dc, GetStockObject(DC_PEN));
        /*if (g_flag) SetDCBrushColor(h_dc, RGB(0, 255, 0));
        else SetDCBrushColor(h_dc, RGB(0, 128, 0));*/
        //SetDCBrushColor(h_dc, RGB(0, 128 + g_flag[0] * 127, 0));       // 2지선다 플래그는 if문을 없앨 수 있음
        //SetDCBrushColor(h_dc, RGB(0, g_flag, 0));
        SetDCBrushColor(h_dc, RGB(230, 177, 91));
        SetDCPenColor(h_dc, RGB(255, 0, 0));
        for (int y = 0; y < AR_COUNT-1; y++) {
            for (int x = 0; x < AR_COUNT-1; x++) {
                //SetDCBrushColor(h_dc, RGB(0, g_flag[y][x], 0));
                Rectangle(h_dc, x*30+15, y*30+15, x*30 + 31+15, y*30 + 31+15);
            }
        }
        
        for (int y = 0; y < AR_COUNT; y++) {
            for (int x = 0; x < AR_COUNT; x++) {
                if (g_flag[y][x]) {
                    if (g_flag[y][x] == 1) SetDCBrushColor(h_dc, RGB(0, 0, 0));
                    else SetDCBrushColor(h_dc, RGB(255, 255, 255));
                    Ellipse(h_dc, x * 30, y * 30, x * 30 + 31, y * 30 + 31);
                }
            }
        }

        /*
        for (int i = 0; i < g_count; i++){
            if (g_click_pos[i].flag) {
                Ellipse(h_dc, g_click_pos[i].x - 10, g_click_pos[i].y - 10, g_click_pos[i].x + 10, g_click_pos[i].y + 10);
            } else{
                Rectangle(h_dc, g_click_pos[i].x - 10, g_click_pos[i].y - 10, g_click_pos[i].x + 10, g_click_pos[i].y + 10);
            }
        }
        */

        ClickInfo* p = g_click_pos;
        ClickInfo* p_end = p + g_count;
        for (p_end; p < p_end; p++)
        {
            if (p->flag) 
            {
                Ellipse(h_dc, p->x - 10, p->y - 10, p->x + 10, p->y + 10);
            }
            else {
                //Rectangle(h_dc, p->x - 10, p->y - 10, p->x + 10, p->y + 10);
            }
        }

        EndPaint(hWnd, &ps);
        return 0;
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        /*
        HDC h_dc = GetDC(hWnd);     // 그리기위해 DC를 가져옴
        HPEN h_red_pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
        HGDIOBJ h_old_pen = SelectObject(h_dc, h_red_pen);
        HBRUSH h_green_brush = CreateSolidBrush(RGB(0, 255, 0));
        HGDIOBJ h_old_brush = SelectObject(h_dc, h_green_brush);

        Rectangle(h_dc, 10, 10, 100, 100);  // 사각형 그리기

        SelectObject(h_dc, h_old_pen);
        SelectObject(h_dc, h_old_brush);
        DeleteObject(h_red_pen);
        DeleteObject(h_green_brush);
        */
        HDC h_dc = GetDC(hWnd);     // 그리기위해 DC를 가져옴
        SelectObject(h_dc, GetStockObject(DC_BRUSH));
        SelectObject(h_dc, GetStockObject(DC_PEN));
        SetDCBrushColor(h_dc, RGB(0, 128, 0));
        SetDCPenColor(h_dc, RGB(255, 0, 0));

        unsigned int x = lParam & 0x0000FFFF;
        unsigned int y = (lParam >> 16) & 0x0000FFFF;
        unsigned int x1 = lParam & 0x0000FFFF;
        unsigned int y1 = (lParam >> 16) & 0x0000FFFF;
        x1 /= 30;
        y1 /= 30;

        if (wParam & MK_CONTROL) Ellipse(h_dc, x - 10, y - 10, x + 10, y + 10);
        else if (y1 < AR_COUNT && x1 < AR_COUNT && g_flag[y][x] == 0) {
            g_flag[y1][x1] = g_step+1;
            g_step = !g_step;
            InvalidateRect(hWnd, NULL, 0);
        }
        /*else if (y <= 30) {     // unsigned int 이기 때문에 y>=0이 필요없음
            for (int i = 0; i < AR_COUNT; i++)
            {
                if (x >= i * 30 && x <= (i * 30 + 30)) {
                    g_flag[i] = g_flag[i] + 128;
                    InvalidateRect(hWnd, NULL, 0);
                    break;
                }
            }
            //g_flag = !g_flag;             // unsigned char g_flag = 0;
            //g_flag = g_flag + 128;    // 127, 255 overflow를 활용 (unsigned char g_flag = 127;)

            //if (g_flag) SetDCBrushColor(h_dc, RGB(0, 255, 0));
            //else SetDCBrushColor(h_dc, RGB(0, 128, 0));
            //Rectangle(h_dc, 20, 20, 100, 100);
            //InvalidateRect(hWnd, NULL, 0);
        }*/
        //else Rectangle(h_dc, x - 10, y - 10, x + 10, y + 10);

        if (g_count < MAX_COUNT) {
            g_click_pos[g_count].x = x;
            g_click_pos[g_count].y = y;
            g_click_pos[g_count].flag = wParam & MK_CONTROL;    // 0 or 8
            g_count++;
        }

        ReleaseDC(hWnd, h_dc);      // GetDC를 반납
        return 1;
    }
    else if (uMsg == WM_COMMAND) {
        if (wParam == ID_TEST_ITEM) {
            MessageBox(hWnd, L"테스트 메뉴 내용", L"타이틀", MB_OK);
        }
    }
    else if (uMsg == WM_CLOSE) {
        if (IDCANCEL == MessageBox(hWnd, L"종료 하시겠습니까?", L"프로그램이 종료됩니다", MB_OKCANCEL))
            return 1;
    }
    //else if (uMsg == WM_CREATE) {   // 가장 먼저 실행되는 함수
    //    memset(g_flag, 127, sizeof(g_flag));        // 원하는 크기만큼 원하는 메모리 셋팅
    //}

    // 개발자가 처리하지 않은 메시지들을 처리한다.
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// WinMain : 시작함수
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 윈도우 클래스 등록 작업
    WNDCLASS wc;

    wc.cbClsExtra = 0;      // 안씀
    wc.cbWndExtra = 0;      // 데이터전달용
    //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;        // 배경색 지정
    //wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    wc.hbrBackground = CreateSolidBrush(RGB(255,255,255));

    //wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // 커서 지정
    wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_MY_CUR));       // 커서 지정
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYWIN32));     // 응용 프로그램 로고 아이콘 지정
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;       // 기본 메시지 처리기 함수 지정
    wc.lpszClassName = L"sonsungjong";      // 윈도우 클래스 이름 지정
    wc.lpszMenuName = MAKEINTRESOURCE(IDC_MYWIN32);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);

    // 윈도우 생성
    HWND hWnd = CreateWindowEx(WS_EX_TOPMOST, L"sonsungjong", L"Caption Title", WS_OVERLAPPEDWINDOW, 100, 90, 800, 800, NULL, NULL, hInstance, NULL);
    
    ShowWindow(hWnd, nCmdShow);     // 윈도우를 화면에 어떻게 출력할 것인가 지정
    UpdateWindow(hWnd);     // WM_PAINT 메시지가 대기 상태라면 즉시 수행

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {      // WM_QUIT 메시지가 발생할 때까지 반복한다
        TranslateMessage(&msg);     // 키보드 메시지 발생시에 추가 메시지를 발생할 지 여부
        DispatchMessage(&msg);      // 발생된 메시지를 처리한다
    }

    DeleteObject(wc.hbrBackground);

    return msg.wParam;
}

/*
    Windows 데스크톱 마법사 -> 데스크톱 애플리케이션(.exe) -> 미리 컴파일된 헤더 체크 -> 만들기
    Win : 윈도우OS
    Wnd : 창 애플리케이션
    
    WM_PAINT:
    (0,0) 에서 오른쪽+, 아래쪽+
    client영역을 벗어나면 그림이 짤림
    다른 윈도우 영역(화면밖)으로 가면 무효화됨
    
    다시 그려야하는 경우 WndProc에 WM_PAINT를 작성

*/