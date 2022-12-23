#pragma once
#include <Renderer.h>
#include <Collision.h>

class Game
{
public:
	Game();
	~Game();
	int Run();

private:
	void PollEvents();
	void GameEvents();
	void Rendering();
	void Gameplay();
	void CheckCollision();

	const int mScreenWidth = 1280;
	const int mScreenHeight = 720;

	Paddle* mPaddle;
	Ball* mBall;
	Renderer mRenderer;

	unsigned int mLastTick, mFpsTicks, mFps, mFrameCount;
	const int mFpsDelay = 0;

	bool mIsRunning;
};

