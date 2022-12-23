#pragma once
#include <SDL.h>

class Collision
{
public:
	static bool PaddleRightWall(const SDL_Rect* paddleRect, const int screenWidth);
	static bool PaddleLeftWall(const SDL_Rect* paddleRect, const int screenWidth);
};

