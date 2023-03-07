#include "Collision.h"

int Collision::Wall(const SDL_Rect* object, const int screenWidth, const int screenHeight, int topWallHeight)
{
    if (object->x + object->w >= screenWidth) {
        return SIDE_RIGHT; // Right Wall
    }

    if (object->x <= 0) {
        return SIDE_LEFT; // Left Wall
    }

    if (object->y <= topWallHeight) {
        return SIDE_TOP; // Top Wall
    } 

    if (object->y >= screenHeight) {
        return SIDE_BOTTOM;
    }
    return -1;
}


bool Collision::HasCollided(const SDL_Rect* rectOne, const SDL_Rect* rectTwo)
{
    if (SDL_HasIntersection(rectOne, rectTwo)) {
        return true;
    }
    return false;
}

int Collision::BallSurface(const SDL_Rect* rectOne, const SDL_Rect* rectTwo)
{
    if (rectOne->x + rectOne->w - 1 > rectTwo->x && rectOne->x < rectTwo->x + rectTwo->w - 1) {

        if (rectOne->y + rectOne->h == rectTwo->y + 1) {
            return SIDE_TOP;
        }

        else if (rectOne->y == rectTwo->y + rectTwo->h - 1) {
            return SIDE_BOTTOM;
        }
    }

    else {

        if (rectOne->x + rectOne->w - 1 == rectTwo->x) {
            return SIDE_LEFT;
        }

        else if (rectOne->x + 1 == rectTwo->x + rectTwo->w) {
            return SIDE_RIGHT;
        }
    }

    return -1;
}

