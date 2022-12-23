#include "Ball.h"

Ball::Ball()
{
	mTexture = "images//Ball.png";
	mRect.w = mWidth; mRect.h = mHeight;
}

void Ball::Move(const SDL_Rect* paddleRect, int dir)
{
	if (mLocked) {
		mRect.y = paddleRect->y - mRect.h - 2;
		mRect.x = paddleRect->x + paddleRect->w/2 - mRect.w/2;
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		if (state[SDL_SCANCODE_SPACE]) {
			mDirection = dir;
			mLocked = false;
		}
	}

	else {
		if (mDirection == -1) {
			mRect.x += 1;
		}
		if (mDirection == 1) {
			mRect.x -= 1;
		}
		mRect.y -= 1;
	}
}
