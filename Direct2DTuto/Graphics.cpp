#include "Graphics.h"

Graphics::Graphics()
{
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;
}

Graphics::~Graphics()
{
	if (factory != NULL) { factory->Release(); }
	if (renderTarget != NULL) { renderTarget->Release(); }
	if (brush != NULL) { brush->Release(); }
}

bool Graphics::Init(HWND hWnd)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) { return false; }

	RECT rect;
	GetClientRect(hWnd, &rect);

	res = factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)), &renderTarget);
	if (res != S_OK) { return false; }

	res = renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);
	if (res != S_OK) { return false; }

	return true;
}

void Graphics::ClearScreen(FLOAT r, FLOAT g, FLOAT b)
{
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::DrawCircle(FLOAT x, FLOAT y, FLOAT radius, FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush, 3.0f);
}