#pragma once
#include <Renderer.h>
#include <Collision.h>
#include <Data.h>

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
	void TurnOnMenu();
	void HandleMouse(SDL_MouseButtonEvent btn);

	void CheckCollision();
	void LoadLevel();
	bool CheckMousePos(int btn);

	void Reset();
	void Restart();

	float SetDeltaTime();

	const int mScreenWidth = 1280;
	const int mScreenHeight = 720;

	Data mData;
	Menu mMenu;
	Level mLevel;
	Paddle mPaddle;
	Ball mBall;
	Renderer mRenderer;
	SDL_Rect mResult;

	unsigned int mLastTick, mFpsTicks, mFps, mFrameCount;
	const int mFpsDelay = 0;

	bool mIsRunning;
	bool mGameStarted = false;
	bool mDeathScreen = false;
	bool mScoreScreen = false;
	bool mAudioScreen = false;
	bool mPause = false;

	int mMousePosX;
	int mMousePosY;
};

