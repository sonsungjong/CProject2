#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <d2d1.h>

class Graphics
{
private:
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;
	ID2D1SolidColorBrush* brush;

public:
	explicit Graphics();
	virtual ~Graphics();

	bool Init(HWND hWnd);

	ID2D1RenderTarget* GetRenderTarget()
	{
		return renderTarget;
	}

	void BeginDraw() { renderTarget->BeginDraw(); }
	void EndDraw() { renderTarget->EndDraw(); }

	void ClearScreen(FLOAT r, FLOAT g, FLOAT b);
	void DrawCircle(FLOAT x, FLOAT y, FLOAT radius, FLOAT r, FLOAT g, FLOAT b, FLOAT a);

};

#endif