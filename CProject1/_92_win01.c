#include <stdio.h>
#include <Windows.h>

struct Pen
{
	int style;
	int color;
};

struct Brush
{
	int color;
	int pattern;
};

struct DC			// Device Context
{
	struct Pen pen;
	struct Brush brush;
};

// getDC : �׸����ڵ��� ������

int main921() {
	//HWND window_handle;

	//HDC h_dc = GetDC(window_handle);	// DC ������
	//Rectangle(h_dc, 10, 10, 100, 100);

	//ReleaseDC(window_handle, h_dc);		// ������ GetDC�� �ݳ�

	return 0;
}

// x = 1920��(0~1919), y = 1080���� ��(0~1079)
// ������ ��ǥ : ȭ���� �������� 0,0
// Ŭ���̾�Ʈ ��ǥ : �� â�� �������� 0,0

// ��Ʈ�� : ��Ʈ�� ����, �޸� ��Ʈ ����
// ���� �׷��� �ý��� : �׷��� ī�尡 ���⶧���� ������ ��°����� ������ �پ��ؼ� �����쿡�� 32bit��� �����ؼ� ���α׷����ϰԲ� ����(A,R,G,B)
// Direct2D : �������̽��� �׸������� �Ŵ��� ���̺귯��, GDI+�� �����ؼ� ���� �׷��� ����

// Rectangle(x,y,x,y,Pen,Brush);
// Pen : ������, ������ ��
// Brush : ��ä���, ȿ��ä��� ��