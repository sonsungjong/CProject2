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

// getDC : 그리기핸들을 가져옴

int main921() {
	//HWND window_handle;

	//HDC h_dc = GetDC(window_handle);	// DC 가져옴
	//Rectangle(h_dc, 10, 10, 100, 100);

	//ReleaseDC(window_handle, h_dc);		// 가져온 GetDC를 반납

	return 0;
}

// x = 1920개(0~1919), y = 1080개의 점(0~1079)
// 윈도우 좌표 : 화면을 기준으로 0,0
// 클라이언트 좌표 : 내 창을 기준으로 0,0

// 비트맵 : 비트의 지도, 메모리 비트 패턴
// 가상 그래픽 시스템 : 그래픽 카드가 많기때문에 색상을 출력가능한 범위가 다양해서 윈도우에서 32bit라고 가정해서 프로그래밍하게끔 설계(A,R,G,B)
// Direct2D : 인터페이스를 그리기위한 거대한 라이브러리, GDI+를 보완해서 나온 그래픽 포맷

// Rectangle(x,y,x,y,Pen,Brush);
// Pen : 선색상, 선굵기 등
// Brush : 색채우기, 효과채우기 등