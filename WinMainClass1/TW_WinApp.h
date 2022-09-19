#ifndef TWAPI_W01_H_
#define TWAPI_W01_H_

#include <d2d1.h>

using namespace D2D1;
extern ID2D1Factory *gp_factory;

class TW_WinApp
{
protected:
	HWND mh_wnd;
	HINSTANCE mh_instance;
	TCHAR m_class_name[32];
	int m_exit_state;
public:
	TW_WinApp(HINSTANCE ah_instance, const TCHAR* ap_class_name);
	virtual ~TW_WinApp();

	int GetExitState() { return m_exit_state; }
	HWND GetMainWindowHandle() { return mh_wnd; }
	HINSTANCE GetInstanceHandle() { return mh_instance; }
	virtual void InitApplication();
	virtual int InitInstance();
	virtual void Run();
};

#endif