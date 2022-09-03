#ifndef IMG_ROTATE_H_
#define IMG_ROTATE_H_

#include "resource.h"

class ImgRotate
{
protected:
	HWND mh_wnd;			// ���� ������ �ڵ�
	HINSTANCE mh_instance;		// WinMain ù��° ����
	INT m_exit_state;				// WinMain ���� ��ȯ�� ����
	TCHAR m_class_name[32];			// ������ Ŭ���� �̸� ����
public:
	ImgRotate(HINSTANCE ah_instance, const TCHAR* ap_class_name);			// ��ü ������
	virtual ~ImgRotate();								// ��ü �ı���
	INT GetExitState();						// WinMain ������ȯ��
	virtual void InitApplication();		// ������ Ŭ���� ���
	virtual int InitInstance();			// ���� ������ ����
	virtual void Run();			// �޽��� ó��

};

#endif