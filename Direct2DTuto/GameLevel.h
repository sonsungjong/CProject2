#ifndef GAME_LEVEL_H_
#define GAME_LEVEL_H_

#include "Graphics.h"

class GameLevel
{
private:

public:
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Render(Graphics* gfx) = 0;
	virtual void Update() = 0;
};

#endif