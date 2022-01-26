// TicTacToeClient.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "TicTacToeClient.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

HINSTANCE hInst;
INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgProcLogin(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#define BLOCK_SIZE      80
#define WM_CLIENT       WM_USER+1
#define WM_CONNECT      WM_USER+2

#define WM_GAME_INT     WM_USER+3
#define WM_GAME_START       WM_USER+4
#define WM_GAME_END     WM_USER+5
#define WM_GAME_STATUS      WM_USER+6
#define WM_NETWORKPLAYER        WM_USER+7

#define BUFSIZE              512
#define BUFSIZE2            100

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

    return 0;
}

// 게임 정보
typedef enum{EMPTY, PLAYER1, PLAYER2} PLAYER;
typedef PLAYER CELL;
typedef enum {NONE, GAMEPLAYER1, GAMEPLAYER2, DRAW} WINNER;

typedef enum {
    LOGIN = 1, LOGEXIST, LOGOUT, OTHERLOGIN, OTHERLOGOUT,
    GAME_ASK, GAME_ACCEPT, GAME_REJECT, GAME_POINT, GAME_END
} PACKET_TYPE;

typedef struct {
    PACKET_TYPE ptype;
    unsigned short len;         // length(data)
    char data[BUFSIZE];
} PACKET;

typedef struct {
    unsigned short lenFrom;
    TCHAR fromID[BUFSIZE2];
    unsigned short lenTo;
    TCHAR toID[BUFSIZE2];
    unsigned short ix;
    unsigned short iy;
}GAME_DATA;

TCHAR loginID[BUFSIZE];
WNDPROC OldProc;
INT_PTR CALLBACK pictureSubClassProc(HWND, UINT, WPARAM, LPARAM);
SOCKET client;

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hList;
    // static SOCKET client;
    static SOCKADDR_IN addr;
    PACKET packet;
    static bool bConnected = false;             // 서버연결
    static bool bLogin = false;                 // 로그인
    static HWND hPicture;                           // 게임 컨트롤
    static bool bGameRunning = false;

    switch (message)
    {
        case WM_INITDIALOG:
        {
            hList = GetDlgItem(hDlg, IDC_LIST1);
            // WinSock2.2 초기화
            
        }
    }

    return (INT_PTR)FALSE;
}
