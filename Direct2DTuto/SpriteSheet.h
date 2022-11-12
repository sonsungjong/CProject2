#ifndef SPRITE_SHEET_H_
#define SPRITE_SHEET_H_

#include <wincodec.h>				// WIC
#include "Graphics.h"
// Link to "windowscodecs.lib

class SpriteSheet
{
	Graphics* gfx;
	ID2D1Bitmap* bmp;
public:
	SpriteSheet(const TCHAR* filename, Graphics* gfx);

	~SpriteSheet();

	void Draw();
};

#endif