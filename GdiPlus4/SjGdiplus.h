#ifndef _SJ_GDIPLUS_H_
#define _SJ_GDIPLUS_H_

// #define WIN32_LEAN_AND_MEAN를 제거하기

class SjGdiplus
{
public:
	SjGdiplus(HWND hWnd)
	{
		GdiplusStartup(&m_gpToken, &m_gpsi, NULL);
		m_hdc = GetDC(hWnd);
		mp_graphics = new Graphics(m_hdc);
		GetClientRect(hWnd, &m_rect);
		mp_pen = new Pen(Color(255, 0, 0, 0), 1.0f);
		
	}
	~SjGdiplus()
	{
		GdiplusShutdown(m_gpToken);
		delete mp_pen;
		delete mp_graphics;
	}
	void ColorPen(int r, int g, int b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		delete mp_pen;
		mp_pen = new Pen(Color(255, r, g, b), thickness);
	}
	void ThickPen(float thickness)
	{
		this->thickness = thickness;
		delete mp_pen;
		mp_pen = new Pen(Color(255, r, g, b), thickness);
	}

protected:
	ULONG_PTR m_gpToken;
	GdiplusStartupInput m_gpsi;
	HDC m_hdc;
	Graphics* mp_graphics;
	Pen* mp_pen;
	SolidBrush* mp_brush;
	FontFamily* mp_fondFamily;
	PointF* pointF;
	RECT m_rect;
	int r = 0, g = 0, b = 0;
	float thickness = 1.0f;
};

#endif