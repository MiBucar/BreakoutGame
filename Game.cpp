#include "Game.h"

Game::Game() : mIsRunning(true), mPaddle(new Paddle(mScreenWidth, mScreenHeight)),
mRenderer(mPaddle, mScreenWidth, mScreenHeight)
{
	mLastTick = SDL_GetTicks();
	mFpsTicks = mLastTick;
	mFps = 0;
	mFrameCount = 0;
}

Game::~Game()
{
	delete mPaddle; mPaddle = nullptr;
	SDL_Quit();
	IMG_Quit();
}

int Game::Run()
{
	while (mIsRunning) {
		PollEvents();
		GameEvents();
	}
	
	return 0;
}

void Game::PollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			break;
		}
	}
}

void Game::GameEvents()
{
	SDL_RenderClear(mRenderer.GetRenderer());

	Gameplay();
	Rendering();

	SDL_RenderPresent(mRenderer.GetRenderer());
}

void Game::Gameplay()
{
	unsigned int curtick = SDL_GetTicks();
	float deltaTime = (curtick - mLastTick) / 1000.0f;
	if (curtick - mFpsTicks >= 0) {
		mFps = mFrameCount * (1000.0f / (curtick - mFpsTicks));
		mFpsTicks = curtick;
		mFrameCount = 0;
	}
	else {
		mFrameCount++;
	}
	mLastTick = curtick;

	mPaddle->Move(deltaTime);
}

void Game::Rendering()
{
	mRenderer.RenderLevel();
}
