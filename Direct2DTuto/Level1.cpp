#include "GameController.h"
#include "Graphics.h"
#include "Level1.h"

void Level1::Load()
{
	y = y_speed = 0.0f;
}

void Level1::Unload()
{

}

void Level1::Update()
{
    y_speed += 1.0f;
    y += y_speed;
    if (y > 600)
    {
        y = 600;
        y_speed = -30.0f;
    }
}

void Level1::Render(Graphics* gfx)
{
    gfx->ClearScreen(0.0f, 0.0f, 0.5f);
    gfx->DrawCircle(375.0f, y, 50.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}