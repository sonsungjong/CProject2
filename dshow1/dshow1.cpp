#include <Windows.h>
#include <dshow.h>
#include <iostream>
#include <conio.h>
#include <tchar.h >

//#include <d3d9.h>
//#include <vmr9.h>

//#include <locale>
//#include <Ole2.h>
#pragma comment(lib, "Strmiids")				// DirectShow

int main1() {
	int DeviceNumber;

	HRESULT hr;

	ICaptureGraphBuilder2* pBuild;
	IGraphBuilder* pGraph;
	IBaseFilter* pCap;

	IMoniker* pMoniker;
	IEnumMoniker* pEnum;
	ICreateDevEnum* pDevEnum;

	IMediaControl* pControl;
	IVideoWindow* pWindow;

	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr)) {/*Handle Error*/ }

	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pBuild);
	if (FAILED(hr)) {/*Handle Error*/ }
	hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IFilterGraph, (void**)&pGraph);
	if (FAILED(hr)) {/*Handle Error*/ }
	pBuild->SetFiltergraph(pGraph);

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));
	if (FAILED(hr)) {/*Handle Error*/ }
	hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0);
	if (FAILED(hr)) {/*Handle Error*/ }

	std::cout << "Select Device :-" << std::endl;
	while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
	{
		IPropertyBag* pPropBag;
		hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
		if (FAILED(hr))
		{
			pMoniker->Release();
			continue;
		}

		VARIANT var;
		VariantInit(&var);

		hr = pPropBag->Read(TEXT("FriendlyName"), &var, 0);
		if (SUCCEEDED(hr))
		{
			std::wcout << (wchar_t*)var.bstrVal << std::endl;
			VariantClear(&var);
		}
		pPropBag->Release();
		pMoniker->Release();
	}
	pEnum->Reset();
	std::cin >> DeviceNumber;
	for (int i = 0; i < DeviceNumber; i++) {
		hr = pEnum->Next(1, &pMoniker, NULL);
		if(FAILED(hr)){}
	}
	hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pCap);
	if (FAILED(hr)) {}
	hr = pGraph->AddFilter(pCap, TEXT("Capture Filter"));
	if (FAILED(hr)) {}
	hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
	if (FAILED(hr)) {}

	
	return 0;
}