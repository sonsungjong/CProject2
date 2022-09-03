#ifndef IMG_ROTATE_H_
#define IMG_ROTATE_H_

#include "resource.h"

class ImgRotate
{
protected:
	HWND mh_wnd;			// 메인 윈도우 핸들
	HINSTANCE mh_instance;		// WinMain 첫번째 인자
	INT m_exit_state;				// WinMain 최종 반환값 저장
	TCHAR m_class_name[32];			// 윈도우 클래스 이름 저장
public:
	ImgRotate(HINSTANCE ah_instance, const TCHAR* ap_class_name);			// 객체 생성자
	virtual ~ImgRotate();								// 객체 파괴자
	INT GetExitState();						// WinMain 최종반환값
	virtual void InitApplication();		// 윈도우 클래스 등록
	virtual int InitInstance();			// 메인 윈도우 생성
	virtual void Run();			// 메시지 처리

};

#endif