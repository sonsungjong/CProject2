#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <Ole2.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

int GetEncoderClsid(const TCHAR* format, CLSID* pClsid)
{
	UINT num = 0;
	UINT size = 0;

	ImageCodecInfo* pImage = NULL;
	GetImageEncodersSize(&num, &size);
	if (size == 0) return -1; 

	pImage = (ImageCodecInfo*)(malloc(size));
	if (pImage == NULL) return -1;

	GetImageEncoders(num, size, pImage);

	for (UINT j = 0; j < num; ++j)
	{
		if (_tcscmp(pImage[j].MimeType, format) == 0)
		{
			*pClsid = pImage[j].Clsid;
			free(pImage);
			return j;
		}
	}

	free(pImage);
	return -1;
}
// 화면 캡처하기
int mainC() {
	HDC hdcScreen = GetDC(NULL);
	HDC hdcMemDC = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmScreen = NULL;

	GdiplusStartupInput gdip;
	ULONG_PTR gdipToken;
	GdiplusStartup(&gdipToken, &gdip, NULL);

	// 이미지 크기
	hbmScreen = CreateCompatibleBitmap(hdcScreen, 700, 700);
	SelectObject(hdcMemDC, hbmScreen);

	// 캡처 영역
	BitBlt(hdcMemDC, 0, 0, 700, 700, hdcScreen, 0, 0, SRCCOPY);

	CLSID encoderID;

	GetEncoderClsid(_T("image/png"), &encoderID);			// image/jpeg
	Bitmap* bmp = new Bitmap(hbmScreen, (HPALETTE)0);
	bmp->Save(_T("D:\\STUFF\\IMAGEOFSCREEN123.png"), &encoderID, NULL);

	GdiplusShutdown(gdipToken);
	DeleteObject(hbmScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);

	_getch();
	return 0;
}