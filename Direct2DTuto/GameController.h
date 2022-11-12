#ifndef GAME_CONTROLLER_H_
#define GAME_CONTROLLER_H_

#include "GameLevel.h"
#include "Graphics.h"

class GameController
{
private:
	GameController() {}
	static GameLevel* currentLevel;				// ╫л╠шео фпео
public:
	static bool Loading;

	static void Init();

	static void LoadInitialLevel(GameLevel* lev);
	static void SwitchLevel(GameLevel* lev);

	static void Render(Graphics* gfx);
	static void Update();
};

#endif