#pragma once
#include <SDL.h>
#include <iostream>
#include <Brick.h>
#include <Objects.h>

class Ball
{
public:
	Ball();
	void Move(const SDL_Rect* paddleRect, float deltaTime, int dir);
	void Reset();
	void BlockBall(int side);
	void Bounce(int side);
	void IncreaseSpeed();
	void RestartSpeed();

	const SDL_Rect* GetRect() const { return &mRect; };
private:
	SDL_Rect mRect;

	const int mWidth = 20;
	const int mHeight = 20;
	bool mLocked = true;

	float mDirectionX;
	float mDirectionY;
	float mMovePosX;
	float mMovePosY;
	float mVel = 300;
};

