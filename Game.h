#pragma once
#include <Renderer.h>

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

	const int mScreenWidth = 1280;
	const int mScreenHeight = 720;

	unsigned int mLastTick, mFpsTicks, mFps, mFrameCount;

	Paddle* mPaddle;
	Renderer mRenderer;

	bool mIsRunning;
};

