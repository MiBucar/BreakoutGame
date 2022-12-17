#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Brick
{
public:
	Brick(std::string id, std::string texture, std::string hitSound, std::string breakSound, int hp, int breakScore);
	~Brick();

	void SetRect(SDL_Rect rect) { mBrickRect = rect; };
	void CreateTexture(SDL_Renderer* renderer);
	void Hit() { mHp--; };

	// Getters
	SDL_Texture* GetTexture() const { return mTexture; };
	const SDL_Rect* GetRect() const { return &mBrickRect; };
	int GetHp() const { return mHp; };
	std::string GetId() const { return mId; };
	std::string GetTexturePatch() const { return mTexturePath; };
private:
	SDL_Rect mBrickRect;
	SDL_Texture *mTexture;
	std::string mId;
	std::string mTexturePath;
	std::string mHitSound;
	std::string mBreakSound;
	int mHp;
	int mBreakScore;
};

