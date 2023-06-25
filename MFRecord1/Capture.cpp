#include "resource.h"
#include "Capture.h"

IMFDXGIDeviceManager* g_pDXGIMan = NULL;
ID3D11Device* g_pDX11Device = NULL;
UINT g_ResetToken = 0;

/*
COM 객체가 어떤 인터페이스를 지원하는지 확인하고 해당 인터페이스의 포인터를 반환
IMFCaptureEngineOnEventCallback 를 구현하고 있고, 지원 가능하면 인터페이스의 포인터를 반환
*/
STDMETHODIMP CaptureManager::CaptureEngineCB::QueryInterface(REFIID riid, void** ppv)
{
	static const QITAB qit[] = { QITABENT(CaptureEngineCB, IMFCaptureEngineOnEventCallback), {0} };

	return QISearch(this, qit, riid, ppv);
}

/*
COM(Component Object Model) 객체의 참조 카운트를 증가시키는 AddRef
COM 객체는 참조 횟수를 세어 관리하며 이 참조 횟수를 증가시킴
*/
STDMETHODIMP_ (ULONG) CaptureManager::CaptureEngineCB::AddRef()
{
	return InterlockedIncrement(&m_cRef);				// 멀티스레드 환경에서 안전하게 참조 카운트를 증가시키고, 그 후 반환
}


/*
COM 객체의 참조 카운트를 감소시키는 Release
COM의 참조 횟수를 감소시키고 0이 되면 객체를 메모리에서 해제
*/
STDMETHODIMP_ (ULONG) CaptureManager::CaptureEngineCB::Release()
{
	LONG cRef = InterlockedDecrement(&m_cRef);				// 멀티스레드 환경에서 안전하게 카운트 감소
	if (cRef == 0)
	{
		delete this;
	}
	return cRef;				// 참조 카운트 수치 반환
}

// 캡처 엔진에서 이벤트를 수신하는 콜백 메서드
STDMETHODIMP CaptureManager::CaptureEngineCB::OnEvent(_In_ IMFMediaEvent* pEvent)
{
	// 애플리케이션 메인쓰레드에서 애플리케이션 창에 메시지를 게시하여 이벤트를 처리한다
	if (m_fSleeping && m_pManager != NULL)
	{
		// 잠이 들기 직전, CE에게 미리보기 중지를 요청하고 기록한다. (메시지 펌프가 없어졌을 수도 있어서 여기서 처리해야함)
		GUID guidType;
		HRESULT hrStatus;
		HRESULT hr = pEvent->GetStatus(&hrStatus);
		if (FAILED(hr)) {
			hrStatus = hr;
		}

		hr = pEvent->GetExtendedType(&guidType);
		if (SUCCEEDED(hr)) {
			if (guidType == MF_CAPTURE_ENGINE_PREVIEW_STOPPED) {
				// 프리뷰 중지
				m_pManager->OnPreviewStopped(hrStatus);
				SetEvent(m_pManager->m_hEvent);
			}
			else if (guidType == MF_CAPTURE_ENGINE_RECORD_STOPPED) {
				// 녹화 중지
				m_pManager->OnRecordStopped(hrStatus);
				SetEvent(m_pManager->m_hEvent);
			}
			else {
				// 알려지지않은 오류 발생으로 이벤트만 설정하고 넘어감
				SetEvent(m_pManager->m_hEvent);
			}
		}

		return S_OK;
	}
	else {
		pEvent->AddRef();				// 메시지를 처리할 때 포인터를 해제
		PostMessage(m_hwnd, WM_APP_CAPTURE_EVENT, (WPARAM)pEvent, 0L);
	}

	return S_OK;
}

/*

*/
HRESULT CreateDX11Device(_Out_ ID3D11Device** ppDevice, _Out_ ID3D11DeviceContext** ppDeviceContext, _Out_ D3D_FEATURE_LEVEL* pFeatureLevel)
{
	HRESULT hr = S_OK;
	static const D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_VIDEO_SUPPORT, levels, ARRAYSIZE(levels),
		D3D11_SDK_VERSION, ppDevice, pFeatureLevel, ppDeviceContext);

	if (SUCCEEDED(hr)) {
		ID3D10Multithread* pMultithread;
		hr = ((*ppDevice)->QueryInterface(IID_PPV_ARGS(&pMultithread)));

		if (SUCCEEDED(hr))
		{
			pMultithread->SetMultithreadProtected(TRUE);
		}

		SafeRelease(&pMultithread);
	}

	return hr;
}

HRESULT CreateD3DManager()
{
	HRESULT hr = S_OK;
	D3D_FEATURE_LEVEL FeatureLevel;
	ID3D11DeviceContext* pDX11DeviceContext;

	hr = CreateDX11Device(&g_pDX11Device, &pDX11DeviceContext, &FeatureLevel);

	if (SUCCEEDED(hr)) {
		hr = MFCreateDXGIDeviceManager(&g_ResetToken, &g_pDXGIMan);
	}

	if (SUCCEEDED(hr)) {
		hr = g_pDXGIMan->ResetDevice(g_pDX11Device, g_ResetToken);
	}

	SafeRelease(&pDX11DeviceContext);

	return hr;
}

HRESULT CaptureManager::InitializeCaptureManager(HWND hwndPreview, IUnknown* pUnk)
{
	HRESULT hr = S_OK;
	IMFAttributes* pAttributes = NULL;
	IMFCaptureEngineClassFactory* pFactory = NULL;

	DestroyCaptureEngine();

	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (NULL != m_hEvent)
	{
		m_pCallback = new (std::nothrow) CaptureEngineCB(m_hwndEvent);
		if (m_pCallback != NULL) {
			m_pCallback->m_pManager = this;
			m_hwndPreview = hwndPreview;

			// Create a D3D Manager
			hr = CreateD3DManager();
			if (SUCCEEDED(hr)) {
				hr = MFCreateAttributes(&pAttributes, 1);
				if (SUCCEEDED(hr)) {
					hr = pAttributes->SetUnknown(MF_CAPTURE_ENGINE_D3D_MANAGER, g_pDXGIMan);
					if (SUCCEEDED(hr)) {
						// 캡처 엔진을 위한 팩토리 객체 생성
						hr = CoCreateInstance(CLSID_MFCaptureEngineClassFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));
						if (SUCCEEDED(hr)) {
							hr = pFactory->CreateInstance(CLSID_MFCaptureEngine, IID_PPV_ARGS(&m_pEngine));
							if (SUCCEEDED(hr)) {
								hr = m_pEngine->Initialize(m_pCallback, pAttributes, NULL, pUnk);
							}
						}
					}
				}
			}
		}
		else {
			// m_pCallback == NULL
			hr = E_OUTOFMEMORY;
		}
	}
	else {
		// m_hEvent == NULL
		hr = HRESULT_FROM_WIN32(GetLastError());
	}

	if (NULL != pAttributes)
	{
		pAttributes->Release();
		pAttributes = NULL;
	}
	if (NULL != pFactory)
	{
		pFactory->Release();
		pFactory = NULL;
	}
	return hr;
}

/*
캡처 엔진의 이벤트를 처리 (애플리케이션의 UI 스레드에서 호출됨)
*/
HRESULT CaptureManager::OnCaptureEvent(WPARAM wParam, LPARAM lParam)
{
	GUID guidType;
	HRESULT hrStatus;

	IMFMediaEvent* pEvent = reinterpret_cast<IMFMediaEvent*>(wParam);
	
	HRESULT hr = pEvent->GetStatus(&hrStatus);
	if (FAILED(hr)) {
		hrStatus = hr;
	}
	hr = pEvent->GetExtendedType(&guidType);
	if (SUCCEEDED(hr)) {
		if (guidType == MF_CAPTURE_ENGINE_INITIALIZED) {
			OnCaptureEngineInitialized(hrStatus);
			SetErrorID(hrStatus, IDS_ERR_INITIALIZE);
		}
		else if (guidType == MF_CAPTURE_ENGINE_PREVIEW_STARTED) {
			OnPreviewStarted(hrStatus);
			SetErrorID(hrStatus, IDS_ERR_PREVIEW);
		}
		else if (guidType == MF_CAPTURE_ENGINE_PREVIEW_STOPPED) {
			OnPreviewStopped(hrStatus);
			SetErrorID(hrStatus, IDS_ERR_PREVIEW);
		}
		else if (guidType == MF_CAPTURE_ENGINE_RECORD_STARTED) {
			OnRecordStarted(hrStatus);
			SetErrorID(hrStatus, IDS_ERR_RECORD);
		}
		else if (guidType == MF_CAPTURE_ENGINE_RECORD_STOPPED) {
			OnRecordStopped(hrStatus);
			SetErrorID(hrStatus, IDS_ERR_RECORD);
		}
		else if (guidType == MF_CAPTURE_ENGINE_PHOTO_TAKEN) {
			m_bPhotoPending = false;
			SetErrorID(hrStatus, IDS_ERR_PHOTO);
		}
		else if (guidType == MF_CAPTURE_ENGINE_ERROR) {
			DestroyCaptureEngine();
			SetErrorID(hrStatus, IDS_ERR_CAPTURE);
		}
		else if (FAILED(hrStatus)) {
			SetErrorID(hrStatus, IDS_ERR_CAPTURE);
		}
	}

	pEvent->Release();
	SetEvent(m_hEvent);
	return hrStatus;
}

void CaptureManager::OnCaptureEngineInitialized(HRESULT& hrStatus)
{
	if (hrStatus == MF_E_NO_CAPTURE_DEVICES_AVAILABLE) {
		hrStatus = S_OK;				// 캡처 장치가 없음. 프로그램 오류가 아님
	}
}

void CaptureManager::OnPreviewStarted(HRESULT& hrStatus)
{
	m_bPreviewing = SUCCEEDED(hrStatus);
}

void CaptureManager::OnPreviewStopped(HRESULT& hrStatus)
{
	m_bPreviewing = false;
}

void CaptureManager::OnRecordStarted(HRESULT& hrStatus)
{
	m_bRecording = SUCCEEDED(hrStatus);
}

void CaptureManager::OnRecordStopped(HRESULT& hrStatus)
{
	m_bRecording = false;
}

HRESULT CaptureManager::StartPreview()
{
	if (m_pEngine == NULL) {
		return MF_E_NOT_INITIALIZED;
	}

	if (m_bPreviewing == true) {
		return S_OK;
	}

	IMFCaptureSink* pSink = NULL;
	IMFMediaType* pMediaType = NULL;
	IMFMediaType* pMediaType2 = NULL;
	IMFCaptureSource* pSource = NULL;

	HRESULT hr = S_OK;

	// 미리보기 싱크 포인터를 가져옴
	if (m_pPreview == NULL) {
		hr = m_pEngine->GetSink(MF_CAPTURE_ENGINE_SINK_TYPE_PREVIEW, &pSink);
		if (SUCCEEDED(hr)) {
			hr = pSink->QueryInterface(IID_PPV_ARGS(&m_pPreview));
			if (SUCCEEDED(hr)) {
				hr = m_pPreview->SetRenderHandle(m_hwndPreview);
				if (SUCCEEDED(hr)) {
					hr = m_pEngine->GetSource(&pSource);
					if (SUCCEEDED(hr)) {
						hr = pSource->GetCurrentDeviceMediaType((DWORD)MF_CAPTURE_ENGINE_PREFERRED_SOURCE_STREAM_FOR_VIDEO_PREVIEW, &pMediaType);
						if (SUCCEEDED(hr)) {
							hr = CloneVideoMediaType(pMediaType, MFVideoFormat_RGB32, &pMediaType2);
							if (SUCCEEDED(hr)) {
								hr = pMediaType2->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);
								if (SUCCEEDED(hr)) {
									DWORD dwSinkStreamIndex;
									hr = m_pPreview->AddStream((DWORD)MF_CAPTURE_ENGINE_PREFERRED_SOURCE_STREAM_FOR_VIDEO_PREVIEW,
										pMediaType2, NULL, &dwSinkStreamIndex);
								}
							}
						}
					}
				}
			}
		}
	}

	if (SUCCEEDED(hr)) {
		hr = m_pEngine->StartPreview();
		if (!m_fPowerRequestSet && m_hpwrRequest != INVALID_HANDLE_VALUE) {
			// 장치를 저전력 상태로 요청
			m_fPowerRequestSet = (TRUE == PowerSetRequest(m_hpwrRequest, PowerRequestExecutionRequired));
		}
	}

	SafeRelease(&pSink);
	SafeRelease(&pMediaType);
	SafeRelease(&pMediaType2);
	SafeRelease(&pSource);

	return hr;
}

HRESULT CaptureManager::StopPreview()
{
	HRESULT hr = S_OK;

	if (m_pEngine == NULL) {
		return MF_E_NOT_INITIALIZED;
	}

	if (!m_bPreviewing) {
		return S_OK;
	}

	hr = m_pEngine->StopPreview();
	if (SUCCEEDED(hr)) {
		WaitForResult();

		if (m_fPowerRequestSet && m_hpwrRequest != INVALID_HANDLE_VALUE) {
			PowerClearRequest(m_hpwrRequest, PowerRequestExecutionRequired);
			m_fPowerRequestSet = false;
		}
	}

	return hr;
}

/*
비디오 미디어 유형에서 프레임 크기를 가져옴
*/
inline HRESULT GetFrameSize(IMFMediaType* pType, UINT32* pWidth, UINT32* pHeight)
{
	return MFGetAttributeSize(pType, MF_MT_FRAME_SIZE, pWidth, pHeight);
}

/*
비디오 미디어 유형에서 프레임 속도를 가져옴
*/
inline HRESULT GetFrameRate(IMFMediaType* pType, UINT32* pNumerator, UINT32* pDenominator)
{
	return MFGetAttributeRatio(pType, MF_MT_FRAME_RATE, pNumerator, pDenominator);
}

/*
인코딩 비트 전송률 가져오기
*/
HRESULT GetEncodingBitrate(IMFMediaType* pMediaType, UINT32* uiEncodingBitrate)
{
	UINT32 uiWidth;
	UINT32 uiHeight;
	float uiBitrate;
	UINT32 uiFrameRateNum;
	UINT32 uiFrameRateDenom;

	HRESULT hr = GetFrameSize(pMediaType, &uiWidth, &uiHeight);
	if (SUCCEEDED(hr)) {
		hr = GetFrameRate(pMediaType, &uiFrameRateNum, &uiFrameRateDenom);
		if (SUCCEEDED(hr)) {
			uiBitrate = uiWidth / 3.0f * uiHeight * uiFrameRateNum / uiFrameRateDenom;
			*uiEncodingBitrate = (UINT32)uiBitrate;
		}
	}

	return hr;
}

HRESULT ConfigureVideoEncoding(IMFCaptureSource* pSource, IMFCaptureRecordSink* pRecord, REFGUID guidEncodingType)
{
	IMFMediaType* pMediaType = NULL;
	IMFMediaType* pMediaType2 = NULL;
	GUID guidSubType = GUID_NULL;

	// 녹화 싱크에 대한 비디오 형식 구성
	HRESULT hr = pSource->GetCurrentDeviceMediaType((DWORD)MF_CAPTURE_ENGINE_PREFERRED_SOURCE_STREAM_FOR_VIDEO_RECORD, &pMediaType);
	if (SUCCEEDED(hr)) {
		hr = CloneVideoMediaType(pMediaType, guidEncodingType, &pMediaType2);
		if (SUCCEEDED(hr)) {
			hr = pMediaType->GetGUID(MF_MT_SUBTYPE, &guidSubType);
			if (SUCCEEDED(hr)) {
				if (guidSubType == MFVideoFormat_H264_ES || guidSubType == MFVideoFormat_H264) {
					// 웹캠이 H264_ES 또는 H264를 지원하는 경우 스트림을 우회한다
					// 캡처 엔진 출력은 웹캠에서 지원하는 기본 유형과 동일해야함
					hr = pMediaType2->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
				}
				else {
					UINT32 uiEncodingBitrate;
					hr = GetEncodingBitrate(pMediaType2, &uiEncodingBitrate);
					if (SUCCEEDED(hr)) {
						hr = pMediaType2->SetUINT32(MF_MT_AVG_BITRATE, uiEncodingBitrate);
					}
				}

				if (SUCCEEDED(hr)) {
					// 녹화 싱크에 비디오 스트림을 연결한다
					DWORD dwSinkStreamIndex;
					hr = pRecord->AddStream((DWORD)MF_CAPTURE_ENGINE_PREFERRED_SOURCE_STREAM_FOR_VIDEO_RECORD, pMediaType2, NULL, &dwSinkStreamIndex);
				}
			}
		}
	}

	SafeRelease(&pMediaType);
	SafeRelease(&pMediaType2);
	return hr;
}

HRESULT ConfigureAudioEncoding(IMFCaptureSource* pSource, IMFCaptureRecordSink* pRecord, REFGUID guidEncodingType)
{
	IMFCollection* pAvailableTypes = NULL;
	IMFMediaType* pMediaType = NULL;
	IMFAttributes* pAttributes = NULL;


}