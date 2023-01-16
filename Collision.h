#pragma once
#include <SDL.h>
#include <iostream>
#include <Objects.h>

class Collision
{
public:
	static int Wall(const SDL_Rect* object, const int screenWidth, const int screenHeight, int topWallHeight);
	static bool HasCollided(const SDL_Rect* rectOne, const SDL_Rect* rectTwo);
	static int BallSurface(const SDL_Rect* rectOne, const SDL_Rect* rectTwo);
};

