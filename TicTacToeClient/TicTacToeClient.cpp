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
#define PORT 20222
#define BLOCK_SIZE      80
#define WM_CLIENT       WM_USER+1
#define WM_CONNECT      WM_USER+2

#define WM_GAME_INIT     WM_USER+3
#define WM_GAME_START       WM_USER+4
#define WM_GAME_END     WM_USER+5
#define WM_GAME_STATUS      WM_USER+6
#define WM_NETWORKPLAYER        WM_USER+7

// ���� ����
typedef enum{EMPTY, PLAYER1, PLAYER2} PLAYER;
typedef PLAYER CELL;
typedef enum {NONE, GAMEPLAYER1, GAMEPLAYER2, DRAW} WINNER;

#define BUFSIZE              512
#define BUFSIZE2            100

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    hInst = hInstance;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

    return 0;
}


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
    static BOOL bConnected = FALSE;             // ��������
    static BOOL bLogin = FALSE;                 // �α���
    static HWND hPicture;                           // ���� ��Ʈ��
    static BOOL bGameRunning = FALSE;

    switch (message)
    {
        case WM_INITDIALOG:
        {
            hList = GetDlgItem(hDlg, IDC_LIST1);
            // WinSock2.2 �ʱ�ȭ
            WSADATA wsaData;
            WSAStartup(0x0202, &wsaData);
            ZeroMemory(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = PORT;

            // �ٸ� ��ǻ���϶� IP���� �ٲ������
            inet_pton(AF_INET, IP, &addr.sin_addr.s_addr);
            SendMessage(hDlg, WM_CONNECT, 0, 0);

            // Picture ��Ʈ�� ũ�� ����
            hPicture = GetDlgItem(hDlg, IDC_PICTURE1);
            SetWindowPos(hPicture, NULL, 0, 0, BLOCK_SIZE * 3, BLOCK_SIZE * 3, SWP_NOMOVE);

            // Picture ��Ʈ�� ����Ŭ����
#ifdef _WIN64
            OldProc = (WNDPROC)SetWindowLong(hPicture, GWLP_WNDPROC, (LONG_PTR)PictureSubClassProc);             // x64
#elif _WIN32
            OldProc = (WNDPROC)SetWindowLong(hPicture, GWL_WNDPROC, (LONG)PictureSubClassProc);                   // x86
#endif
            SendMessage(hPicture, WM_GAME_INIT, 0, 0);

            HFONT hFont = CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("����"));
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
            // ��ȭ���� ���� �귯��
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
                bConnected = FALSE;
                SetWindowText(hDlg, _T("Disconnected..."));
                SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), _T("���� ����"));
            }
            else            // connection OK
            {
                bConnected = TRUE;
                WSAAsyncSelect(client, hDlg, WM_CLIENT, FD_READ | FD_CLOSE);
                SetWindowText(hDlg, _T("Connected..."));
                SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), _T("���� ������"));
            }
            return (INT_PTR)TRUE;
        }

        // wParam : socket, LOWORD(lParam) : network event
        case WM_CLIENT:
            switch (WSAGETSELECTEVENT(lParam))
            {
                case FD_CLOSE:
                    // ������ ���� ��
                    closesocket((SOCKET)wParam);
                    bLogin = FALSE;
                    bConnected = FALSE;

                    SetWindowText(hDlg, _T("Disconnected..."));
                    SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), _T("�α���"));
                    SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), _T("���� ����"));
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
                            bLogin = TRUE;
                            memcpy(loginID, packet.data, packet.len);
                            _stprintf_s(strTemp, sizeof(strTemp)/sizeof(strTemp[0]), _T("[%s]�� ȯ���մϴ�."), loginID);
                            SetWindowText(hDlg, strTemp);
                            SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), _T("�α� �ƿ�"));
                            break;
                        }
                        case LOGOUT:
                        {
                            SendMessage(hList, LB_RESETCONTENT, 0, 0);
                            bGameRunning = SendMessage(hPicture, WM_GAME_STATUS, 0, 0);
                            if (bGameRunning) {
                                SendMessage(hPicture, WM_GAME_INIT, 0, 0); 
                            }
                            bLogin = FALSE;
                            memset(loginID, 0, BUFSIZE);
                            SetWindowText(hDlg, _T("�α����� �ʿ��մϴ�."));
                            SetWindowText(GetDlgItem(hDlg, IDC_BUTTON_LOGIN), _T("�α���"));
                            break;
                        }
                        case LOGEXIST:
                        {
                            bLogin = FALSE;
                            SetWindowText(hDlg, _T("������ ���� ���̵� �����մϴ�."));
                            break;
                        }
                        case OTHERLOGIN:
                        {
                            // IDC_LIST1 �� �߰�
                            memcpy(recvID, packet.data, packet.len);
                            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)recvID);
                            break;
                        }
                        case OTHERLOGOUT:
                        {
                            // IDC_LIST1 ���� ����
                            memcpy(recvID, packet.data, packet.len);
                            unsigned int k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)recvID);
                            SendMessage(hList, LB_DELETESTRING, k, 0);

                            TCHAR networkID[100];
                            bGameRunning = SendMessage(hPicture, WM_GAME_STATUS, (WPARAM)&networkID, 0);
                            // ���� ���� ��밡 �α׾ƿ�
                            if (bGameRunning && _tcscmp(recvID, networkID) == 0) {
                                SendMessage(hPicture, WM_GAME_INIT, 0, 0); 
                            }
                            break;
                        }
                        case GAME_ASK:
                        {
                            // fromID <-> toID ��ȯ�� ����
                            GAME_DATA temp, gdata;
                            memcpy(&temp, packet.data, packet.len);

                            gdata.lenFrom = temp.lenTo;         // temp.lenTo == loginID
                            memcpy(gdata.fromID, temp.toID, temp.lenTo);
                            
                            gdata.lenTo = temp.lenFrom;
                            memcpy(gdata.toID, temp.fromID, temp.lenFrom);

                            bGameRunning = SendMessage(hPicture, WM_GAME_STATUS, 0, 0);
                            if (bGameRunning) {
                                packet.ptype = GAME_REJECT; 
                            }
                            else
                            {
                                if (MessageBox(hDlg, _T("�����ұ��?"), _T("Msg"), MB_YESNO | MB_TOPMOST) == IDYES)
                                {
                                    // ������ ����� PLAYER 2
                                    packet.ptype = GAME_ACCEPT;
                                    SendMessage(hPicture, WM_GAME_START, (WPARAM)PLAYER2, (LPARAM)(temp.fromID));
                                    unsigned int k = SendMessage(hList, LB_FINDSTRINGEXACT, 1, (LPARAM)temp.fromID);
                                    SendMessage(hList, LB_SETCURSEL, k, 0);
                                    TCHAR str[100];
                                    _stprintf_s(str, sizeof(str)/sizeof(str[0]), _T("Player2[%s]: X"), loginID);
                                    SetDlgItemText(hDlg, IDC_BUTTON_GAME, str);
                                    EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), FALSE);
                                    _stprintf_s(str, sizeof(str)/sizeof(str[0]), _T("Player1[%s]: O"), temp.fromID);
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
                            // ��Ŷ ����
                            packet.len = sizeof(GAME_DATA);
                            memcpy(packet.data, &gdata, packet.len);
                            send(client, (char*)&packet, sizeof(PACKET), 0);
                            break;
                        }
                        case GAME_ACCEPT:
                        {
                            // ��û�ؼ� ������ ���� �޽��� (��û�� ���)
                            GAME_DATA temp;
                            memcpy(&temp, packet.data, packet.len);
                            //  ��û�� ����� PLAYER1
                            SendMessage(hPicture, WM_GAME_START, (WPARAM)PLAYER1, (LPARAM)(temp.fromID));
                            TCHAR str[100];
                            _stprintf_s(str, sizeof(str) / sizeof(str[0]), _T("Player1[%s]: O"), loginID);
                            SetDlgItemText(hDlg, IDC_BUTTON_GAME, str);
                            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), FALSE);
                            _stprintf_s(str, sizeof(str)/sizeof(str[0]), _T("Player2[%s]: X"), temp.fromID);
                            SetDlgItemText(hDlg, IDC_STATIC_PLAYER, str);
                            ShowWindow(GetDlgItem(hDlg, IDC_STATIC_PLAYER), SW_SHOW);
                            // ���� : O - Turn
                            SetDlgItemText(hDlg, IDC_STATIC_TURN, _T("O - Turn"));
                            ShowWindow(GetDlgItem(hDlg, IDC_STATIC_TURN), SW_SHOW);
                            break;
                        }
                        case GAME_REJECT:
                        {
                            bGameRunning = FALSE;
                            EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_GAME), TRUE);
                            MessageBox(hDlg, _T("�����Ͽ����ϴ�."), _T("Msg"), MB_OK | MB_TOPMOST);
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
                            if ((WINNER)temp.ix == DRAW) { 
                                _stprintf_s(str, sizeof(str) / sizeof(str[0]), _T("Draw!")); 
                            }
                            else { 
                                _stprintf_s(str, sizeof(str) / sizeof(str[0]), _T("Winner = %s"), ((WINNER)temp.ix == GAMEPLAYER1) ? _T("O") : _T("X")); 
                            }

                            if (MessageBox(hDlg, str, _T("Msg"), MB_OK | MB_TOPMOST) == IDOK) {
                                SendMessage(hPicture, WM_GAME_INIT, 0, 0); 
                            }
                            SetDlgItemText(hDlg, IDC_BUTTON_GAME, _T("���� ����"));
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
                    if (!bConnected) {
                        SendMessage(hDlg, WM_CONNECT, 0, 0); 
                    }
                    else
                    {
                        closesocket(client);
                        SendMessage(hDlg, WM_CLIENT, 0, MAKEWORD(FD_CLOSE, 0));
                    }
                    break;
                case IDC_BUTTON_LOGIN:
                    if (!bConnected) { 
                        break;
                    }
                    if (!bLogin)
                    {
                        if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, DlgProcLogin) == IDOK)
                        {
                            if (_tcslen(loginID) == 0) {
                                break; 
                            }
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
                            MessageBox(hDlg, _T("���� ��븦 �����ϼ���!"), _T("Msg"), MB_OK | MB_TOPMOST);
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
                        MessageBox(hDlg, _T("��밡 �������Դϴ�. �ٸ� ��븦 �����ϼ���!"), _T("Msg"), MB_OK|MB_TOPMOST);
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

// ���� ��ȭ������ �޽��� ó����
INT_PTR CALLBACK DlgProcLogin(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
        {
            // �α��� ��ȭ���ڸ� �θ� ��ȭ���� ����� ���
            HWND hParent = GetParent(hDlg);
            RECT rcParent, rcDlg, rc;
            GetWindowRect(hParent, &rcParent);
            GetWindowRect(hDlg, &rcDlg);
            CopyRect(&rc, &rcParent);           // rc = rcDlgMain

            OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
            OffsetRect(&rc, -rc.left, -rc.top);
            OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);
            SetWindowPos(hDlg, HWND_TOP, rcParent.left + (rc.right / 2), rcParent.top + (rc.bottom / 2), 0, 0, SWP_NOSIZE);

            HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("����"));
            SendDlgItemMessage(hDlg, IDC_EDIT_LOGIN, WM_SETFONT, (WPARAM)hFont, FALSE);
            return (INT_PTR)TRUE;
        }
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
            case IDOK:
                GetDlgItemText(hDlg, IDC_EDIT_LOGIN, loginID, 128);
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            case IDCANCEL:
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
    }
    return (INT_PTR)FALSE;
}

void DrawBlock(HDC hdc, int ix, int iy, HBITMAP hBit)
{
    // BITMAP bit;
    // bit.bmWidth, bit.bmHeight;
    // GetObject(hBit, sizeof(BITMAP), &bit);
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBit = (HBITMAP)SelectObject(memDC, hBit);
    BitBlt(hdc, ix * BLOCK_SIZE, iy * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, memDC, 0, 0, SRCCOPY);
    SelectObject(memDC, oldBit);
    DeleteDC(memDC);
}

void DrawBoard(HDC hdc, HBITMAP hBitmap[], CELL board[][3])
{
    for (int ix = 0; ix < 3; ix++) {
        for (int iy = 0; iy < 3; iy++) {
            int img = (int)board[iy][ix];
            DrawBlock(hdc, ix, iy, hBitmap[img]);
        }
    }
}

WINNER WinnerCheck(int ix, int iy, CELL board[][3])
{
    //if (board[iy][ix] == EMPTY) {
    //    return NONE;
    //}
    if (board[iy][0] == board[iy][1] && board[iy][1] == board[iy][2]) {
        return (WINNER)board[iy][0];        // ����
    }
    else if (board[0][ix] == board[1][ix] && board[1][ix] == board[2][ix]) {
        return (WINNER)board[0][ix];            // ����
    }
    else if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return (WINNER)board[0][0];             // �밢��
    }
    else if (board[2][0] == board[1][1] && board[1][1] == board[0][2]) {
        return (WINNER)board[2][0];             // ���밢��
    }
    else {
        // ���
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                if (board[y][x] == EMPTY) {
                    return NONE;
                }
            }
        }
        return DRAW;
    }
    return NONE;
}

INT_PTR CALLBACK PictureSubClassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // static SOCKET client;
    static TCHAR networkID[100];
    static HBITMAP hBitmap[3];
    static CELL board[3][3];
    static BOOL bBitmapLoaded = FALSE;
    static PLAYER gamePlayer;
    static BOOL bGameRunning;
    static BOOL bMyTurn = TRUE;
    static HWND hParent;        // �θ� ��ȭ����

    switch (message)
    {
    case WM_GAME_INIT:
        if (!bBitmapLoaded)
        {
            bBitmapLoaded = TRUE;
            hBitmap[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP0));
            hBitmap[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
            hBitmap[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        }
        hParent = GetParent(hWnd);
        SetDlgItemText(hParent, IDC_BUTTON_GAME, _T("���� ����"));
        EnableWindow(GetDlgItem(hParent, IDC_BUTTON_GAME), TRUE);
        ShowWindow(GetDlgItem(hParent, IDC_STATIC_PLAYER), SW_HIDE);
        ShowWindow(GetDlgItem(hParent, IDC_STATIC_TURN), SW_HIDE);

        // ���� ����
        SendMessage(GetDlgItem(hParent, IDC_LIST1), LB_SETCURSEL, -1, 0);
        bGameRunning = FALSE;
        memset(board, EMPTY, sizeof(board));
        InvalidateRect(hWnd, NULL, FALSE);
        return TRUE;
    case WM_GAME_START:
        gamePlayer = (PLAYER)wParam;
        if (gamePlayer == PLAYER1) {
            bMyTurn = TRUE;
        }
        else {
            bMyTurn = FALSE;
        }
        memcpy(networkID, (TCHAR*)lParam, (_tcslen((TCHAR*)lParam) + 1) * sizeof(TCHAR));
        bGameRunning = TRUE;
        return TRUE;
    case WM_GAME_STATUS:
        {
            TCHAR* ptr = (TCHAR*)wParam;
            if (ptr) {
                memcpy(ptr, networkID, (_tcslen(networkID) + 1) * sizeof(TCHAR));
            }
            return bGameRunning;
        }
    case WM_NETWORKPLAYER:
        {
            int ix = (int)wParam;
            int iy = (int)lParam;
            if (board[iy][ix] != EMPTY || !bGameRunning) {
                return TRUE;
            }

            // ���� �����͸� ����ϱ� ���� ���
            board[iy][ix] = (gamePlayer == PLAYER1) ? PLAYER2 : PLAYER1;
            bMyTurn = TRUE;
            if (gamePlayer == PLAYER1) {
                SetDlgItemText(hParent, IDC_STATIC_TURN, _T("O - turn"));
            }
            else {
                SetDlgItemText(hParent, IDC_STATIC_TURN, _T("X - turn"));
            }
            InvalidateRect(hWnd, NULL, FALSE);
            return TRUE;
        }
    case WM_LBUTTONDOWN:
        {
            if (!bMyTurn) {
                return TRUE;
            }
            int ix, iy;
            ix = (int)(LOWORD(lParam) / BLOCK_SIZE);
            ix = (int)(HIWORD(lParam) / BLOCK_SIZE);
            if (board[iy][ix] != EMPTY || !bGameRunning) {
                return TRUE;
            }

            // EMPTY�� ��츸
            board[iy][ix] = gamePlayer;

            // ��Ʈ��ũ ����
            PACKET packet;
            packet.ptype = GAME_POINT;
            packet.len = sizeof(GAME_DATA);

            GAME_DATA gameData;
            // from
            gameData.lenFrom = (unsigned short)(_tcslen(loginID) + 1) * sizeof(TCHAR);
            memcpy(gameData.fromID, loginID, gameData.lenFrom);

            // to
            gameData.lenTo = (unsigned short)(_tcslen(networkID) + 1) * sizeof(TCHAR);
            memcpy(gameData.toID, networkID, gameData.lenTo);

            gameData.ix = ix;
            gameData.iy = iy;
            memcpy(packet.data, &gameData, packet.len);
            send(client, (char*)&packet, sizeof(PACKET), 0);

            InvalidateRect(hWnd, NULL, FALSE);          // ���� Ŭ���ݿ�

            WINNER winner = WinnerCheck(ix, iy, board);
            if (winner != NONE) {
                bGameRunning = FALSE;
                TCHAR str[128];
                if (winner == DRAW) {
                    _stprintf_s(str, sizeof(str) / sizeof(str[0]), _T("Draw!"));
                }
                else {
                    _stprintf_s(str, sizeof(str) / sizeof(str[0]), _T("Winner = %s"), (winner == GAMEPLAYER1) ? _T("O") : _T("X"));
                }

                // from, to�� ���� �� �״��
                packet.ptype = GAME_END;
                // packet.len = sizeof(GAME_DATA);
                gameData.ix = (unsigned short)winner;
                memcpy(packet.data, &gameData, packet.len);
                send(client, (char*)&packet, sizeof(PACKET), 0);

                if (MessageBox(hWnd, str, _T("Msg"), MB_OK | MB_TOPMOST) == IDOK) {
                    SendMessage(hWnd, WM_GAME_INIT, 0, 0);
                }
            }
            else {
                bMyTurn = FALSE;            // �÷��̾� ��ȯ
                if (gamePlayer == PLAYER1) {
                    SetDlgItemText(hParent, IDC_STATIC_TURN, _T("X - turn"));
                }
                else {
                    SetDlgItemText(hParent, IDC_STATIC_TURN, _T("O - turn"));
                }
            }
            return TRUE;
        }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DrawBoard(hdc, hBitmap, board);
            EndPaint(hWnd, &ps);
            return TRUE;
        }
    case WM_GAME_END:
        for (int i = 0; i < 3; i++) {
            DeleteObject(hBitmap[i]);
            bBitmapLoaded = FALSE;
            bGameRunning = FALSE;
            return TRUE;
        }
    }
    return CallWindowProc((WNDPROC)OldProc, hWnd, message, wParam, lParam);
}