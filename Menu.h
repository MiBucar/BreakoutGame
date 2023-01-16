#pragma once
#include <SDL.h>
#include <iostream>
#include <Objects.h>
#include <SDL_mixer.h>

class Menu
{
public:
	Menu(const int width, const int height);

	std::string GetMenuTexture(int id) const { return mMenuScreenPath[id]; };
	std::string GetButtonTexture(int id) const { return mButtonTexture[id]; };
	std::string GetAudioLevelsTexture() const { return mAudioLevelsTexture; };

	const SDL_Rect* GetMenuRect(int id) const { return &mMenuScreenRect[id]; };
	const SDL_Rect* GetButtonRect(int id) const { return &mButtonRect[id]; };
	const SDL_Rect* GetHighScoreRect(int id) const { return &mHighScoresRect[id]; };
	const SDL_Rect* GetSoundLevelRect() const { return &mSoundLevelRect; };
	const SDL_Rect* GetMusicLevelRect() const { return &mMusicLevelRect; };

	void ChangeMusicLevel(int lvl);
	void ChangeSoundLevel(int lvl);

private:
	void InitRects();

	SDL_Rect mMenuScreenRect[MENU_EMPTY];
	std::string mMenuScreenPath[MENU_EMPTY];

	SDL_Rect mButtonRect[BTN_EMPTY];
	std::string mButtonTexture[BTN_EMPTY];

	SDL_Rect mHighScoresRect[HIGHSCORE_EMPTY];

	std::string mAudioLevelsTexture;
	SDL_Rect mSoundLevelRect;
	SDL_Rect mMusicLevelRect;

	const int mScreenWidth;
	const int mScreenHeight;
};

