#include "Collision.h"

bool Collision::PaddleRightWall(const SDL_Rect* paddleRect, const int screenWidth)
{
    if (paddleRect->x + paddleRect->w >= screenWidth) {
        return true;
    }
    return false;
}

bool Collision::PaddleLeftWall(const SDL_Rect* paddleRect, const int screenWidth)
{
    if (paddleRect->x <= 0) {
        return true;
    }
    return false;
}
