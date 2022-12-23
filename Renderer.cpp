#include "Renderer.h"

Renderer::Renderer(Paddle* paddle, Ball* ball, const int screenWidth, const int screenHeight) : mWindow(nullptr), mRenderer(nullptr),
mPaddle(paddle), mBall(ball), mScreenWidth(screenWidth), mScreenHeight(screenHeight)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "Couldnt initialize the renderer" << SDL_GetError() << "\n";
	}
	else {
		mWindow = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mScreenWidth, mScreenHeight, 0);
		if (!mWindow) {
			std::cout << "Failed to create the window: " << SDL_GetError() << "\n";
		}
		mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
		if (!mRenderer) {
			std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
		}
	}

	mLevel = new Level();
	mLevel->LoadLevel(mRenderer);
	mTextures[PADDLETEXTURE] = IMG_LoadTexture(mRenderer, mPaddle->GetTexture().c_str());
	mTextures[BALLTEXTURE] = IMG_LoadTexture(mRenderer, mBall->GetTexture().c_str());
}

Renderer::~Renderer()
{
	SDL_DestroyWindow(mWindow); mWindow = nullptr;
	SDL_DestroyRenderer(mRenderer); mRenderer = nullptr;
	delete mLevel; mLevel = nullptr;

	for (int i = 0; i < TEXTUREEMPTY; i++) {
		SDL_DestroyTexture(mTextures[i]); mTextures[i] = nullptr;
	}
}

void Renderer::RenderLevel()
{
	RenderBackground();
	RenderBricks();
	RenderPaddle();
	RenderBall();
}

void Renderer::RenderBricks()
{
	SDL_Rect src = { 0, 0, 80, 20 };

	for (auto brick : mLevel->GetBricks()) {
		if (brick.GetHp() > 0) {
			SDL_RenderCopy(mRenderer, brick.GetTexture(), &src, brick.GetRect());
		}
	}
}

void Renderer::RenderPaddle()
{
	SDL_Rect src = { 0, 0, 80, 20 };
	SDL_RenderCopy(mRenderer, mTextures[PADDLETEXTURE], &src, mPaddle->GetRect());
}

void Renderer::RenderBall()
{
	SDL_Rect src = { 0, 0, 20, 20 };
	SDL_RenderCopy(mRenderer, mTextures[BALLTEXTURE], &src, mBall->GetRect());
}

void Renderer::RenderBackground()
{
	SDL_Rect src = { 0, 0, mScreenWidth, mScreenHeight };
	SDL_RenderCopy(mRenderer, mLevel->GetBackgroundTexture(), &src, &src);
}
