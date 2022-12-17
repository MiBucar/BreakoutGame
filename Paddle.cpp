#include "Paddle.h"

Paddle::Paddle(const int screenWidth, const int screenHeight)
{
	mTexture = "images//Platform.png";
	mRect = { screenWidth / 2 - mWidth / 2, screenHeight - 40, mWidth, mHeight }; // Starting position
	moveVel.x = 3 * mMoveSpeed;
}

void Paddle::Spawn()
{
}

void Paddle::Move(float deltaTime)
{
	int direction = 0;

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_D]) {
		direction += 1;
	}
	if (state[SDL_SCANCODE_A]) {
		direction -= 1;
	}

	if (direction != 0) {
		mRect.x += deltaTime * moveVel.x * direction;
	}
}
