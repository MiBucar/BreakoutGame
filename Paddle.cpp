#include "Paddle.h"

Paddle::Paddle(const int screenWidth, const int screenHeight) : mScreenWidth(screenWidth), mScreenHeight(screenHeight)
{
	mTexture = "images//Platform.png";
	mMovePos.x = mScreenWidth / 2 - mWidth / 2;
	mRect = { static_cast<int>(mMovePos.x), mScreenHeight - 40, mWidth, mHeight }; // Starting position
}

void Paddle::Move(float deltaTime)
{
	mRect.x = mMovePos.x;
	mDirectionX = 0;

	const Uint8* state = SDL_GetKeyboardState(nullptr);

	if (state[SDL_SCANCODE_D]) {
		mDirectionX = 1;
	}
	if (state[SDL_SCANCODE_A]) {
		mDirectionX = -1;
	}

	mMovePos.x += mVelX * mDirectionX * deltaTime;
}

void Paddle::BlockPaddle(int side)
{
	if (side == 0) {
		mRect.x = mScreenWidth - mRect.w;
	}
	else if (side == 1) {
		mRect.x = 0;
	}
}
