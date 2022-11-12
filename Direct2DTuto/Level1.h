#ifndef LEVEL_1_H_
#define LEVEL_1_H_

#include "GameLevel.h"
class Level1 : public GameLevel
{
private:
	float y;
	float y_speed;
public:
	void Load() override;
	void Unload() override;
	void Render(Graphics* gfx) override;
	void Update() override;
};

#endif