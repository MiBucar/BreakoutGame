#include "Game.h"

Game::Game() : mIsRunning(true), mMenu(new Menu(mScreenWidth, mScreenHeight)), mLevel(new Level), mPaddle(new Paddle(mScreenWidth, mScreenHeight)), mBall(new Ball),
mRenderer(mMenu, mLevel, mPaddle, mBall, mScreenWidth, mScreenHeight), mMousePosX(0), mMousePosY(0)
{
	mLastTick = SDL_GetTicks();
	mFpsTicks = mLastTick;
	mFps = 0;
	mFrameCount = 0;
	mResult = { 0, 0, 0, 0 };

	// Ensure music and sound remain changed upon launching the game
	mMenu->ChangeMusicLevel(mData.GetAudioLevel(AUDIO_MUSIC));
	mMenu->ChangeSoundLevel(mData.GetAudioLevel(AUDIO_SOUND));
}

Game::~Game()
{
	delete mMenu; mMenu = nullptr;
	delete mLevel; mLevel = nullptr;
	delete mPaddle; mPaddle = nullptr;
	delete mBall; mBall = nullptr;

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
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
		case SDL_MOUSEBUTTONDOWN:
			HandleMouse(event.button);
			break;
		default:
			break;
		}
	}
	
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_ESCAPE]) {
		mScoreScreen = false;
		mAudioScreen = false;
	}

	if (mGameStarted) {
		if (state[SDL_SCANCODE_ESCAPE] && mPause == false) {
			mPause = true;
		}
	}

	int x, y;
	SDL_GetMouseState(&x, &y);
	mMousePosX = x; mMousePosY = y;
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
	float deltaTime = SetDeltaTime();

	if (mGameStarted) {
		if (!mDeathScreen && !mPause) {
			mPaddle->Move(deltaTime);
			mBall->Move(mPaddle->GetRect(), deltaTime, mPaddle->GetDirection());

			CheckCollision();
			CheckIfClear();

			// Open the "game over" menu after the player dies
			if (mLevel->GetLives() == 0) {
				TurnOnMenu();
			}
		}
	}
}

void Game::Rendering()
{
	// If the game isn't on, render the menu and update high scores
	if (!mGameStarted) {
		mRenderer.RenderMenu();
		if (mScoreScreen) {
			int arr[5];
			for (int i = 0; i < 5; i++) {
				arr[i] = mData.GetTopFiveScores().at(mData.GetTopFiveScores().size() - (i + 1));
			}
			mRenderer.RenderScoreScreen(arr);
		}
		else if (mAudioScreen) {
			// Get the current audio level for both music and sound
			mRenderer.RenderAudioScreen(mData.GetAudioLevel(AUDIO_MUSIC), mData.GetAudioLevel(AUDIO_SOUND));
		}
	}
	else {
		// If the game started, render the gameplay
		mRenderer.RenderLevel();

		// Update the high score
		if (mDeathScreen) {
			mRenderer.RenderDeathScreen(mData.GetHighScore());
		}
		else if (mPause) {
			mRenderer.RenderPauseScreen(mData.GetHighScore());
		}
	}
}

// Turn on the endgame screen and update the scores
void Game::TurnOnMenu()
{
	mData.AddScore(mLevel->GetScore());
	mDeathScreen = true;
}

void Game::CheckCollision()
{
	// Wall collisions
	mPaddle->BlockPaddle(Collision::Wall(mPaddle->GetRect(), mScreenWidth, mScreenHeight, mLevel->GetTopWallRect()->h));
	mBall->BlockBall(Collision::Wall(mBall->GetRect(), mScreenWidth, mScreenHeight, mLevel->GetTopWallRect()->h));
	
	// If the ball goes through the floor, reset and lose hp
	if ((Collision::Wall(mBall->GetRect(), mScreenWidth, mScreenHeight, mLevel->GetTopWallRect()->h)) == SIDE_BOTTOM) {
		Reset();
		mLevel->LoseHp();
	}

	// Paddle Ball collisions
	if (Collision::HasCollided(mBall->GetRect(), mPaddle->GetRect())) {
		SDL_IntersectRect(mBall->GetRect(), mPaddle->GetRect(), &mResult);
		mBall->Bounce(Collision::BallSurface(&mResult, mPaddle->GetRect()));
	}

	// Ball Brick collisions
	std::for_each(mLevel->GetBricks()->begin(), mLevel->GetBricks()->end(), [this](Brick &brick) {

		if (Collision::HasCollided(mBall->GetRect(), brick.GetRect())) {
			SDL_IntersectRect(mBall->GetRect(), brick.GetRect(), &mResult); // Create the point where these 2 intersect
			mResult.y += 1; // Increase the Y by 1 to match the brick Y
			mBall->Bounce(Collision::BallSurface(&mResult, brick.GetRect())); // Bounce the ball off
			mRenderer.PlaySound(SOUND_BRICKHIT);
			brick.Hit();
			if (brick.IsHit()) {
				mRenderer.PlaySound(SOUND_BRICKBREAK);
				mLevel->AddToScore(brick.GetBreakScore());
			}
		}
		});
}

void Game::CheckIfClear()
{
	int areHit = 0;

	// Increase areHit for each brick that has been hit
	for (int i = 0; i < mLevel->GetBricks()->size(); i++) { 
		if (mLevel->GetBricks()->at(i).IsHit()) {
			areHit++;
		}
	}

	if (areHit == mLevel->GetBricks()->size()) {

		// If its the last level load deathScreen instead of the next level
		if (mLevel->GetCurrentLevel() == mLevel->GetMaxLevel()) { 
			TurnOnMenu();
		}

		// Load next level
		else {
			mLevel->IncreaseLevel();
			mLevel->LoadLevel(mRenderer.GetRenderer());
			Reset(); // Reset paddle and ball position
		}
	}
}

void Game::HandleMouse(SDL_MouseButtonEvent btn)
{
	if (btn.button == SDL_BUTTON_LEFT) {
		if (!mGameStarted) {
			// Main Menu
			if (!mScoreScreen && !mAudioScreen) {
				if (CheckMousePos(BTN_PLAY)) {
					mRenderer.PlaySound(SOUND_BUTTON);
					mGameStarted = true;
					Reset();
					Restart();
				}

				else if (CheckMousePos(BTN_SCORES)) {
					mRenderer.PlaySound(SOUND_BUTTON);
					mScoreScreen = true;
				}

				else if (CheckMousePos(BTN_AUDIO)) {
					mRenderer.PlaySound(SOUND_BUTTON);
					mAudioScreen = true;
				}
			}
			// Leaderboard menu
			if (mScoreScreen && !mAudioScreen) {
				if (CheckMousePos(BTN_RESET)) {
					mRenderer.PlaySound(SOUND_BUTTON);
					mData.RestartScores();
				}
			}

			// Audio menu
			if (mAudioScreen && !mScoreScreen) {
				if (CheckMousePos(BTN_LEFTVOLUME_ONE)) {
					mRenderer.PlaySound(SOUND_BUTTON);
					mData.ChangeAudio(-1, AUDIO_MUSIC);
					mMenu->ChangeMusicLevel(mData.GetAudioLevel(AUDIO_MUSIC));
				}
				if (CheckMousePos(BTN_RIGHTVOLUME_ONE)) {
					mRenderer.PlaySound(SOUND_BUTTON);
					mData.ChangeAudio(1, AUDIO_MUSIC);
					mMenu->ChangeMusicLevel(mData.GetAudioLevel(AUDIO_MUSIC));
				}
				if (CheckMousePos(BTN_LEFTVOLUME_TWO)) {
					mRenderer.PlaySound(SOUND_BUTTON);
					mData.ChangeAudio(-1, AUDIO_SOUND);
					mMenu->ChangeSoundLevel(mData.GetAudioLevel(AUDIO_SOUND));
				}
				if (CheckMousePos(BTN_RIGHTVOLUME_TWO)) {
					mRenderer.PlaySound(SOUND_BUTTON);
					mData.ChangeAudio(1, AUDIO_SOUND);
					mMenu->ChangeSoundLevel(mData.GetAudioLevel(AUDIO_SOUND));
				}
			}

			if (mAudioScreen || mScoreScreen) {
				if (CheckMousePos(BTN_CLOSE)) {
					mRenderer.PlaySound(SOUND_BUTTON);
					mScoreScreen = false;
					mAudioScreen = false;
				}
			}
		}
		else {
			if (mDeathScreen) {

				if (CheckMousePos(BTN_HOME)) {
					mRenderer.PlaySound(SOUND_BUTTON);
					mDeathScreen = false;
					mGameStarted = false;
					Mix_PauseMusic();
				}

				else if (CheckMousePos(BTN_RESTART)) {
					mRenderer.PlaySound(SOUND_BUTTON);
					Restart();
					Reset();
					mDeathScreen = false;
				}
			}
			
			if (mPause) {
				if (CheckMousePos(BTN_HOME)) {
					mRenderer.PlaySound(SOUND_BUTTON);
					mGameStarted = false;
					mPause = false;
					Mix_PauseMusic();
				}
				if (CheckMousePos(BTN_RESUME)) {
					mRenderer.PlaySound(SOUND_BUTTON);
					mPause = false;
				}
			}
		}
	}
}

// Return true if the mouse position is inside the button
bool Game::CheckMousePos(int btn) 
{
	if (mMousePosX >= mMenu->GetButtonRect(btn)->x && mMousePosX <= mMenu->GetButtonRect(btn)->x + mMenu->GetButtonRect(btn)->w &&
		mMousePosY >= mMenu->GetButtonRect(btn)->y && mMousePosY <= mMenu->GetButtonRect(btn)->y + mMenu->GetButtonRect(btn)->h) {
		return true;
	}

	return false;
}

// Reset paddle's and ball's position
void Game::Reset() 
{
	mPaddle->Reset();
	mBall->Reset();
}

void Game::Restart()
{
	mLevel->ResetHp();
	mLevel->ResetLevel();
	mLevel->ResetScore();
	mLevel->LoadLevel(mRenderer.GetRenderer());
}

float Game::SetDeltaTime()
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

	return deltaTime;
}
