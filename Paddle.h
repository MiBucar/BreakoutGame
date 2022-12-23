#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

struct Vector2D {
	float x;
	float y;
};

class Paddle
{
public:
	Paddle(const int screenWidth, const int screenHeight);
	void Move(float deltaTime);

	std::string GetTexture() const { return mTexture; };
	float GetDirection() const { return mDirectionX; };
	const SDL_Rect* GetRect() const { return &mRect; };

	void BlockPaddle(int side);

private:
	std::string mTexture;
	SDL_Rect mRect;

	const int mWidth = 80;
	const int mHeight = 20;
	const int mScreenHeight;
	const int mScreenWidth;

	float mDirectionX;
	const float mVelX = 500;

	Vector2D mMovePos;
};

