#include "Renderer.h"

Renderer::Renderer(Menu* menu, Level* level, Paddle* paddle, Ball* ball, const int screenWidth, const int screenHeight) : mWindow(nullptr), mRenderer(nullptr),
mMenu(menu), mLevel(level), mPaddle(paddle), mBall(ball), mScreenWidth(screenWidth), mScreenHeight(screenHeight), mSrc{0, 0, 0, 0}
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		std::cout << "Couldnt initialize the renderer" << SDL_GetError() << "\n";
	}
	if (!IMG_Init(IMG_INIT_PNG)) {
		std::cout << "Couldnt initialize the image" << SDL_GetError() << "\n";
	}
	if (TTF_Init() == -1) {
		std::cout << "Couldnt initialize the font" << SDL_GetError() << "\n";
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Failed to init the mixer: " << SDL_GetError() << "\n";
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
		mFont = TTF_OpenFont("fonts//AmazingViews.ttf", 20);
		if (!mFont) {
			std::cout << "Failed to load the font: " << SDL_GetError() << "\n";
		}
	}

	mWhiteColor = { 255, 255, 255 };

	mLevel->LoadLevel(mRenderer);

	mMusic = Mix_LoadMUS(mLevel->GetMusicPath().c_str());
	mSound[SOUND_BUTTON] = Mix_LoadWAV("sounds//Button.wav");
	mSound[SOUND_BRICKHIT] = Mix_LoadWAV("sounds//BrickHit.wav");
	mSound[SOUND_BRICKBREAK] = Mix_LoadWAV("sounds//BrickBreak.wav");

	InitTextures();
}

Renderer::~Renderer()
{
	SDL_DestroyWindow(mWindow); mWindow = nullptr;
	SDL_DestroyRenderer(mRenderer); mRenderer = nullptr;

	for (int i = 0; i < TEXTURE_EMPTY; i++) {
		SDL_DestroyTexture(mTextures[i]); mTextures[i] = nullptr;
	}

	for (int i = 0; i < TEXT_EMPTY; i++) {
		SDL_FreeSurface(mTextSurface[i]); mTextSurface[i] = nullptr;
	}

	for (int i = 0; i < TEXT_EMPTY; i++) {
		SDL_DestroyTexture(mTextTexture[i]); mTextTexture[i] = nullptr;
	}

	for (int i = 0; i < HIGHSCORE_EMPTY; i++) {
		SDL_FreeSurface(mHighScoreSurface[i]); mHighScoreSurface[i] = nullptr;
		SDL_DestroyTexture(mHighScoreTexture[i]); mHighScoreTexture[i] = nullptr;
	}

	for (int i = 0; i < SOUND_EMPTY; i++) {
		Mix_FreeChunk(mSound[i]); mSound[i] = nullptr;
	}

	SDL_DestroyTexture(mAudioLevelTexture); mAudioLevelTexture = nullptr;
	mFont = nullptr; TTF_CloseFont(mFont);
}

void Renderer::RenderLevel()
{
	RenderBackground(mLevel->GetBackgroundTexture(), mLevel->GetBackgroundRect());
	RenderTopWall();
	RenderBricks();
	RenderPaddle();
	RenderBall();

	if (Mix_PlayingMusic() == 0 || Mix_PausedMusic() == 1) {
		Mix_PlayMusic(mMusic, -1);
	}
}

void Renderer::RenderMenu()
{
	RenderBackground(mTextures[TEXTURE_MENUBKG], mMenu->GetMenuRect(MENU_BACKGROUND));
	RenderButton(mTextures[TEXTURE_BTNPLAY], mMenu->GetButtonRect(BTN_PLAY));
	RenderButton(mTextures[TEXTURE_BTNSCORES], mMenu->GetButtonRect(BTN_SCORES));
	RenderButton(mTextures[TEXTURE_BTNAUDIO], mMenu->GetButtonRect(BTN_AUDIO));
}

void Renderer::RenderDeathScreen(int highScore)
{
	RenderBackground(mTextures[TEXTURE_DEATHSCREEN], mMenu->GetMenuRect(MENU_DEATHSCREEN));
	RenderButton(mTextures[TEXTURE_BTNHOME], mMenu->GetButtonRect(BTN_HOME));
	RenderButton(mTextures[TEXTURE_BTNRESTART], mMenu->GetButtonRect(BTN_RESTART));

	textDest = { mMenu->GetButtonRect(BTN_HOME)->x + 240, mMenu->GetButtonRect(BTN_HOME)->y - 82, 35, 45 };
	mText[TEXT_HIGHSCORE] = std::to_string(highScore);
	mTextSurface[TEXT_HIGHSCORE] = TTF_RenderText_Solid(mFont, mText[TEXT_HIGHSCORE].c_str(), mWhiteColor);
	mTextTexture[TEXT_HIGHSCORE] = SDL_CreateTextureFromSurface(mRenderer, mTextSurface[TEXT_HIGHSCORE]);
	mSrc = { 0, 0, 35, 45 };
	SDL_RenderCopy(mRenderer, mTextTexture[TEXT_HIGHSCORE], &mSrc, &textDest);
	ClearTextPointers(TEXT_HIGHSCORE);
}

void Renderer::RenderScoreScreen(int arr[5])
{
	RenderBackground(mTextures[TEXTURE_SCORESCREEN], mMenu->GetMenuRect(MENU_SCORESCREEN));
	RenderButton(mTextures[TEXTURE_BTNCLOSE], mMenu->GetButtonRect(BTN_CLOSE));
	RenderButton(mTextures[TEXTURE_BTNRESET], mMenu->GetButtonRect(BTN_RESET));

	mSrc = { 0, 0, mMenu->GetHighScoreRect(HIGHSCORE_ONE)->w, mMenu->GetHighScoreRect(HIGHSCORE_ONE)->h };

	RenderHighScoreText(HIGHSCORE_ONE, arr[0]);
	RenderHighScoreText(HIGHSCORE_TWO, arr[1]);
	RenderHighScoreText(HIGHSCORE_THREE, arr[2]);
	RenderHighScoreText(HIGHSCORE_FOUR, arr[3]);
	RenderHighScoreText(HIGHSCORE_FIVE, arr[4]);
}

void Renderer::RenderAudioScreen(int musicLevel, int soundLevel)
{
	RenderBackground(mTextures[TEXTURE_AUDIOSCREEN], mMenu->GetMenuRect(MENU_AUDIOSCREEN));
	RenderButton(mTextures[TEXTURE_BTNCLOSE], mMenu->GetButtonRect(BTN_CLOSE));

	RenderAudioLevels(musicLevel, AUDIO_MUSIC);
	RenderAudioLevels(soundLevel, AUDIO_SOUND);

	RenderButton(mTextures[TEXTURE_BTNLEFTVOLUME], mMenu->GetButtonRect(BTN_LEFTVOLUME_ONE));
	RenderButton(mTextures[TEXTURE_BTNRIGHTVOLUME], mMenu->GetButtonRect(BTN_RIGHTVOLUME_ONE));
	RenderButton(mTextures[TEXTURE_BTNLEFTVOLUME], mMenu->GetButtonRect(BTN_LEFTVOLUME_TWO));
	RenderButton(mTextures[TEXTURE_BTNRIGHTVOLUME], mMenu->GetButtonRect(BTN_RIGHTVOLUME_TWO));
}

void Renderer::RenderPauseScreen(int highScore)
{
	RenderBackground(mTextures[TEXTURE_PAUSESCREEN], mMenu->GetMenuRect(MENU_PAUSESCREEN));
	RenderButton(mTextures[TEXTURE_BTNHOME], mMenu->GetButtonRect(BTN_HOME));
	RenderButton(mTextures[TEXTURE_BTNRESUME], mMenu->GetButtonRect(BTN_RESUME));


	textDest = { mMenu->GetButtonRect(BTN_HOME)->x + 240, mMenu->GetButtonRect(BTN_HOME)->y - 82, 35, 45 };
	mText[TEXT_HIGHSCORE] = std::to_string(highScore);
	mTextSurface[TEXT_HIGHSCORE] = TTF_RenderText_Solid(mFont, mText[TEXT_HIGHSCORE].c_str(), mWhiteColor);
	mTextTexture[TEXT_HIGHSCORE] = SDL_CreateTextureFromSurface(mRenderer, mTextSurface[TEXT_HIGHSCORE]);
	mSrc = { 0, 0, 35, 45 };
	SDL_RenderCopy(mRenderer, mTextTexture[TEXT_HIGHSCORE], &mSrc, &textDest);
	ClearTextPointers(TEXT_HIGHSCORE);
}

void Renderer::RenderBricks()
{

	std::for_each(mLevel->GetBricks()->begin(), mLevel->GetBricks()->end(), [this](Brick& brick) {
		mSrc = { 0, 0, 80, 20 };

		if (brick.IsHit()) {
			brick.DestroyRect();
		}

		SDL_RenderCopy(mRenderer, brick.GetTexture(), &mSrc, brick.GetRect());
		});
}

void Renderer::RenderPaddle()
{
	mSrc = { 0, 0, 80, 20 };
	SDL_RenderCopy(mRenderer, mTextures[TEXTURE_PADDLE], &mSrc, mPaddle->GetRect());
}

void Renderer::RenderBall()
{
	mSrc = { 0, 0, 20, 20 };
	SDL_RenderCopy(mRenderer, mTextures[TEXTURE_BALL], &mSrc, mBall->GetRect());
}

void Renderer::RenderBackground(SDL_Texture* texture, const SDL_Rect* destRect)
{
	mSrc = { 0, 0, destRect->w, destRect->h };
	SDL_RenderCopy(mRenderer, texture, &mSrc, destRect);
}

void Renderer::RenderTopWall()
{
	mSrc = { 0, 0, mLevel->GetTopWallRect()->w, mLevel->GetTopWallRect()->h };
	SDL_RenderCopy(mRenderer, mTextures[TEXTURE_TOPWALL], &mSrc, mLevel->GetTopWallRect());
	RenderHealth();
	
	mText[TEXT_LEVEL] = std::to_string(mLevel->GetCurrentLevel() + 1);
	mTextSurface[TEXT_LEVEL] = TTF_RenderText_Solid(mFont, mText[TEXT_LEVEL].c_str(), mWhiteColor);
	mTextTexture[TEXT_LEVEL] = SDL_CreateTextureFromSurface(mRenderer, mTextSurface[TEXT_LEVEL]);
	textDest = { 130, 10, 15, 45 };
	mSrc = { 0, 0, 15, 45 };
	SDL_RenderCopy(mRenderer, mTextTexture[TEXT_LEVEL], &mSrc, &textDest);
	ClearTextPointers(TEXT_LEVEL);

	mText[TEXT_SCORE] = std::to_string(mLevel->GetScore());
	mTextSurface[TEXT_SCORE] = TTF_RenderText_Solid(mFont, mText[TEXT_SCORE].c_str(), mWhiteColor);
	mTextTexture[TEXT_SCORE] = SDL_CreateTextureFromSurface(mRenderer, mTextSurface[TEXT_SCORE]);
	textDest = { 1220, 10, 35, 45 };
	mSrc = { 0, 0, 35, 45 }; 
	SDL_RenderCopy(mRenderer, mTextTexture[TEXT_SCORE], &mSrc, &textDest);
	ClearTextPointers(TEXT_SCORE);
}

void Renderer::RenderButton(SDL_Texture* texture, const SDL_Rect* destRect)
{
	mSrc = { 0, 0, 300, 70 };
	SDL_RenderCopy(mRenderer, texture, &mSrc, destRect);
}

void Renderer::RenderHealth()
{
	int lives = mLevel->GetLives();
	int multiplier = 0;

	if (lives == 2) multiplier = 1;
	else if (lives == 1) multiplier = 2;
	else if (lives == 0) multiplier = 3;

	mSrc = { 150 * multiplier, 0, mLevel->GetLivesRect()->w, mLevel->GetLivesRect()->h };
	SDL_RenderCopy(mRenderer, mTextures[TEXTURE_LIVES], &mSrc, mLevel->GetLivesRect());
}

void Renderer::RenderHighScoreText(int id, int num)
{
	mHighScoreText[id] = std::to_string(num);
	mHighScoreSurface[id] = TTF_RenderText_Solid(mFont, mHighScoreText[id].c_str(), mWhiteColor);
	mHighScoreTexture[id] = SDL_CreateTextureFromSurface(mRenderer, mHighScoreSurface[id]);
	SDL_RenderCopy(mRenderer, mHighScoreTexture[id], &mSrc, mMenu->GetHighScoreRect(id));
}

void Renderer::RenderAudioLevels(int musicLevel, int type)
{
	int frame = 0;
	const SDL_Rect* dest = nullptr;

	switch (musicLevel)
	{
	case 21:
		frame = 0;
		break;
	case 18:
		frame = 250;
		break;
	case 15:
		frame = 500;
		break;
	case 12:
		frame = 750;
		break;
	case 9:
		frame = 1000;
		break;
	case 6:
		frame = 1250;
		break;
	case 3:
		frame = 1500;
		break;
	case 0:
		frame = 1750;
		break;
	default:
		break;
	}

	if (type == AUDIO_MUSIC) {
		dest = mMenu->GetMusicLevelRect();
	}
	else if (type == AUDIO_SOUND) {
		dest = mMenu->GetSoundLevelRect();
	}

	mAudioLevelAnimation.x = frame;
	mAudioLevelAnimation.y = 0;
	mAudioLevelAnimation.w = 250;
	mAudioLevelAnimation.h = 50;

	SDL_RenderCopy(mRenderer, mAudioLevelTexture, &mAudioLevelAnimation, dest);
	dest = nullptr;
}

void Renderer::PlaySound(int id)
{
	Mix_PlayChannel(-1, mSound[id], 0);
}

// set pointers to nullptr after a texture for text is created, otherwise it would overload the system
void Renderer::ClearTextPointers(int id)
{
	SDL_DestroyTexture(mTextTexture[id]); mTextTexture[id] = nullptr;
	SDL_FreeSurface(mTextSurface[id]); mTextSurface[id] = nullptr;
}

void Renderer::InitTextures()
{
	mTextures[TEXTURE_PADDLE] = IMG_LoadTexture(mRenderer, mLevel->GetTexturePath(TEXTUREPATH_PADDLE).c_str());
	mTextures[TEXTURE_BALL] = IMG_LoadTexture(mRenderer, mLevel->GetTexturePath(TEXTUREPATH_BALL).c_str());
	mTextures[TEXTURE_TOPWALL] = IMG_LoadTexture(mRenderer, mLevel->GetTexturePath(TEXTUREPATH_TOPWALL).c_str());
	mTextures[TEXTURE_LIVES] = IMG_LoadTexture(mRenderer, mLevel->GetTexturePath(TEXTUREPATH_LIVES).c_str());

	mTextures[TEXTURE_MENUBKG] = IMG_LoadTexture(mRenderer, mMenu->GetMenuTexture(MENU_BACKGROUND).c_str());
	mTextures[TEXTURE_DEATHSCREEN] = IMG_LoadTexture(mRenderer, mMenu->GetMenuTexture(MENU_DEATHSCREEN).c_str());
	mTextures[TEXTURE_SCORESCREEN] = IMG_LoadTexture(mRenderer, mMenu->GetMenuTexture(MENU_SCORESCREEN).c_str());
	mTextures[TEXTURE_AUDIOSCREEN] = IMG_LoadTexture(mRenderer, mMenu->GetMenuTexture(MENU_AUDIOSCREEN).c_str());
	mTextures[TEXTURE_PAUSESCREEN] = IMG_LoadTexture(mRenderer, mMenu->GetMenuTexture(MENU_PAUSESCREEN).c_str());

	mTextures[TEXTURE_BTNPLAY] = IMG_LoadTexture(mRenderer, mMenu->GetButtonTexture(BTN_PLAY).c_str());
	mTextures[TEXTURE_BTNSCORES] = IMG_LoadTexture(mRenderer, mMenu->GetButtonTexture(BTN_SCORES).c_str());
	mTextures[TEXTURE_BTNHOME] = IMG_LoadTexture(mRenderer, mMenu->GetButtonTexture(BTN_HOME).c_str());
	mTextures[TEXTURE_BTNRESTART] = IMG_LoadTexture(mRenderer, mMenu->GetButtonTexture(BTN_RESTART).c_str());
	mTextures[TEXTURE_BTNCLOSE] = IMG_LoadTexture(mRenderer, mMenu->GetButtonTexture(BTN_CLOSE).c_str());
	mTextures[TEXTURE_BTNRESET] = IMG_LoadTexture(mRenderer, mMenu->GetButtonTexture(BTN_RESET).c_str());
	mTextures[TEXTURE_BTNAUDIO] = IMG_LoadTexture(mRenderer, mMenu->GetButtonTexture(BTN_AUDIO).c_str());
	mTextures[TEXTURE_BTNRESUME] = IMG_LoadTexture(mRenderer, mMenu->GetButtonTexture(BTN_RESUME).c_str());
	mTextures[TEXTURE_BTNLEFTVOLUME] = IMG_LoadTexture(mRenderer, mMenu->GetButtonTexture(BTN_LEFTVOLUME_ONE).c_str());
	mTextures[TEXTURE_BTNRIGHTVOLUME] = IMG_LoadTexture(mRenderer, mMenu->GetButtonTexture(BTN_RIGHTVOLUME_ONE).c_str());

	mAudioLevelTexture = IMG_LoadTexture(mRenderer, mMenu->GetAudioLevelsTexture().c_str());
}
