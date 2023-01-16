#include "Paddle.h"

Paddle::Paddle(const int screenWidth, const int screenHeight) : mScreenWidth(screenWidth), mScreenHeight(screenHeight)
{
	mMovePosX = mScreenWidth / 2 - mWidth / 2;
	mRect = { static_cast<int>(mMovePosX), mScreenHeight - 40, mWidth, mHeight }; // Starting position
}

void Paddle::Move(float deltaTime)
{
	mRect.x = mMovePosX;
	mDirectionX = 0;

	const Uint8* state = SDL_GetKeyboardState(nullptr);

	if (state[SDL_SCANCODE_D]) {
		mDirectionX = 1;
	}
	if (state[SDL_SCANCODE_A]) {
		mDirectionX = -1;
	}

	mMovePosX += mVelX * mDirectionX * deltaTime;
}

void Paddle::Reset()
{
	mMovePosX = mScreenWidth / 2 - mWidth / 2;
}

void Paddle::BlockPaddle(int side)
{
	if (side == SIDE_LEFT) {
		mMovePosX = 1;
	}
	else if (side == SIDE_RIGHT) {
		mMovePosX = mScreenWidth - mRect.w - 1;
	}
}
