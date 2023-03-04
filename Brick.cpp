#include "Brick.h"

Brick::Brick(std::string id, std::string texture, std::string hitSound, std::string breakSound, int hp, int breakScore)
{
	mId = id;
	mTexturePath = texture;
	mHitSound = hitSound;
	mBreakSound = breakSound;
	mHp = hp;
	mBreakScore = breakScore;
}

Brick::~Brick()
{
	SDL_DestroyTexture(mTexture); mTexture = nullptr;
}

void Brick::CreateTexture(SDL_Renderer* renderer)
{
	mTexture = IMG_LoadTexture(renderer, mTexturePath.c_str());
}

void Brick::Hit()
{
	mHp--;
	if (mHp == 0) {
		mIsHit = true;
	}
}

void Brick::DestroyRect()
{
	mBrickRect.w = 0;
	mBrickRect.h = 0;
}
