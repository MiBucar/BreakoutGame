#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <Objects.h>

struct Vector2D {
	float x;
	float y;
};

class Paddle
{
public:
	Paddle(const int screenWidth, const int screenHeight);
	void Move(float deltaTime);
	void Reset();

	int GetDirection() const { return mDirectionX; };
	float GetMovePosX() const { return mMovePosX; };
	const SDL_Rect* GetRect() const { return &mRect; };

	void BlockPaddle(int side);

	int GetY() const { return mRect.y; };
private:
	SDL_Rect mRect;

	const int mWidth = 80;
	const int mHeight = 20;
	const int mScreenHeight;
	const int mScreenWidth;

	float mDirectionX;
	const float mVelX = 500;

	float mMovePosX;
};

