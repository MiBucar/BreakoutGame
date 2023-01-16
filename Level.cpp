#include "Level.h"

Level::Level() : mTopWallRect{0, 0, 1280, 70}, mLivesRect{640 - 75, 20, 150, 30}
{
	mCurrentLevel = 0;
	mLevels[0] = "levels//Level1.xml"; mLevels[1] = "levels//Level2.xml"; mLevels[2] = "levels//Level3.xml";

	mTopMargin = mTopWallRect.h;

	mBackgroundRect = { 0, 0, 1280, 720 };

	mTexturePath[TEXTUREPATH_BALL] = "images//Ball.png";
	mTexturePath[TEXTUREPATH_PADDLE] = "images//Platform.png";
	mTexturePath[TEXTUREPATH_TOPWALL] = "images//TopWall.png";
	mTexturePath[TEXTUREPATH_LIVES] = "images//Lives.png";

	mMusicPath = "sounds//M.O.O.N. - Hydrogen.mp3";

	mScore = 0;
	mLives = 3;
}

Level::~Level()
{
	delete mDoc; mDoc = nullptr;
	mDataElement = nullptr;
	SDL_DestroyTexture(mBackgroundTexture); mBackgroundTexture = nullptr;
}

void Level::LoadLevel(SDL_Renderer* renderer)
{
	mBrickArr.clear();

	mDoc = new XMLDocument();
	mDoc->LoadFile(mLevels[mCurrentLevel].c_str());
	mDataElement = mDoc->FirstChildElement("Level")->FirstChildElement("BrickTypes");
	mRows = GetValueFromFile("RowCount");
	mRowSpacing = GetValueFromFile("RowSpacing");
	mRowSpacingDuplicate = mRowSpacing;
	mColumns = GetValueFromFile("ColumnCount");
	mBackgroundTexture = IMG_LoadTexture(renderer, GetTextureFromFile().c_str());
	mBrickPlan = GetBrickPlanFromFile();

	SetBrickTypes();
	AddBricksToArr();
	MakeBrickRects();
	RemoveEmptySpaces();

	for (auto &brick : mBrickArr) {
		brick.CreateTexture(renderer);
	}
}

int Level::GetMaxLevel() const
{
	int maxLevel = 0;
	for (auto level : mLevels) {
		maxLevel++;
	}
	return maxLevel - 1;
}

int Level::GetValueFromFile(std::string text) const
{
	int value;
	std::stringstream valueInFile(mDoc->FirstChildElement("Level")->FirstChildElement(text.c_str())->GetText());
	valueInFile >> value;
	return value;
}

std::string Level::GetTextureFromFile() const
{
	std::string text = mDoc->FirstChildElement("Level")->FirstChildElement("BackgroundTexture")->GetText();
	return text;
}

std::string Level::GetBrickPlanFromFile() const
{
	std::string text = mDoc->FirstChildElement("Level")->FirstChildElement("Bricks")->GetText();

	// Remove all the unnecessary blanks from the brickPlan
	std::string temp = "";
	for (int i = 0; i < text.size(); ++i) {
		if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z')) {
			temp = temp + text[i];
		}
	}
	text = temp;
	return text;
}

void Level::SetBrickTypes()
{
	for (XMLElement* brickElement = mDataElement->FirstChildElement("BrickType");
		brickElement != NULL; brickElement = brickElement->NextSiblingElement())
	{
		std::string id = brickElement->FirstChildElement("Id")->GetText();
		std::string texture = brickElement->FirstChildElement("Texture")->GetText();
		std::string hitSound = brickElement->FirstChildElement("HitSound")->GetText();
		std::string breakSound = brickElement->FirstChildElement("BreakSound")->GetText();

		int hp;
		std::stringstream hpF(brickElement->FirstChildElement("HitPoints")->GetText());
		hpF >> hp;

		int breakScore;
		std::stringstream breakScoreF(brickElement->FirstChildElement("BreakScore")->GetText());
		breakScoreF >> breakScore;

		mBrickTypes.emplace(id, Brick(id, texture, hitSound, breakSound, hp, breakScore));
	}
}

void Level::AddBricksToArr()
{
	for (auto id : mBrickPlan) {
		if (id != 'E') {
			std::string str(1, id);
			mBrickArr.push_back(mBrickTypes.at(str));
		}
	}
}

void Level::MakeBrickRects()
{
	int index = 0;
	for (int i = 0; i < mRows; i++) {

		mColumnSpacing = GetValueFromFile("ColumnSpacing");
		mColumnSpacingDuplicate = mColumnSpacing;

		for (int j = 0; j < mColumns; j++) {
			SDL_Rect brickRect = { mColumnSpacing, mRowSpacing + mTopMargin, mBrickWidth, mBrickHeight };
			mBrickRect.push_back(brickRect);
			mColumnSpacing = mBrickWidth + mColumnSpacing + mColumnSpacingDuplicate;
			index++;
		}
		mRowSpacing = mBrickHeight + mRowSpacing + mRowSpacingDuplicate;
	}
}

void Level::RemoveEmptySpaces()
{
	std::unordered_set<int> letterE;
	std::vector<int> Es;
	for (int i = 0; i < mBrickPlan.size(); i++) {
		if (mBrickPlan[i] == 'E') {
			letterE.insert(i);
		}
	}

	int index = 0;
	for (int i = 0; i < mBrickArr.size(); i++) {
		while (letterE.find(index) != letterE.end()) { // if letter E's index is the same as Rect's, increment an index
			index++;
		}
		mBrickArr[i].SetRect(mBrickRect[index]);
		index++;
	}
}
