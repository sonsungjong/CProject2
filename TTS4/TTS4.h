#ifndef _TTS4_H_
#define _TTS4_H_

#include "resource.h"

#pragma warning(disable:4996)
#include <sapi.h>
#include <sphelper.h>

class TTSClass
{
public:
	TTSClass() 
	{
		initTTS();
	}
	~TTSClass()
	{
		delTTS();
	}
	int initTTS()
	{
		CoInitialize(NULL);
		m_hr = m_tts_audio.CoCreateInstance(CLSID_SpVoice);
		if (FAILED(m_hr)) {
			return -1;
		}
		return 0;
	}
	void delTTS()
	{
		m_tts_audio.Release();
		CoUninitialize();
	}
	void StartTTS(TCHAR* a_str)
	{
		m_tts_audio->Pause();
		m_tts_audio->Resume();
		m_tts_audio->Speak(a_str, SPF_ASYNC | SPF_IS_NOT_XML | SPF_PURGEBEFORESPEAK, 0);
	}
protected:
	CComPtr<ISpVoice> m_tts_audio;
	HRESULT m_hr;
};

#endif