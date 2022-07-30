// ImageDrawD2Ddlg.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "ImageDrawD2Ddlg.h"

#include <d2d1.h>
#include <wincodec.h>
#pragma comment(lib, "D2D1.lib")
using namespace D2D1;

ID2D1Factory* gp_factory;
ID2D1HwndRenderTarget* gp_render_target;
ID2D1Bitmap* gp_bitmap;
D2D1_RECT_F g_image_rect;

int LoadMyImage(ID2D1RenderTarget* ap_target, const TCHAR* ap_path);

LRESULT CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        //PAINTSTRUCT ps;
        //HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        ValidateRect(hWnd, NULL);

        gp_render_target->BeginDraw();
        if (gp_bitmap != NULL) {
            gp_render_target->DrawBitmap(gp_bitmap, &g_image_rect);
        }
        else {
            gp_render_target->Clear(ColorF(0.0f, 0.8f, 1.0f));
        }
        gp_render_target->EndDraw();

        //EndPaint(hWnd, &ps);
    }
    return (LRESULT)TRUE;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDCANCEL:
            EndDialog(hWnd, LOWORD(wParam));
            break;
        }
    }
    return (LRESULT)TRUE;
    case WM_INITDIALOG:
        RECT r;
        GetClientRect(hWnd, &r);
        gp_factory->CreateHwndRenderTarget(RenderTargetProperties(), HwndRenderTargetProperties(hWnd, SizeU(r.right, r.bottom)), &gp_render_target);
        g_image_rect.right = (float)r.right;
        g_image_rect.bottom = (float)r.bottom;
        LoadMyImage(gp_render_target, _T("../img/sea.jpg"));
        return (LRESULT)TRUE;
    case WM_DESTROY:
        if (gp_render_target != NULL) {
            gp_render_target->Release();
            gp_render_target = NULL;
        }
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    }
    return (LRESULT)FALSE;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // D2D 초기화작업
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gp_factory);

    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DlgProc);

    // D2D 정리작업
    if (gp_bitmap != NULL) {
        gp_bitmap->Release();
    }
    gp_factory->Release();
    CoUninitialize();

    return 0;
}

int LoadMyImage(ID2D1RenderTarget* ap_target, const TCHAR* ap_path)
{
    if (gp_bitmap != NULL) {
        gp_bitmap->Release();
        gp_bitmap = NULL;
    }
    IWICImagingFactory* p_wic_factory;
    CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&p_wic_factory));
    IWICBitmapDecoder* p_decoder;
    IWICBitmapFrameDecode* p_frame;
    IWICFormatConverter* p_converter;
    int result = 0;         // 0 : 그림읽기 실패, 1 : 그림읽기 성공

    if (S_OK == p_wic_factory->CreateDecoderFromFilename(ap_path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &p_decoder)) {
        if (S_OK == p_decoder->GetFrame(0, &p_frame)) {
            if (S_OK == p_wic_factory->CreateFormatConverter(&p_converter)) {
                if (S_OK == p_converter->Initialize(p_frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom)) {
                    if (S_OK == ap_target->CreateBitmapFromWicBitmap(p_converter, NULL, &gp_bitmap)) {
                        result = 1;
                    }
                }
                p_converter->Release();
            }
            p_frame->Release();
        }
        p_decoder->Release();
    }
    p_wic_factory->Release();
    return result;
}