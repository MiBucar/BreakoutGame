#pragma once
#include <SDL.h>
#include <iostream>
#include <Vector2D.h>

class Paddle
{
public:
	Paddle(const int screenWidth, const int screenHeight);
	void Spawn();
	void Move(float deltaTime);

	std::string GetTexture() const { return mTexture; };
	const SDL_Rect* GetRect() const { return &mRect; };
private:
	std::string mTexture;
	SDL_Rect mRect;

	const int mWidth = 80;
	const int mHeight = 20;
	const int mMoveSpeed = 200;

	Vector2D moveVel;
	Vector2D movePos;
};

