#include <stdio.h>
#include <Windows.h>

struct WNDCLASS
{
	void (*p_wnd_pro)(int, int, int);
};

// typedef void* HANDLE;

struct WNDCLASS g_wnd_class_data;

// ������� ���ø����̼� �Լ��� �̸� ��� (�Լ�������)
void WndProc(int id, int w_param, int l_param)
{
	HANDLE h;

	if (id == 1) {
		// ���콺 ���� ��ư ���������� �ؾ��� �۾�
	}
}

// app() �� ���� �������, �����ڰ� ����� WndProc�� ����ؼ� �Լ������͸� ���
app() {
	// ������ �����̰ų� �ִ�ȭ, �ּ�ȭ
	//if (g_wnd_class_data.p_wnd_pro) g_wnd_class_data.p_wnd_pro(1, 2, 3);		// NULL�� �ƴϸ� 1,2,3 �ѱ�
	if (g_wnd_class_data.p_wnd_pro) WndProc(1, 2, 3);		// NULL�� �ƴϸ� 1,2,3 �ѱ� (�Լ�������)
}

int main902() {
	g_wnd_class_data.p_wnd_pro = WndProc;
	app();

	return 0;
}