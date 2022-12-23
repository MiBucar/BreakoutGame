#pragma once
#include <SDL.h>
#include <iostream>
#include <Level.h>
#include <Paddle.h>
#include <Ball.h>
#include <vector>
#include <Objects.h>
using namespace obj;

class Renderer
{
public:
	Renderer(Paddle* paddle, Ball* ball, const int screenWidth, const int screenHeight);
	~Renderer();
	void RenderLevel();
	SDL_Renderer* GetRenderer() const { return mRenderer; };

	const int GetScreenWidth() const { return mScreenWidth; };
	const int GetScreenHeight() const { return mScreenHeight; };
private:
	
	void RenderBricks();
	void RenderPaddle();
	void RenderBall();
	void RenderBackground();
	SDL_Renderer* mRenderer;
	SDL_Window* mWindow;
	SDL_Texture* mTextures[TEXTUREEMPTY];

	const int mScreenWidth;
	const int mScreenHeight;
	
	Level *mLevel;
	Paddle* mPaddle;
	Ball* mBall;
};

