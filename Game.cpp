#include "Game.h"

Game::Game() : mIsRunning(true), mPaddle(new Paddle(mScreenWidth, mScreenHeight)), mBall(new Ball),
mRenderer(mPaddle, mBall, mScreenWidth, mScreenHeight)
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
	if (curtick - mFpsTicks >= mFpsDelay) {
		mFps = mFrameCount * (1000.0f / (curtick - mFpsTicks));
		mFpsTicks = curtick;
		mFrameCount = 0;
	}
	else {
		mFrameCount++;
	}
	mLastTick = curtick;

	CheckCollision();
	mPaddle->Move(deltaTime);
	mBall->Move(mPaddle->GetRect(), mPaddle->GetDirection());
}

void Game::CheckCollision()
{
	if (Collision::PaddleLeftWall(mPaddle->GetRect(), mScreenWidth)) {
		mPaddle->BlockPaddle(1);
	}
	else if (Collision::PaddleRightWall(mPaddle->GetRect(), mScreenWidth)) {
		mPaddle->BlockPaddle(0);
	}
}

void Game::Rendering()
{
	mRenderer.RenderLevel();
}
