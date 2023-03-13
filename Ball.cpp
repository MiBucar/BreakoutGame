#include "Ball.h"

Ball::Ball() : mMovePosX{0}, mMovePosY{0}, mDirectionX{0}, mDirectionY{0}
{
	mRect.w = mWidth; mRect.h = mHeight;
}

void Ball::Move(const SDL_Rect* paddleRect, float deltaTime, int dir)
{
	if (mLocked) {
		mMovePosY = static_cast<float>(paddleRect->y - mRect.h - 2);
		mMovePosX = static_cast<float>(paddleRect->x + paddleRect->w/2 - mRect.w/2);
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		if (state[SDL_SCANCODE_SPACE]) {
			mDirectionX =  (-1) * dir;
			mDirectionY = -1;
			mLocked = false;

			if (mDirectionX == 0) mDirectionX = 1;
		}
	}

	else {
		mMovePosX += mDirectionX * mVel * deltaTime;
		mMovePosY += mDirectionY * mVel * deltaTime;
	}


	mRect.x = mMovePosX;
	mRect.y = mMovePosY;
}

void Ball::Reset()
{
	mLocked = true;
}

void Ball::BlockBall(int side)
{
	if (side == SIDE_RIGHT) {
		mMovePosX = mMovePosX - 1;
		mDirectionX *= -1;
	}

	if (side == SIDE_LEFT) {
		mMovePosX = 1;
		mDirectionX *= -1;
	}

	if (side == SIDE_TOP) {
		mMovePosY = mMovePosY + 1;
		mDirectionY *= -1;
	}
}

void Ball::Bounce(int side)
{
	if (side == SIDE_LEFT) {
		mDirectionX = -1;
		mMovePosX = mMovePosX - 1;
	}

	else if (side == SIDE_RIGHT) {
		mDirectionX = 1;
		mMovePosX = mMovePosX + 1;
	}

	else if (side == SIDE_TOP) {
		mDirectionY = -1;
	}

	else if (side == SIDE_BOTTOM) {
		mDirectionY = 1;
		mMovePosY = mMovePosY + 1;
	}
}

void Ball::IncreaseSpeed()
{
	mVel += 35;
}

void Ball::RestartSpeed()
{
	mVel = 300;
}
