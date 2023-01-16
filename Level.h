#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <tinyxml2.h>
#include <Brick.h>
#include <sstream>
#include <vector>
#include <SDL.h>
#include <unordered_set>
#include <SDL_image.h>
#include <Objects.h>
using namespace tinyxml2;

class Level
{
public:
	Level();
	~Level();

	void LoadLevel(SDL_Renderer* renderer);
	void LoseHp() { mLives--; };
	void AddToScore(int score) { mScore += score; };
	void IncreaseLevel() { mCurrentLevel++; };
	void ResetLevel() { mCurrentLevel = 0; };
	void ResetScore() { mScore = 0; };
	void ResetHp() { mLives = 3; };
	
	// Getters
	std::vector<Brick>* GetBricks() { return &mBrickArr; };
	std::string GetTexturePath(int id) const { return mTexturePath[id]; };
	std::string GetMusicPath() const { return mMusicPath; };

	SDL_Texture* GetBackgroundTexture() const { return mBackgroundTexture; };
	const SDL_Rect* GetTopWallRect() const { return &mTopWallRect; };
	const SDL_Rect* GetLivesRect() const { return &mLivesRect; };
	const SDL_Rect* GetBackgroundRect() const { return &mBackgroundRect; };

	int GetLives() const { return mLives; };
	int GetCurrentLevel() const { return mCurrentLevel; };
	int GetMaxLevel() const;
	int GetScore() const { return mScore; };

private:
	int GetValueFromFile(std::string text) const;
	std::string GetTextureFromFile() const;
	std::string GetBrickPlanFromFile() const; // Get the order of bricks from XML

	void SetBrickTypes(); // Look for types of bricks and add them to the map
	void AddBricksToArr(); // Make an array of bricks 
	void MakeBrickRects(); // Creates recs based on columns, rows, widths and heights
	void RemoveEmptySpaces(); // Adds the rects to the Brick class where there arent empty spaces

	std::unordered_map<std::string, Brick> mBrickTypes;
	std::vector<Brick> mBrickArr;
	std::vector<SDL_Rect> mBrickRect;

	XMLDocument* mDoc;
	XMLElement* mDataElement;
	SDL_Texture* mBackgroundTexture; // Create the texture here so we wouldn't have to do it every frame
	SDL_Rect mBackgroundRect;

	const int mBrickWidth = 80;
	const int mBrickHeight = 20;

	unsigned int mScore;
	unsigned int mLives;

	int mRows, mRowSpacing, mRowSpacingDuplicate, mColumns, mColumnSpacing, mColumnSpacingDuplicate, mTopMargin;
	std::string mBrickPlan;

	std::string mTexturePath[TEXTUREPATH_EMPTY];
	std::string mMusicPath;

	SDL_Rect mTopWallRect;
	SDL_Rect mLivesRect;

	std::string mLevels[3];
	int mCurrentLevel;
};

