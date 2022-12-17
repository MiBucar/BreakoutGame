#pragma once
#include <SDL.h>
#include <iostream>
#include <Level.h>
#include <Paddle.h>
#include <vector>
#include <Objects.h>

class Renderer
{
public:
	Renderer(Paddle* paddle, const int screenWidth, const int screenHeight);
	~Renderer();
	void RenderLevel();
	SDL_Renderer* GetRenderer() const { return mRenderer; };

	const int GetScreenWidth() const { return mScreenWidth; };
	const int GetScreenHeight() const { return mScreenHeight; };
private:
	
	void RenderBricks();
	void RenderPaddle();
	SDL_Renderer* mRenderer;
	SDL_Window* mWindow;
	SDL_Texture* mTextures[EMPTYTEXTURE];

	const int mScreenWidth;
	const int mScreenHeight;
	
	Level *mLevel;
	Paddle* mPaddle;
};

