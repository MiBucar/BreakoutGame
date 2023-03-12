#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <Level.h>
#include <Paddle.h>
#include <Ball.h>
#include <vector>
#include <algorithm>
#include <Menu.h>

class Renderer
{
public:
	Renderer(Menu* menu, Level* level, Paddle* paddle, Ball* ball, const int screenWidth, const int screenHeight);
	~Renderer();

	void RenderLevel();
	void RenderMenu();
	void RenderDeathScreen(int highScore);
	void RenderScoreScreen(int arr[5]);
	void RenderAudioScreen(int musicLevel, int soundLevel);
	void RenderPauseScreen(int highScore);

	void PlaySound(int id);

	// Getters
	SDL_Renderer* GetRenderer() const { return mRenderer; };

	int GetScreenWidth() const { return mScreenWidth; };
	int GetScreenHeight() const { return mScreenHeight; };

private:
	void InitTextures();

	void RenderBricks();
	void RenderPaddle();
	void RenderBall();
	void RenderBackground(SDL_Texture* texture, const SDL_Rect* destRect);
	void RenderTopWall();
	void RenderButton(SDL_Texture* texture, const SDL_Rect* destRect);
	void RenderHealth();
	void RenderHighScoreText(int id, int num);
	void RenderAudioLevels(int musicLevel, int type);
	void ClearTextPointers(int id);

	SDL_Renderer* mRenderer;
	SDL_Window* mWindow;
	SDL_Texture* mTextures[TEXTURE_EMPTY];
	SDL_Rect mSrc;
	SDL_Rect mLivesRect[3];
	SDL_Rect textDest;
	SDL_Rect mAudioLevelAnimation;
	SDL_Color mWhiteColor;
	TTF_Font* mFont;

	Mix_Chunk* mSound[SOUND_EMPTY];
	Mix_Music* mMusic;

	std::string mText[TEXT_EMPTY];
	SDL_Surface* mTextSurface[TEXT_EMPTY];
	SDL_Texture* mTextTexture[TEXT_EMPTY];

	SDL_Texture* mAudioLevelTexture;

	// Text in leaderboard menu
	std::string mHighScoreText[HIGHSCORE_EMPTY];
	SDL_Surface* mHighScoreSurface[HIGHSCORE_EMPTY];
	SDL_Texture* mHighScoreTexture[HIGHSCORE_EMPTY];

	Menu* mMenu;
	Level *mLevel;
	Paddle* mPaddle;
	Ball* mBall;

	const int mScreenWidth;
	const int mScreenHeight;
};

