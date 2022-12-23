#pragma once
#include <SDL.h>
#include <iostream>

class Ball
{
public:
	Ball();
	void Move(const SDL_Rect* paddleRect, int dir);

	std::string GetTexture() const { return mTexture; };
	const SDL_Rect* GetRect() const { return &mRect; };

private:
	void ShootInDirection(int dir);

	SDL_Rect mRect;
	std::string mTexture;

	const int mWidth = 20;
	const int mHeight = 20;
	bool mLocked = true;

	int mDirection;
};

