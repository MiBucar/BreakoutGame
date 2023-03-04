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
    if (rectOne->y < rectTwo->y + rectTwo->h && rectOne->y != rectTwo->y) {
        if (rectOne->x == rectTwo->x) {
            return SIDE_LEFT;
        }

        else if (rectOne->x + 1 == rectTwo->x + rectTwo->w) {
            return SIDE_RIGHT;
        }
    }

    else if (rectOne->y == rectTwo->y || rectOne->y + 1 == rectTwo->y) {
        return SIDE_TOP;
    }

    else if (rectOne->y == rectTwo->y + rectTwo->h) {
        return SIDE_BOTTOM;
    }

    return -1;
}

