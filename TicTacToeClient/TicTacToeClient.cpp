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

#define IP "127.0.0.1"
#define PORT 44554
#define BLOCK_SIZE      80
#define WM_CLIENT       WM_USER+1
#define WM_CONNECT      WM_USER+2

#define WM_GAME_INIT     WM_USER+3
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
            WSADATA wsaData;
            WSAStartup(0x0202, &wsaData);
            ZeroMemory(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = PORT;
            inet_pton(AF_INET, IP, &addr.sin_addr.s_addr);
            SendMessage(hDlg, WM_CONNECT, 0, 0);

            // Picture 컨트롤 크기 조정
            hPicture = GetDlgItem(hDlg, IDC_PICTURE1);
            SetWindowPos(hPicture, nullptr, 0, 0, BLOCK_SIZE * 3, BLOCK_SIZE * 3, SWP_NOMOVE);

            // Picture 컨트롤 서브클래싱
#ifdef _WIN64
            OldProc = (WNDPROC)SetWindowLong(hPicture, GWLP_WNDPROC, (LONG_PTR)PictureSubClassProc);             // x64
#elif _WIN32
            OldProc = (WNDPROC)SetWindowLong(hPicture, GWL_WNDPROC, (LONG)PictureSubClassProc);                   // x86
#endif
            SendMessage(hPicture, WM_GAME_INIT, 0, 0);

            HFONT hFont = CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("바탕"));
            SendDlgItemMessage(hDlg, IDC_BUTTON_LOGIN, WM_SETFONT, (WPARAM)hFont, FALSE);
            SendDlgItemMessage(hDlg, IDC_BUTTON_CONNECT, WM_SETFONT, (WPARAM)hFont, FALSE);
            SendDlgItemMessage(hDlg, IDC_BUTTON_GAME, WM_SETFONT, (WPARAM)hFont, FALSE);
            SendDlgItemMessage(hDlg, IDC_LIST1, WM_SETFONT, (WPARAM)hFont, FALSE);
            SendDlgItemMessage(hDlg, IDC_STATIC_TURN, WM_SETFONT, (WPARAM)hFont, FALSE);
            SendDlgItemMessage(hDlg, IDC_STATIC_PLAYER, WM_SETFONT, (WPARAM)hFont, FALSE);
            return (INT_PTR)TRUE;
        }
        case WM_CTLCOLORDLG:
        {
            // 대화상자 배경색 브러시
            return (INT_PTR)GetStockObject(GRAY_BRUSH);
        }
        case WM_CTLCOLORSTATIC:
        {
            HWND ctrlWnd = (HWND)lParam;
            HDC hdc = (HDC)wParam;
            if (ctrlWnd == GetDlgItem(hDlg, IDC_STATIC) || ctrlWnd == GetDlgItem(hDlg, IDC_STATIC_TURN))
            {
                SetTextColor(hdc, RGB(0, 0, 255));
                SetBkMode(hdc, TRANSPARENT);
                return (INT_PTR)GetStockObject(GRAY_BRUSH);
            }
        }
        break;
        case WM_CONNECT:
        {
            // IPv4, TCP stream, protocol
            client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (connect(client, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
            {
                closesocket(client);
                bConnected = false;
                SetWindowText(hDlg, _T("Disconnected..."));
                SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), _T("서버 연결"));
            }
            else            // connection OK
            {
                bConnected = true;
                WSAAsyncSelect(client, hDlg, WM_CLIENT, FD_READ | FD_CLOSE);
                SetWindowText(hDlg, _T("Connected..."));
                SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), _T("서버 나가기"));
            }
            return (INT_PTR)TRUE;
        }

        // wParam : socket, LOWORD(lParam) : network event
        case WM_CLIENT:
            switch (WSAGETSELECTEVENT(lParam))
            {
                case FD_CLOSE:
                    // 소켓이 닫힐 때
                    closesocket((SOCKET)wParam);
                    bLogin = false;
                    bConnected = false;

                    SetWindowText(hDlg, _T("Disconnected..."));
                    SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), _T("로그인"));
                    SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), _T("서버 연결"));
                    SendMessage(hList, LB_RESETCONTENT, 0, 0);
                    SendMessage(hPicture, WM_GAME_INIT, 0, 0);
                    break;
                case FD_READ:
                {
                    // client = (SOCKET)wParam;
                    TCHAR recvID[BUFSIZE];
                    TCHAR strTemp[BUFSIZE];
                    int nR = recv(client, (char*)&packet, sizeof(PACKET), 0);
                    switch (packet.ptype)
                    {
                        case LOGIN:
                        {
                            bLogin = true;
                            memcpy(loginID, packet.data, packet.len);
                            _stprintf_s(strTemp, sizeof(strTemp)/sizeof(strTemp[0]), _T("[%s]님 환영합니다."), loginID);
                            SetWindowText(hDlg, strTemp);
                            SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), _T("로그 아웃"));
                            break;
                        }
                        case LOGOUT:
                        {
                            SendMessage(hList, LB_RESETCONTENT, 0, 0);
                            bGameRunning = SendMessage(hPicture, WM_GAME_STATUS, 0, 0);
                            if (bGameRunning) { SendMessage(hPicture, WM_GAME_INIT, 0, 0); }
                            bLogin = false;
                            memset(loginID, 0, BUFSIZE);
                            SetWindowText(hDlg, _T("로그인이 필요합니다."));
                            SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), _T("로그인"));
                            break;
                        }
                        case LOGEXIST:
                        {
                            bLogin = false;
                            SetWindowText(hDlg, _T("서버에 같은 아이디가 존재합니다."));
                            break;
                        }
                        case OTHERLOGIN:
                        {
                            // IDC_LIST1 에 추가
                            memcpy(recvID, packet.data, packet.len);
                            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)recvID);
                            break;
                        }
                        case OTHERLOGOUT:
                        {
                            // IDC_LIST1 에서 삭제
                            memcpy(recvID, packet.data, packet.len);
                            unsigned int k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)recvID);
                            TCHAR networkID[100];
                            bGameRunning = SendMessage(hPicture, WM_GAME_STATUS, (WPARAM)&networkID, 0);
                            // 게임 중인 상대가 로그아웃
                            if (bGameRunning && _tcscmp(recvID, networkID) == 0) { SendMessage(hPicture, WM_GAME_INIT, 0, 0); }
                            break;
                        }
                        case GAME_ASK:
                        {
                            // fromID <-> toID 교환해 응답
                            GAME_DATA temp, gdata;
                            memcpy(&temp, packet.data, packet.len);
                            gdata.lenFrom = temp.lenTo;         // temp.lenTo == loginID

                            memcpy(gdata.fromID, temp.toID, temp.lenTo);
                            gdata.lenTo = temp.lenFrom;
                            
                            memcpy(gdata.toID, temp.fromID, temp.lenFrom);
                            bGameRunning = SendMessage(hPicture, WM_GAME_STATUS, 0, 0);
                            if (bGameRunning) { packet.ptype = GAME_REJECT; }
                            else
                            {
                                if (MessageBox(hDlg, _T("게임할까요?"), _T("Msg"), MB_YESNO | MB_TOPMOST) == IDYES)
                                {
                                    // 수락한 사람은 PLAYER 2
                                    packet.ptype = GAME_ACCEPT;
                                    SendMessage(hPicture, WM_GAME_START, (WPARAM)PLAYER2, (LPARAM)(temp.fromID));
                                    unsigned int k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)temp.fromID);
                                    SendMessage(hList, LB_SETCURSEL, k, 0);
                                    TCHAR str[100];
                                    _stprintf_s(str, sizeof(str)/sizeof(str[0]), _T("Player2[%s]: X"), loginID);
                                    SetDlgItemText(hDlg, IDC_BUTTON_GAME, str);
                                    EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), FALSE);
                                    _stprintf_s(str, _T("Player1[%s]: O"), temp.fromID);
                                    SetDlgItemText(hDlg, IDC_STATIC_PLAYER, str);
                                    ShowWindow(GetDlgItem(hDlg, IDC_STATIC_PLAYER), SW_SHOW);
                                    SetDlgItemText(hDlg, IDC_STATIC_TURN, _T("O - Turn"));
                                    ShowWindow(GetDlgItem(hDlg, IDC_STATIC_TURN), SW_SHOW);
                                }
                                else
                                {
                                    packet.ptype = GAME_REJECT;
                                }
                            }
                            // 패킷 응답
                            packet.len = sizeof(GAME_DATA);
                            memcpy(packet.data, &gdata, packet.len);
                            send(client, (char*)&packet, sizeof(PACKET), 0);
                            break;
                        }
                        case GAME_ACCEPT:
                        {
                            // 요청해서 수락을 받은 메시지 (요청한 사람)
                            GAME_DATA temp;
                            memcpy(&temp, packet.data, packet.len);
                            //  요청한 사람은 PLAYER1
                            SendMessage(hPicture, WM_GAME_START, (WPARAM)PLAYER1, (LPARAM)(temp.fromID));
                            TCHAR str[100];
                            _stprintf_s(str, sizeof(str) / sizeof(str[0]), _T("Player1[%s]: O"), loginID);
                            SetDlgItemText(hDlg, IDC_BUTTON_GAME, str);
                            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), SW_SHOW);
                            _stprintf_s(str, _T("Player2[%s]: X"), temp.fromID);
                            SetDlgItemText(hDlg, IDC_STATIC_PLAYER, str);
                            ShowWindow(GetDlgItem(hDlg, IDC_STATIC_PLAYER), SW_SHOW);
                            // 시작 : O - Turn
                            SetDlgItemText(hDlg, IDC_STATIC_TURN, _T("O - Turn"));
                            ShowWindow(GetDlgItem(hDlg, IDC_STATIC_TURN), SW_SHOW);
                            break;
                        }
                        case GAME_REJECT:
                        {
                            bGameRunning = false;
                            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), TRUE);
                            MessageBox(hDlg, _T("거절하였습니다."), _T("Msg"), MB_OK | MB_TOPMOST);
                            break;
                        }
                        case GAME_POINT:
                        {
                            GAME_DATA temp;
                            memcpy(&temp, packet.data, packet.len);
                            int ix = temp.ix;
                            int iy = temp.iy;
                            SendMessage(hPicture, WM_NETWORKPLAYER, (WPARAM)ix, (LPARAM)iy);
                            break;
                        }
                        case GAME_END:
                        {
                            GAME_DATA temp;
                            memcpy(&temp, packet.data, packet.len);

                            TCHAR str[128];
                            if ((WINNER)temp.ix == DRAW) { _stprintf_s(str, sizeof(str) / sizeof(str[0]), _T("Draw!")); }
                            else { _stprintf_s(str, sizeof(str) / sizeof(str[0]), _T("Winner = %s"), ((WINNER)temp.ix == GAMEPLAYER1) ? _T("O") : _T("X")); }

                            if (MessageBox(hDlg, str, _T("Msg"), MB_OK | MB_TOPMOST) == IDOK) { SendMessage(hPicture, WM_GAME_INIT, 0, 0); }
                            SetDlgItemText(hDlg, IDC_BUTTON_GAME, _T("게임 시작"));
                            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), TRUE);
                            ShowWindow(GetDlgItem(hDlg, IDC_STATIC_TURN), SW_HIDE);
                        }
                    }
                }
                return (INT_PTR)TRUE;
            }
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDC_BUTTON_CONNECT:
                    if (!bConnected) { SendMessage(hDlg, WM_CONNECT, 0, 0); }
                    else
                    {
                        closesocket(client);
                        SendMessage(hDlg, WM_CLIENT, 0, MAKEWORD(FD_CLOSE, 0));
                    }
                    break;
                case IDC_BUTTON_LOGIN:
                    if (!bConnected) { break; }
                    if (!bLogin)
                    {
                        if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, DlgProcLogin) == IDOK)
                        {
                            if (_tcslen(loginID) == 0) { break; }
                            packet.ptype = LOGIN;
                            packet.len = (unsigned short)(_tcslen(loginID) + 1) * sizeof(TCHAR);
                            memcpy(packet.data, loginID, packet.len);
                            send(client, (char*)&packet, sizeof(PACKET), 0);
                        }
                    }
                    else
                    {
                        packet.ptype = LOGOUT;
                        packet.len = (unsigned short)(_tcslen(loginID) + 1) * sizeof(TCHAR);
                        memcpy(packet.data, loginID, packet.len);
                        send(client, (char*)&packet, sizeof(PACKET), 0);
                    }
                    break;
                case IDC_BUTTON_GAME:
                    if (bGameRunning == FALSE)
                    {
                        EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), FALSE);
                        packet.ptype = GAME_ASK;
                        packet.len = sizeof(GAME_DATA);
                        GAME_DATA temp;
                        // from
                        temp.lenFrom = (unsigned short)(_tcslen(loginID) + 1) * sizeof(TCHAR);
                        memcpy(temp.fromID, loginID, temp.lenFrom);

                        // to
                        TCHAR strID[128];
                        int i = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
                        if (i < 0)
                        {
                            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), TRUE);
                            MessageBox(hDlg, _T("게임 상대를 선택하세요!"), _T("Msg"), MB_OK | MB_TOPMOST);
                            break;
                        }
                        SendMessage(hList, LB_GETTEXT, i, (LPARAM)strID);
                        temp.lenTo = (unsigned short)(_tcslen(strID) + 1) * sizeof(TCHAR);
                        memcpy(temp.toID, strID, temp.lenTo);
                        memcpy(packet.data, &temp, packet.len);
                        send(client, (char*)&packet, sizeof(PACKET), 0);
                    }
                    else
                    {
                        EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), TRUE);
                        MessageBox(hDlg, _T("상대가 게임중입니다. 다른 상대를 선택하세요!"), _T("Msg"), MB_OK|MB_TOPMOST);                    
                    }
                    break;
            }
            return (INT_PTR)TRUE;
        case WM_CLOSE:
            if (bLogin)
            {
                packet.ptype = LOGOUT;
                packet.len = (unsigned short)(_tcslen(loginID) + 1) * sizeof(TCHAR);
                memcpy(packet.data, loginID, packet.len);
                send(client, (char*)&packet, sizeof(PACKET), 0);
            }
            SendMessage(hPicture, WM_GAME_END, 0, 0);
            DestroyWindow(hDlg);
            return TRUE;
        case WM_DESTROY:
            return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}

// 정보 대화상자의 메시지 처리기
INT_PTR CALLBACK DlgProcLogin(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
        {
            // 로그인 대화상자를 부모 대화상자 가운데에 출력

        }
    }
}


INT_PTR CALLBACK PictureSubClassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


    return CallWindowProc((WNDPROC)OldProc, hWnd, message, wParam, lParam);
}