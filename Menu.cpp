#include "Menu.h"

Menu::Menu(const int width, const int height) : mScreenWidth(width), mScreenHeight(height)
{
	mMenuScreenPath[MENU_BACKGROUND] = "images//MenuBackground.jpg";
	mMenuScreenPath[MENU_DEATHSCREEN] = "images//DeathScreen.png";
	mMenuScreenPath[MENU_SCORESCREEN] = "images//ScoresScreen.png";
	mMenuScreenPath[MENU_AUDIOSCREEN] = "images//AudioScreen.png";
	mMenuScreenPath[MENU_PAUSESCREEN] = "images//PauseScreen.png";

	mButtonTexture[BTN_PLAY] = "images//MenuPlay.png";
	mButtonTexture[BTN_HOME] = "images//HomeButton.png";
	mButtonTexture[BTN_RESTART] = "images//RestartButton.png";
	mButtonTexture[BTN_SCORES] = "images//ScoresButton.png";
	mButtonTexture[BTN_CLOSE] = "images//CloseButton.png";
	mButtonTexture[BTN_RESET] = "images//ResetButton.png";
	mButtonTexture[BTN_AUDIO] = "images//AudioButton.png";
	mButtonTexture[BTN_RESUME] = "images//ResumeButton.png";
	mButtonTexture[BTN_LEFTVOLUME_ONE] = "images//LeftVolumeButton.png";
	mButtonTexture[BTN_RIGHTVOLUME_ONE] = "images//RightVolumeButton.png";
	mButtonTexture[BTN_LEFTVOLUME_TWO] = mButtonTexture[BTN_LEFTVOLUME_ONE];
	mButtonTexture[BTN_RIGHTVOLUME_TWO] = mButtonTexture[BTN_RIGHTVOLUME_ONE];

	mAudioLevelsTexture = "images//AudioLevels.png";

	InitRects();
}

void Menu::ChangeMusicLevel(int lvl)
{
	Mix_VolumeMusic(lvl);
}

void Menu::ChangeSoundLevel(int lvl)
{
	Mix_Volume(-1, lvl);
}

void Menu::InitRects()
{
	mMusicLevelRect.w = 250;
	mMusicLevelRect.h = 50;
	mMusicLevelRect.x = mScreenWidth / 2 - (mMusicLevelRect.w / 2);
	mMusicLevelRect.y = 320;

	mSoundLevelRect.w = 250;
	mSoundLevelRect.h = 50;
	mSoundLevelRect.x = mScreenWidth / 2 - (mSoundLevelRect.w / 2);
	mSoundLevelRect.y = 440;

	mMenuScreenRect[MENU_BACKGROUND] = { 0, 0, 1280, 720 };
	mMenuScreenRect[MENU_DEATHSCREEN] = { mScreenWidth / 2 - 300, mScreenHeight / 2 - 200, 600, 360 };
	mMenuScreenRect[MENU_SCORESCREEN] = mMenuScreenRect[MENU_DEATHSCREEN];
	mMenuScreenRect[MENU_AUDIOSCREEN] = mMenuScreenRect[MENU_DEATHSCREEN];
	mMenuScreenRect[MENU_PAUSESCREEN] = mMenuScreenRect[MENU_DEATHSCREEN];

	for (int i = 0; i < HIGHSCORE_EMPTY; i++) {
		mHighScoresRect[i].x = mMenuScreenRect[MENU_SCORESCREEN].x + 80;
		mHighScoresRect[i].w = 35;
		mHighScoresRect[i].h = 40;
	}

	// Hardcoded Y values to make it fit better
	mHighScoresRect[HIGHSCORE_ONE].y = mMenuScreenRect[MENU_SCORESCREEN].y + 122;
	mHighScoresRect[HIGHSCORE_TWO].y = mMenuScreenRect[MENU_SCORESCREEN].y + 165;
	mHighScoresRect[HIGHSCORE_THREE].y = mMenuScreenRect[MENU_SCORESCREEN].y + 207;
	mHighScoresRect[HIGHSCORE_FOUR].y = mMenuScreenRect[MENU_SCORESCREEN].y + 249;
	mHighScoresRect[HIGHSCORE_FIVE].y = mMenuScreenRect[MENU_SCORESCREEN].y + 290;

	for (int i = 0; i < BTN_EMPTY; i++) {
		mButtonRect[i].w = 300;
		mButtonRect[i].h = 70;
	}

	int screenCenter = mScreenWidth / 2 - 150;

	mButtonRect[BTN_PLAY].x = screenCenter;
	mButtonRect[BTN_PLAY].y = 250;

	mButtonRect[BTN_SCORES].x = screenCenter;
	mButtonRect[BTN_SCORES].y = mButtonRect[BTN_PLAY].y + 100;

	mButtonRect[BTN_AUDIO].x = screenCenter;
	mButtonRect[BTN_AUDIO].y = mButtonRect[BTN_SCORES].y + 100;

	mButtonRect[BTN_HOME].x = screenCenter;
	mButtonRect[BTN_HOME].y = mMenuScreenRect[MENU_DEATHSCREEN].y + 195;

	mButtonRect[BTN_RESUME].x = screenCenter;
	mButtonRect[BTN_RESUME].y = mMenuScreenRect[MENU_DEATHSCREEN].y + 275;

	mButtonRect[BTN_RESTART].x = screenCenter;
	mButtonRect[BTN_RESTART].y = mMenuScreenRect[MENU_DEATHSCREEN].y + 275;

	mButtonRect[BTN_CLOSE].x = screenCenter - 155;
	mButtonRect[BTN_CLOSE].y = mMenuScreenRect[MENU_SCORESCREEN].y + mMenuScreenRect[MENU_SCORESCREEN].h + 20;

	mButtonRect[BTN_RESET].x = screenCenter + 155;
	mButtonRect[BTN_RESET].y = mMenuScreenRect[MENU_SCORESCREEN].y + mMenuScreenRect[MENU_SCORESCREEN].h + 20;

	mButtonRect[BTN_LEFTVOLUME_ONE] = {
		mScreenWidth / 2 - 190, 320, 60, 50
	};

	mButtonRect[BTN_RIGHTVOLUME_ONE] = {
		mScreenWidth / 2 + 130, 320, 60, 50
	};

	mButtonRect[BTN_LEFTVOLUME_TWO] = {
		mScreenWidth / 2 - 190, 440, 60, 50
	};

	mButtonRect[BTN_RIGHTVOLUME_TWO] = {
		mScreenWidth / 2 + 130, 440, 60, 50
	};
}
