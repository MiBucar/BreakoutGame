#pragma once
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<Objects.h>

class Data
{
public:
	void AddScore(int score);
	int GetHighScore();
	void RestartScores();
	std::vector<int> GetTopFiveScores();

	void ChangeAudio(int lvl, int type);
	int GetAudioLevel(int type);

private:
	void Swap(int* a, int* b);
	int Partition(std::vector<int>& array, int low, int high);
	void QuickSort(std::vector<int>& array, int low, int high);
	std::ofstream mFileO[FILE_EMPTY];
	std::ifstream mFileI[FILE_EMPTY];
};

