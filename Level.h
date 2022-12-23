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
using namespace tinyxml2;

class Level
{
public:
	~Level();
	void LoadLevel(SDL_Renderer* renderer);
	std::vector<Brick> GetBricks() const { return mBrickArr; };
	SDL_Texture* GetBackgroundTexture() const { return mBackgroundTexture; };
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

	const int mBrickWidth = 80;
	const int mBrickHeight = 20;

	int mRows, mRowSpacing, mRowSpacingDuplicate, mColumns, mColumnSpacing, mColumnSpacingDuplicate;
	std::string mBrickPlan;
};

