#include "framework.h"

#include <new>
#include <windowsx.h>
#include <mfplay.h>
#include <mferror.h>
#include <shobjidl.h>   // defines IFileOpenDialog
#include <strsafe.h>
#include <Shlwapi.h>

#include "MFPlayer1.h"

#pragma comment(lib, "mfplay.lib")
#pragma comment(lib, "shlwapi.lib")


IMFPMediaPlayer* g_pPlayer = NULL;      // The MFPlay player object.
MediaPlayerCallback* g_pPlayerCB = NULL;    // Application callback object.

BOOL                    g_bHasVideo = FALSE;

void ShowErrorMessage(PCWSTR format, HRESULT hrErr)
{
    HRESULT hr = S_OK;
    WCHAR msg[MAX_PATH];

    hr = StringCbPrintf(msg, sizeof(msg), L"%s (hr=0x%X)", format, hrErr);

    if (SUCCEEDED(hr))
    {
        MessageBox(NULL, msg, L"Error", MB_ICONERROR);
    }
}


HRESULT PlayMediaFile(HWND hwnd, const WCHAR* sURL)
{
    HRESULT hr = S_OK;

    // Create the MFPlayer object.
    if (g_pPlayer == NULL)
    {
        g_pPlayerCB = new (std::nothrow) MediaPlayerCallback();

        if (g_pPlayerCB == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto done;
        }

        hr = MFPCreateMediaPlayer(
            NULL,
            FALSE,          // Start playback automatically?
            0,              // Flags
            g_pPlayerCB,    // Callback pointer
            hwnd,           // Video window
            &g_pPlayer
        );

        if (FAILED(hr)) { goto done; }
    }

    // Create a new media item for this URL.
    hr = g_pPlayer->CreateMediaItemFromURL(sURL, FALSE, 0, NULL);

    // The CreateMediaItemFromURL method completes asynchronously. 
    // The application will receive an MFP_EVENT_TYPE_MEDIAITEM_CREATED 
    // event. See MediaPlayerCallback::OnMediaPlayerEvent().


done:
    return hr;
}


//-------------------------------------------------------------------
// OnFileOpen
//
// Handles the "File Open" command.
//-------------------------------------------------------------------

void OnFileOpen(HWND hwnd)
{
    HRESULT hr = S_OK;

    IFileOpenDialog* pFileOpen = NULL;
    IShellItem* pItem = NULL;

    PWSTR pwszFilePath = NULL;

    // Create the FileOpenDialog object.
    hr = CoCreateInstance(
        __uuidof(FileOpenDialog),
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pFileOpen)
    );

    if (FAILED(hr)) { goto done; }


    hr = pFileOpen->SetTitle(L"Select a File to Play");

    if (FAILED(hr)) { goto done; }


    // Show the file-open dialog.
    hr = pFileOpen->Show(hwnd);

    if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
    {
        // User cancelled.
        hr = S_OK;
        goto done;
    }
    if (FAILED(hr)) { goto done; }


    // Get the file name from the dialog.
    hr = pFileOpen->GetResult(&pItem);

    if (FAILED(hr)) { goto done; }


    hr = pItem->GetDisplayName(SIGDN_URL, &pwszFilePath);

    if (FAILED(hr)) { goto done; }


    // Open the media file.
    hr = PlayMediaFile(hwnd, pwszFilePath);

    if (FAILED(hr)) { goto done; }

done:
    if (FAILED(hr))
    {
        ShowErrorMessage(L"Could not open file.", hr);
    }

    CoTaskMemFree(pwszFilePath);

    if (pItem)
    {
        pItem->Release();
    }
    if (pFileOpen)
    {
        pFileOpen->Release();
    }
}



void OnMediaItemCreated(MFP_MEDIAITEM_CREATED_EVENT* pEvent)
{
    HRESULT hr = S_OK;

    // The media item was created successfully.

    if (g_pPlayer)
    {
        BOOL bHasVideo = FALSE, bIsSelected = FALSE;

        // Check if the media item contains video.
        hr = pEvent->pMediaItem->HasVideo(&bHasVideo, &bIsSelected);

        if (FAILED(hr)) { goto done; }

        g_bHasVideo = bHasVideo && bIsSelected;

        // Set the media item on the player. This method completes asynchronously.
        hr = g_pPlayer->SetMediaItem(pEvent->pMediaItem);
    }

done:
    if (FAILED(hr))
    {
        ShowErrorMessage(L"Error playing this file.", hr);
    }
}


//-------------------------------------------------------------------
// OnMediaItemSet
//
// Called when the IMFPMediaPlayer::SetMediaItem method completes.
//-------------------------------------------------------------------

void OnMediaItemSet(MFP_MEDIAITEM_SET_EVENT* /*pEvent*/)
{
    HRESULT hr = S_OK;

    hr = g_pPlayer->Play();

    if (FAILED(hr))
    {
        ShowErrorMessage(L"IMFPMediaPlayer::Play failed.", hr);
    }
}


void __stdcall MediaPlayerCallback::OnMediaPlayerEvent(MFP_EVENT_HEADER* pEventHeader)
{
    if (FAILED(pEventHeader->hrEvent))
    {
        ShowErrorMessage(L"Playback error", pEventHeader->hrEvent);
        return;
    }

    switch (pEventHeader->eEventType)
    {
    case MFP_EVENT_TYPE_MEDIAITEM_CREATED:
        OnMediaItemCreated(MFP_GET_MEDIAITEM_CREATED_EVENT(pEventHeader));
        break;

    case MFP_EVENT_TYPE_MEDIAITEM_SET:
        OnMediaItemSet(MFP_GET_MEDIAITEM_SET_EVENT(pEventHeader));
        break;
    }
}

void OnClose(HWND /*hwnd*/)
{
    if (g_pPlayer)
    {
        g_pPlayer->Shutdown();
        g_pPlayer->Release();
        g_pPlayer = NULL;
    }

    if (g_pPlayerCB)
    {
        g_pPlayerCB->Release();
        g_pPlayerCB = NULL;
    }

    PostQuitMessage(0);
}


//-------------------------------------------------------------------
// OnPaint
//
// Handles the WM_PAINT message.
//-------------------------------------------------------------------

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = 0;

    hdc = BeginPaint(hwnd, &ps);

    if (g_pPlayer && g_bHasVideo)
    {
        // Playback has started and there is video. 

        // Do not draw the window background, because the video 
        // frame fills the entire client area.

        g_pPlayer->UpdateVideo();
    }
    else
    {
        // There is no video stream, or playback has not started.
        // Paint the entire client area.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
    }

    EndPaint(hwnd, &ps);
}


void OnSize(HWND /*hwnd*/, UINT state, int /*cx*/, int /*cy*/)
{
    if (state == SIZE_RESTORED)
    {
        if (g_pPlayer)
        {
            // Resize the video.
            g_pPlayer->UpdateVideo();
        }
    }
}


void OnKeyDown(HWND /*hwnd*/, UINT vk, BOOL /*fDown*/, int /*cRepeat*/, UINT /*flags*/)
{
    HRESULT hr = S_OK;

    switch (vk)
    {
    case VK_SPACE:

        // Toggle between playback and paused/stopped.
        if (g_pPlayer)
        {
            MFP_MEDIAPLAYER_STATE state = MFP_MEDIAPLAYER_STATE_EMPTY;

            hr = g_pPlayer->GetState(&state);

            if (SUCCEEDED(hr))
            {
                if (state == MFP_MEDIAPLAYER_STATE_PAUSED || state == MFP_MEDIAPLAYER_STATE_STOPPED)
                {
                    hr = g_pPlayer->Play();
                }
                else if (state == MFP_MEDIAPLAYER_STATE_PLAYING)
                {
                    hr = g_pPlayer->Pause();
                }
            }
        }
        break;
    }

    if (FAILED(hr))
    {
        ShowErrorMessage(TEXT("Playback Error"), hr);
    }
}


void OnCommand(HWND hwnd, int id, HWND /*hwndCtl*/, UINT /*codeNotify*/)
{
    switch (id)
    {
    case IDM_OPEN_FILE:
        OnFileOpen(hwnd);
        break;

    case IDM_EXIT:
        OnClose(hwnd);
        break;
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        HANDLE_MSG(hWnd, WM_CLOSE, OnClose);
        HANDLE_MSG(hWnd, WM_KEYDOWN, OnKeyDown);
        HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hWnd, WM_SIZE, OnSize);

    case WM_ERASEBKGND:
        return 1;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    (void)HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
    if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
    {
        return 0;
    }

    WNDCLASS wc;

    LPCTSTR className = _T("Media foundation Player1");
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = hBrush;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = className;
    wc.lpszMenuName = MAKEINTRESOURCE(IDC_MFPLAYER1);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);

    HWND hWnd = CreateWindow(className, _T("Media foundation Player1"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
    // TODO : Initialize

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();
    DeleteObject(hBrush);
    DestroyWindow(hWnd);

    return (int)msg.wParam;
}


