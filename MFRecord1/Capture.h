#ifndef CAPTURE_H_
#define CAPTURE_H_

#include <new>
#include <Windows.h>
#include <windowsx.h>
#include <mfapi.h>
#include <mfidl.h>
#include <Mferror.h>
#include <mfcaptureengine.h>
#include <Shlwapi.h>
#include <strsafe.h>
#include <CommCtrl.h>
#include <d3d11.h>

const UINT WM_APP_CAPTURE_EVENT = WM_APP + 1;

HWND CreatePreviewWindow(HINSTANCE hInstance, HWND hParent);
HWND CreateMainWindow(HINSTANCE hInstance);
void SetMenuItemText(HMENU hMenu, UINT uItem, _In_ PWSTR pszText);
void ShowError(HWND hwnd, PCWSTR szMessage, HRESULT hr);
void ShowError(HWND hwnd, UINT id, HRESULT hr);
HRESULT CloneVideoMediaType(IMFMediaType* pSrcMediaType, REFGUID guidSubType, IMFMediaType** ppNewMediaType);
HRESULT CreatePhotoMediaType(IMFMediaType* pSrcMediaType, IMFMediaType** ppPhotoMediaType);

extern IMFDXGIDeviceManager* g_pDXGIMan;
extern ID3D11Device* g_pDX11Device;
extern UINT g_ResetToken;

template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

/*
Media Foundation 컬렉션에서 인터페이스 포인터를 가져온다
*/
template<class IFACE>
HRESULT GetCollectionObject(IMFCollection* pCollection, DWORD index, IFACE** ppObject)
{
	IUnknown* pUnk;
	HRESULT hr = pCollection->GetElement(IID_PPV_ARGS(ppObject));
	if (SUCCEEDED(hr))
	{
		hr = pUnk->QueryInterface(IID_PPV_ARGS(ppObject));
		pUnk->Release();
	}
	return hr;
}

struct ChooseDeviceParam
{
	ChooseDeviceParam() : ppDevices(NULL), count(0)
	{
	}
	~ChooseDeviceParam()
	{
		for (DWORD i = 0; i < count; i++)
		{
			SafeRelease(&ppDevices[i]);
		}
		CoTaskMemFree(ppDevices);
	}

	IMFActivate** ppDevices;
	UINT32 count;
	UINT32 selection;
};

/*
캡처매니저 클래스
캡처 엔진을 래핑하고 이벤트 콜백을 구현한다
*/
class CaptureManager
{
	class CaptureEngineCB : public IMFCaptureEngineOnEventCallback
	{
		long m_cRef;
		HWND m_hwnd;

	public:
		CaptureEngineCB(HWND hwnd) : m_cRef(1), m_hwnd(hwnd), m_fSleeping(false), m_pManager(NULL) {}

		STDMETHODIMP QueryInterface(REFIID riid, void** ppv);
		STDMETHODIMP_ (ULONG) AddRef();
		STDMETHODIMP_ (ULONG) Release();

		// IMF 캡처엔진 OnEvent 콜백
		STDMETHODIMP OnEvent(_In_ IMFMediaEvent* pEvent);
		bool m_fSleeping;
		CaptureManager* m_pManager;
	};

	HWND m_hwndEvent;
	HWND m_hwndPreview;

	IMFCaptureEngine* m_pEngine;
	IMFCapturePreviewSink* m_pPreview;

	CaptureEngineCB* m_pCallback;

	bool m_bPreviewing;
	bool m_bRecording;
	bool m_bPhotoPending;

	UINT m_errorID;
	HANDLE m_hEvent;
	HANDLE m_hpwrRequest;
	bool m_fPowerRequestSet;

	CaptureManager(HWND hwnd) :
		m_hwndEvent(hwnd), m_hwndPreview(NULL), m_pEngine(NULL), m_pPreview(NULL),
		m_pCallback(NULL), m_bRecording(false), m_bPreviewing(false), m_bPhotoPending(false), m_errorID(0),
		m_hEvent(NULL), m_hpwrRequest(INVALID_HANDLE_VALUE), m_fPowerRequestSet(false)
	{
		REASON_CONTEXT pwrCtxt;

		pwrCtxt.Version = POWER_REQUEST_CONTEXT_VERSION;
		pwrCtxt.Flags = POWER_REQUEST_CONTEXT_SIMPLE_STRING;
		pwrCtxt.Reason.SimpleReasonString = const_cast<LPWSTR>(L"CaptureEngine is recording!");

		m_hpwrRequest = PowerCreateRequest(&pwrCtxt);
	}

	void SetErrorID(HRESULT hr, UINT id)
	{
		m_errorID = SUCCEEDED(hr) ? 0 : id;
	}

	// 캡처 엔진 이벤트 핸들러
	void OnCaptureEngineInitialized(HRESULT& hrStatus);
	void OnPreviewStarted(HRESULT& hrStatus);
	void OnPreviewStopped(HRESULT& hrStatus);
	void OnRecordStarted(HRESULT& hrStatus);
	void OnRecordStopped(HRESULT& hrStatus);
	void WaitForResult()
	{
		WaitForSingleObject(m_hEvent, INFINITE);
	}
public:
	~CaptureManager()
	{
		DestroyCaptureEngine();
	}

	static HRESULT CreateInstance(HWND hwndEvent, CaptureManager** ppEngine)
	{
		HRESULT hr = S_OK;
		*ppEngine = NULL;

		CaptureManager* pEngine = new (std::nothrow) CaptureManager(hwndEvent);
		if (pEngine == NULL)
		{
			hr = E_OUTOFMEMORY;
		}
		else {
			*ppEngine = pEngine;
			pEngine = NULL;
		}

		if (NULL != pEngine)
		{
			delete pEngine;
		}

		return hr;
	}

	HRESULT InitializeCaptureManager(HWND hwndPreview, IUnknown* pUnk);
	void DestroyCaptureEngine()
	{
		if (NULL != m_hEvent)
		{
			CloseHandle(m_hEvent);
			m_hEvent = NULL;
		}
		SafeRelease(&m_pPreview);
		SafeRelease(&m_pEngine);
		SafeRelease(&m_pCallback);

		if (g_pDXGIMan)
		{
			g_pDXGIMan->ResetDevice(g_pDX11Device, g_ResetToken);
		}
		SafeRelease(&g_pDX11Device);
		SafeRelease(&g_pDXGIMan);

		m_bPreviewing = false;
		m_bRecording = false;
		m_bPhotoPending = false;
		m_errorID = 0;
	}

	bool IsPreviewing() const { return m_bPreviewing; }
	bool IsRecording() const { return m_bRecording; }
	bool IsPhotoPending() const { return m_bPhotoPending; }
	UINT ErrorID() const { return m_errorID; }

	HRESULT OnCaptureEvent(WPARAM wParam, LPARAM lParam);
	HRESULT SetVideoDevice(IUnknown* pUnk);
	HRESULT StartPreview();
	HRESULT StopPreview();
	HRESULT StartRecord(PCWSTR pszDestinationFile);
	HRESULT StopRecord();
	HRESULT TakePhoto(PCWSTR pszFileName);

	void SleepState(bool fSleeping)
	{
		if (NULL != m_pCallback)
		{
			m_pCallback->m_fSleeping = fSleeping;
		}
	}

	HRESULT UpdateVideo()
	{
		if (m_pPreview)
		{
			return m_pPreview->UpdateVideo(NULL, NULL, NULL);
		}
		else {
			return S_OK;
		}
	}
};

#endif CAPTURE_H_