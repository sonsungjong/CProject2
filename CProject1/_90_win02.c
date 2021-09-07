#include <stdio.h>
#include <Windows.h>

struct WNDCLASS
{
	void (*p_wnd_pro)(int, int, int);
};

// typedef void* HANDLE;

struct WNDCLASS g_wnd_class_data;

// 만들어질 어플리케이션 함수를 미리 대비 (함수포인터)
void WndProc(int id, int w_param, int l_param)
{
	HANDLE h;

	if (id == 1) {
		// 마우스 왼쪽 버튼 눌러졌을때 해야할 작업
	}
}

// app() 이 먼저 만들어짐, 개발자가 사용할 WndProc에 대비해서 함수포인터를 사용
app() {
	// 윈도우 움직이거나 최대화, 최소화
	//if (g_wnd_class_data.p_wnd_pro) g_wnd_class_data.p_wnd_pro(1, 2, 3);		// NULL이 아니면 1,2,3 넘김
	if (g_wnd_class_data.p_wnd_pro) WndProc(1, 2, 3);		// NULL이 아니면 1,2,3 넘김 (함수포인터)
}

int main902() {
	g_wnd_class_data.p_wnd_pro = WndProc;
	app();

	return 0;
}