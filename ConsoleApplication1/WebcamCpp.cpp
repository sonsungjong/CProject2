#include <Windows.h>
#include <iostream>
#include <tchar.h>

#include <dshow.h>					// DirectShow
//#include <d3d9.h>
//#include <vmr9.h>

//#include <locale>
//#include <Ole2.h>
#include <conio.h>
#pragma comment(lib, "Strmiids")				// DirectShow

int main() {
	int DeviceNumber;

	HRESULT hr;

	ICaptureGraphBuilder2 *pBuild;
	IGraphBuilder *pGraph;
	IBaseFilter *pCap;
	
	IMoniker* pMoniker;
	IEnumMoniker *pEnum;
	ICreateDevEnum *pDevEnum;

	IMediaControl *pControl;
	IVideoWindow *pWindow;

	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr)){/*Handle Error*/ }

	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pBuild);
	if(FAILED(hr)){/*Handle Error*/ }
	// ¿€º∫¡ﬂ...

	return 0;
}